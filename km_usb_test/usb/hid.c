/*
 * hid.c
 *
 */

#include <avr/io.h>
#include "usb.h"
#include "usb_config.h"

uint8_t hid_report[USB_MAX_PACKET_SIZE];
uint8_t report_size = USB_MAX_PACKET_SIZE;


/* Send HID reports
 */
void hid_send_report(void)
{
	PORTC.OUTCLR = PIN4_bm;
	PORTC.OUTSET = PIN4_bm;
	USARTC1.DATA = 'H';
	for (uint8_t i = 0; i < 8; i++)
		hid_report[i] += (i+1);

	if (usb_ep_is_ready(0x81))
		usb_ep_start_in(0x81, hid_report, 3, false);
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
	return hid_report;
}
