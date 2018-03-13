#include <avr/io.h>
#include "usb.h"
#include "usb_config.h"

USB_SetupPacket usb_setup;
__attribute__((__aligned__(4))) uint8_t ep0_buf_in[USB_EP0_BUFFER_SIZE];
__attribute__((__aligned__(4))) uint8_t ep0_buf_out[USB_EP0_BUFFER_SIZE];
volatile uint8_t usb_configuration;

uint16_t usb_ep0_in_byte_count;
const uint8_t* usb_ep0_in_ptr;

// handle multi-packet transfers for endpoint 0
// called in transaction complete interrupt
void usb_ep0_in_multi(void) {
	uint16_t tsize = usb_ep0_in_byte_count;

	if (tsize > USB_EP0_MAX_PACKET_SIZE) {
		tsize = USB_EP0_MAX_PACKET_SIZE;
	}

	memcpy(ep0_buf_in, usb_ep0_in_ptr, tsize);
	usb_ep_start_in(0x80, ep0_buf_in, tsize, false);

	if (tsize == 0) {
		usb_ep0_out();
	}

	usb_ep0_in_byte_count -= tsize;
	usb_ep0_in_ptr += tsize;
}


/* Handle standard setup packet device requests
 */
void usb_handle_standard_setup_requests(void)
{
//	USARTC1.DATA = 0xAA;
//	USARTC1.DATA = usb_setup.bRequest;

	switch (usb_setup.bRequest)
	{
		case USB_REQ_GetStatus:
			ep0_buf_in[0] = 0;
			ep0_buf_in[1] = 0;
			usb_ep0_in(2);
			return usb_ep0_out();

		case USB_REQ_ClearFeature:
		case USB_REQ_SetFeature:
			USARTC1.DATA = 0x5F;
			usb_ep0_in(0);
			return usb_ep0_out();

		case USB_REQ_SetAddress:
			usb_ep0_in(0);
			return usb_ep0_out();

		case USB_REQ_GetDescriptor:
		{
			uint8_t type = usb_setup.wValue >> 8;
			uint8_t index = usb_setup.wValue & 0xFF;
			const uint8_t* descriptor = 0;
			uint16_t size = usb_cb_get_descriptor(type, index, &descriptor);

			if (size && descriptor)
			{
				if (size > usb_setup.wLength)	// host requested partial descriptor
					size = usb_setup.wLength;

				usb_ep_start_in(0x80, descriptor, size, true);
/*				if (descriptor == ep0_buf_in)
				{
					usb_ep0_in_byte_count = 0;
					usb_ep_start_in(0x80, ep0_buf_in, size, false);
				}
				else
				{
					usb_ep0_in_byte_count = size;
					usb_ep0_in_ptr = descriptor;
					usb_ep0_in_multi();
				}
*/
				return;
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
			USARTC1.DATA = 0x55;
			return usb_ep0_stall();
	}
}

/* Handle class setup packet device requests
 */
void usb_handle_class_setup_requests(void)
{
//	USARTC1.DATA = 0xBB;
//	USARTC1.DATA = usb_setup.bRequest;

#ifdef USB_HID
	switch (usb_setup.bRequest)
	{
		// IN requests
		case USB_REQ_GetDescriptor:
		{
			uint8_t type = usb_setup.wValue >> 8;
			uint8_t index = usb_setup.wValue & 0xFF;
			const uint8_t* descriptor = 0;
			uint16_t size = usb_cb_get_descriptor(type, index, &descriptor);

			if (size && descriptor)
			{
				if (size > usb_setup.wLength)
					size = usb_setup.wLength;

				usb_ep_start_in(0x80, ep0_buf_in, size, true);
				/*
				if (descriptor == ep0_buf_in)
				{
					usb_ep0_in_byte_count = 0;
					usb_ep_start_in(0x80, ep0_buf_in, size, true);
				}
				else
				{
					usb_ep0_in_byte_count = size;
					usb_ep0_in_ptr = descriptor;
					usb_ep0_in_multi();
				}*/

				return;
			}
			else
				return usb_ep0_stall();
		}

		case USB_HIDREQ_GET_REPORT:
		{
			uint16_t bytes_in = -1;
			switch(usb_setup.wValue >> 8)
			{
				case USB_HID_REPORT_TYPE_INPUT:
					bytes_in = hid_cb_get_report_input(ep0_buf_in, usb_setup.wValue & 0xFF);
					break;
				case USB_HID_REPORT_TYPE_OUTPUT:
					bytes_in = hid_cb_get_report_output(ep0_buf_in, usb_setup.wValue & 0xFF);
					break;
				case USB_HID_REPORT_TYPE_FEATURE:
					bytes_in = hid_cb_get_report_feature(ep0_buf_in, usb_setup.wValue & 0xFF);
					break;
			}
			PORTC.OUTCLR = PIN4_bm;
			PORTC.OUTSET = PIN4_bm;
			USARTC1.DATA = bytes_in;
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
			//return usb_ep0_stall();

		case USB_HIDREQ_SET_PROTOCOL:
			USARTC1.DATA = 0x57;
			return usb_ep0_stall();

		default:
			USARTC1.DATA = 0x56;
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

void usb_handle_control_out_complete(void) {
	if ((usb_setup.bmRequestType & USB_REQTYPE_TYPE_MASK) == USB_REQTYPE_STANDARD) {
		// Let the status stage proceed
	} else {
		// empty callback
		//usb_cb_control_out_completion();
	}
}

void usb_handle_control_in_complete(void) {
	if ((usb_setup.bmRequestType & USB_REQTYPE_TYPE_MASK) == USB_REQTYPE_STANDARD) {
		switch (usb_setup.bRequest){
			case USB_REQ_SetAddress:
				usb_set_address(usb_setup.wValue & 0x7F);
				return;
			//case USB_REQ_GetDescriptor:
			//	usb_ep0_in_multi();
			//	return;
		}
	} else {
		// empty
		//usb_cb_control_in_completion();
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

void* usb_string_to_descriptor(char* str) {
	USB_StringDescriptor* desc = (((USB_StringDescriptor*)ep0_buf_in));
	uint16_t len = strlen(str);
	const uint16_t maxlen = (USB_EP0_MAX_PACKET_SIZE - 2)/2;
	if (len > maxlen) len = maxlen;
	desc->bLength = USB_STRING_LEN(len);
	desc->bDescriptorType = USB_DTYPE_String;
	for (int i=0; i<len; i++) {
		desc->bString[i] = str[i];
	}
	return desc;
}
