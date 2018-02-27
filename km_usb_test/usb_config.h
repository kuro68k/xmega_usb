/*
 * usb_config.h
 *
 */


#ifndef USB_CONFIG_H_
#define USB_CONFIG_H_


/* USB vendor and product IDs, version number
 */
#define USB_VID				0x9999
#define USB_PID				0x003A

#define USB_VERSION_MAJOR	1
#define USB_VERSION_MINOR	0


/* USB strings
 */
#define USB_STRING_MANUFACTURER		"Example Manufacturer"
#define USB_STRING_PRODUCT			"Example Device"


/* Generate a USB serial number from the MCU's unique identifiers. Can be
 * disabled to save flash memory.
 */
#define	USB_SERIAL_NUMBER


/* Use Microsoft WCID descriptors
 */
#define	USB_WCID
//#define USB_WCID_EXTENDED


/* Add DFU run-time interface
 */
#define USB_DFU_RUNTIME


/* Enable HID, otherwise vendor specific
 */
#define USB_HID


/* USB clock configuration
 */
#define USB_USE_PLL			// configure in usb_configure_clock() in usb_xmega.c
//#define USB_USE_RC32


/* DFU callbacks
 */
static inline void dfu_runtime_cb_enter_dfu_mode(void)
{
	// watchdog reset gives USB time to send response
	asm("cli");
	asm("wdr");
	CCP = CCP_IOREG_gc;
	WDT.CTRL = WDT_WPER_128CLK_gc | WDT_ENABLE_bm | WDT_WCEN_bm;
	asm("sei");
	return;
}


#endif /* USB_CONFIG_H_ */
