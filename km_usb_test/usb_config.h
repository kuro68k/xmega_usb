/*
 * usb_config.h
 *
 */


#ifndef USB_CONFIG_H_
#define USB_CONFIG_H_


/* USB vendor and product IDs, version number
 */
#define USB_VID				0x9999
#define USB_PID				0x0037

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


/* USB clock configuration
 */
#define USB_USE_PLL			// configure in usb_configure_clock() in usb_xmega.c
//#define USB_USE_RC32



#endif /* USB_CONFIG_H_ */