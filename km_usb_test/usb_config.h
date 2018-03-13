/*
 * usb_config.h
 *
 */


#ifndef USB_CONFIG_H_
#define USB_CONFIG_H_


/* USB vendor and product IDs, version number
 */
#define USB_VID				0x9999
#define USB_PID				0x005C

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
//#define	USB_WCID
//#define USB_WCID_EXTENDED


/* Add DFU run-time interface
 */
//#define USB_DFU_RUNTIME


/* Enable HID, otherwise vendor specific
 */
#define USB_HID
#define USB_MAX_PACKET_SIZE		64


/* USB clock configuration
 */
#define USB_USE_PLL			// configure in usb_configure_clock() in usb_xmega.c
//#define USB_USE_RC32

//#define USB_LOW_SPEED

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


/* HID callbacks
 */

// GET_REPORT handlers. *report is USB_MAX_PACKET_SIZE.
// Return number of bytes in report, or -1 if not supported
static inline uint16_t hid_cb_get_report_input(uint8_t *report, uint8_t report_id)
{
	report[0] = 1;
	report[1] = 99;
	report[2] = 77;
	report[3] = 55;
	return 4;
	//return -1;
}

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
