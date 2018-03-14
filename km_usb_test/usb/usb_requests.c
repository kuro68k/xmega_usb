/* usb_xmega.c
 *
 * Copyright 2011-2014 Nonolith Labs
 * Copyright 2014 Technical Machine
 * Copyright 2017-2018 Paul Qureshi
 *
 * Handle supported USB requests
 */

#include <avr/io.h>
#include "usb.h"
#include "usb_config.h"
#include "hid.h"

USB_SetupPacket usb_setup;
__attribute__((__aligned__(4))) uint8_t ep0_buf_in[USB_EP0_BUFFER_SIZE];
__attribute__((__aligned__(4))) uint8_t ep0_buf_out[USB_EP0_BUFFER_SIZE];
volatile uint8_t usb_configuration;

/**************************************************************************************************
* Handle standard setup requests
*/
void usb_handle_standard_setup_requests(void)
{
	switch (usb_setup.bRequest)
	{
		case USB_REQ_GetStatus:
			// Device:		D0	Self powered
			//				D1	Remote wake-up
			// Interface:	(all reserved)
			// Endpoint:	D0 endpoint halted
			ep0_buf_in[0] = 0;
			ep0_buf_in[1] = 0;
			usb_ep0_in(2);
			return usb_ep0_out();

		case USB_REQ_ClearFeature:
		case USB_REQ_SetFeature:
			// not implemented
			usb_ep0_in(0);
			return usb_ep0_out();

		case USB_REQ_SetAddress:
			// USB.ADDR must only change after the IN transaction has completed,
			// see USB_TRNCOMPL_vect vector
			usb_ep0_in(0);
			return usb_ep0_out();

		case USB_REQ_GetDescriptor:
		{
			uint8_t type = usb_setup.wValue >> 8;
			uint8_t index = usb_setup.wValue & 0xFF;
			uint16_t size = usb_cb_get_descriptor(type, index);

			if (size)
			{
				if (size > usb_setup.wLength)	// host requested partial descriptor
					size = usb_setup.wLength;

				return usb_ep_start_in(0x80, ep0_buf_in, size, true);
			}
			else
				return usb_ep0_stall();
		}

		case USB_REQ_GetConfiguration:
			ep0_buf_in[0] = usb_configuration;
			usb_ep0_in(1);
			return usb_ep0_out();

		case USB_REQ_SetConfiguration:
			if (usb_cb_set_configuration((uint8_t)usb_setup.wValue))
			{
				usb_ep0_in(0);
				usb_configuration = (uint8_t)(usb_setup.wValue);
				return usb_ep0_out();
			}
			return usb_ep0_stall();

		case USB_REQ_SetInterface:
			if (usb_cb_set_interface(usb_setup.wIndex, usb_setup.wValue))
			{
				usb_ep0_in(0);
				return usb_ep0_out();
			}
			return usb_ep0_stall();

		default:
			return usb_ep0_stall();
	}
}

/**************************************************************************************************
* Handle class setup requests
*/
void usb_handle_class_setup_requests(void)
{
#ifdef USB_HID
	switch (usb_setup.bRequest)
	{
		// IN requests
/*
		case USB_REQ_GetDescriptor:
		{
			uint8_t type = usb_setup.wValue >> 8;
			uint8_t index = usb_setup.wValue & 0xFF;
			uint16_t size = usb_cb_get_descriptor(type, index);

			if (size)
			{
				if (size > usb_setup.wLength)
					size = usb_setup.wLength;

				return usb_ep_start_in(0x80, ep0_buf_in, size, true);
			}
			else
				return usb_ep0_stall();
		}
*/
		case USB_HIDREQ_GET_REPORT:
		{
			uint16_t bytes_in = -1;
			switch(usb_setup.wValue >> 8)
			{
				case USB_HID_REPORT_TYPE_INPUT:
					hid_send_report();
					return usb_ep0_out();
					//bytes_in = hid_cb_get_report_input(ep0_buf_in, usb_setup.wValue & 0xFF);
					//break;
				case USB_HID_REPORT_TYPE_OUTPUT:
					bytes_in = hid_cb_get_report_output(ep0_buf_in, usb_setup.wValue & 0xFF);
					break;
				case USB_HID_REPORT_TYPE_FEATURE:
					bytes_in = hid_cb_get_report_feature(ep0_buf_in, usb_setup.wValue & 0xFF);
					break;
			}
			if (bytes_in == -1)
				return usb_ep0_stall();
			usb_ep0_in(bytes_in);
			return usb_ep0_out();
		}

		case USB_HIDREQ_GET_IDLE:
			return usb_ep0_stall();

		case USB_HIDREQ_GET_PROTOCOL:
			return usb_ep0_stall();

		// OUT requests
		case USB_HIDREQ_SET_REPORT:
		{
			switch(usb_setup.wValue >> 8)
			{
				case USB_HID_REPORT_TYPE_INPUT:
					return (hid_cb_set_report_input(ep0_buf_out, usb_setup.wLength, usb_setup.wValue & 0xFF) ?
						usb_ep0_out() : usb_ep0_stall);
				case USB_HID_REPORT_TYPE_OUTPUT:
					return (hid_cb_set_report_output(ep0_buf_out, usb_setup.wLength, usb_setup.wValue & 0xFF) ?
						usb_ep0_out() : usb_ep0_stall);
				case USB_HID_REPORT_TYPE_FEATURE:
					return (hid_cb_set_report_feature(ep0_buf_out, usb_setup.wLength, usb_setup.wValue & 0xFF) ?
						usb_ep0_out() : usb_ep0_stall);
			}
			return usb_ep0_stall();
		}

		case USB_HIDREQ_SET_IDLE:
			usb_ep0_in(0);
			return usb_ep0_out();

		case USB_HIDREQ_SET_PROTOCOL:
			return usb_ep0_stall();

		default:
			return usb_ep0_stall();
	}
#else
	return usb_ep0_stall();
#endif
}

/* Handle setup packets
 */
void usb_handle_setup(void)
{
	switch (usb_setup.bmRequestType & USB_REQTYPE_TYPE_MASK)
	{
		case USB_REQTYPE_STANDARD:
			return usb_handle_standard_setup_requests();

		case USB_REQTYPE_CLASS:
			return usb_handle_class_setup_requests();

		case USB_REQTYPE_VENDOR:
		default:
			return usb_cb_control_setup();
	}
}

void usb_handle_msft_compatible(const USB_MicrosoftCompatibleDescriptor* msft_compatible) {
	if (usb_setup.wIndex == 0x0004) {
		uint16_t len = usb_setup.wLength;
		if (len > msft_compatible->dwLength) {
			len = msft_compatible->dwLength;
		}
		if (len > USB_EP0_MAX_PACKET_SIZE) {
			len = USB_EP0_MAX_PACKET_SIZE;
		}
		memcpy(ep0_buf_in, msft_compatible, len);
		usb_ep_start_in(0x80, ep0_buf_in, len, false);
		return usb_ep0_out();
	} else {
		return usb_ep0_stall();
	}
}
