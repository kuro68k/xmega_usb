/*
 * km_usb_test.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "xmega/usb_xmega.h"

USB_ENDPOINTS(1);

int main(void)
{
	usb_configure_clock();

	PORTCFG.CLKEVOUT = PORTCFG_CLKOUTSEL_CLK1X_gc | PORTCFG_CLKOUT_PC7_gc;
	PORTC.DIRSET = PIN7_bm;
	//for(;;);

	// Enable USB interrupts
	USB.INTCTRLA = /*USB_SOFIE_bm |*/ USB_BUSEVIE_bm | USB_INTLVL_MED_gc;
	USB.INTCTRLB = USB_TRNIE_bm | USB_SETUPIE_bm;

	usb_init();

	USB.CTRLA |= USB_FIFOEN_bm;

	PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
	sei();

	usb_attach();

	for(;;);
}

