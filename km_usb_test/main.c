/*
 * km_usb_test.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "xmega/usb_xmega.h"
#include "hid.h"

int main(void)
{
	// debug USART
	PORTC.DIRSET = PIN7_bm | PIN5_bm | PIN4_bm;
	PORTC.OUTCLR = PIN4_bm;
	_delay_us(10);
	PORTC.OUTSET = PIN4_bm;
	USARTC1.BAUDCTRLA = 0;
	USARTC1.BAUDCTRLB = 0;
	USARTC1.CTRLA = 0;
	USARTC1.CTRLB = USART_TXEN_bm | USART_CLK2X_bm;
	USARTC1.CTRLC = USART_CHSIZE_8BIT_gc;// | USART_CMODE_MSPI_gc;
	USARTC1.DATA = 'R';

	usb_configure_clock();

	//PORTCFG.CLKEVOUT = PORTCFG_CLKOUTSEL_CLK1X_gc | PORTCFG_CLKOUT_PC7_gc;
	//PORTC.DIRSET = PIN7_bm;
	//for(;;);

	// Enable USB interrupts
	USB.INTCTRLA = /*USB_SOFIE_bm |*/ USB_BUSEVIE_bm | USB_INTLVL_MED_gc;
	USB.INTCTRLB = USB_TRNIE_bm | USB_SETUPIE_bm;

	usb_init();

	//USB.CTRLA |= USB_FIFOEN_bm;

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();

	//usb_attach();
	//usb_ep_enable(0x81, USB_EP_TYPE_BULK_gc, 64);	// interrupt == bulk (see manual)

#ifdef USB_HID
	hid_set_report_size(4);
	uint8_t *buffer = hid_get_report_buffer();
	for(;;)
	{
		_delay_ms(50);
		buffer[1]++;
		//if (usb_ep_ready(0x81))
			//asm("nop");
			hid_send_report();
	}
#endif
	for(;;);
}
