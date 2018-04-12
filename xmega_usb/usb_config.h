/* usb_config.h
 *
 * Copyright 2018 Paul Qureshi
 *
 * USB stack configuration
 */


#ifndef USB_CONFIG_H_
#define USB_CONFIG_H_


/* USB vendor and product IDs, version number
 */
#define USB_VID				0x9999
#define USB_PID				0x007F

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

#define WCID_REQUEST_ID			0x22
#define WCID_REQUEST_ID_STR		u"\x22"


/* Add DFU run-time interface
 */
#define USB_DFU_RUNTIME


/* Enable HID, otherwise vendor specific bulk endpoints
 */
#define USB_HID
#define USB_HID_REPORT_SIZE		3
#define USB_HID_POLL_RATE_MS	0x08		// HID polling rate in milliseconds


/* USB clock configuration
 */
#define USB_USE_PLL			// configure in usb_configure_clock() in usb_xmega.c
//#define USB_USE_RC32


/* DFU callbacks
 */
extern void	CCPWrite(volatile uint8_t *address, uint8_t value);
static inline void dfu_cb_enter_dfu_mode(void)
{
	// watchdog reset gives USB time to send response
	asm("wdr");
	CCPWrite(&WDT.CTRL, WDT_WPER_128CLK_gc | WDT_ENABLE_bm | WDT_WCEN_bm);
}


/* HID callbacks
 */

// GET_REPORT handlers. *report is USB_MAX_PACKET_SIZE.
// Return number of bytes in report, or -1 if not supported
/*
static inline uint16_t hid_cb_get_report_input(uint8_t *report, uint8_t report_id)
{
	return -1;
}*/

static inline uint16_t hid_cb_get_report_output(uint8_t *report, uint8_t report_id)
{
	return -1;
}

static inline uint16_t hid_cb_get_report_feature(uint8_t *report, uint8_t report_id)
{
	return -1;
}

// SET_REPORT handlers.
// Return true if report OK
static inline bool hid_cb_set_report_input(uint8_t *report, uint16_t report_length, uint8_t report_id)
{
	return false;
}

static inline bool hid_cb_set_report_output(uint8_t *report, uint16_t report_length, uint8_t report_id)
{
	return false;
}

static inline bool hid_cb_set_report_feature(uint8_t *report, uint16_t report_length, uint8_t report_id)
{
	return false;
}


#endif /* USB_CONFIG_H_ */
