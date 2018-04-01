/*
 * main.c
 *
 * Copyright 2018 Paul Qureshi
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usb_xmega.h"
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

	// clock output check
	//PORTCFG.CLKEVOUT = PORTCFG_CLKOUTSEL_CLK1X_gc | PORTCFG_CLKOUT_PC7_gc;
	//PORTC.DIRSET = PIN7_bm;
	//for(;;);

	// Enable USB interrupts
	USB.INTCTRLA = /*USB_SOFIE_bm |*/ USB_BUSEVIE_bm | USB_INTLVL_MED_gc;
	USB.INTCTRLB = USB_TRNIE_bm | USB_SETUPIE_bm;

	usb_init();

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();

	usb_attach();

#ifdef USB_HID
	for(;;)
	{
		for (uint8_t i = 0; i < USB_HID_REPORT_SIZE; i++)
			hid_report[i] += (i+1);
		_delay_ms(50);
		hid_send_report();
	}
#endif

	for(;;);
}
