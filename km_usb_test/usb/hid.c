/*
 * hid.c
 *
 */

#include <avr/io.h>
#include "usb.h"
#include "usb_config.h"

struct {
	USB_SetupPacket setup;
	uint8_t data[USB_MAX_PACKET_SIZE];
} hid_report;

uint8_t report_size = USB_MAX_PACKET_SIZE;


/* Send HID reports
 */
void hid_send_report(void)
{
	PORTC.OUTCLR = PIN4_bm;
	PORTC.OUTSET = PIN4_bm;
	USARTC1.DATA = 'H';
	for (uint8_t i = 1; i < 8; i++)
		hid_report.data[i] += i;
	//hid_report.data[2] += 2;
	hid_report.setup.bmRequestType = 0b10100001;
	hid_report.setup.bRequest = USB_DTYPE_HID;
	hid_report.setup.wValue = 1<<8;
	hid_report.setup.wLength = 4;
	//usb_ep_start_in(0x81, (uint8_t *)&hid_report, report_size, false);
	//usb_ep_start_in(0x81, (uint8_t *)&hid_report, sizeof(USB_SetupPacket) + 4, false);
	//usb_ep_start_in(0x81, hid_report.data, report_size, false);
	usb_ep_start_in(0x81, hid_report.data, 3, false);
}

/* Set reports size
 */
void hid_set_report_size(uint8_t size)
{
	report_size = size;
}

/* Get report buffer
 */
uint8_t *hid_get_report_buffer(void)
{
	return hid_report.data;
}
