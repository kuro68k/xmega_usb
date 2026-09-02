/* hid.h
 *
 * Copyright 2018 Paul Qureshi
 *
 * Human Interface Device support
 */

#ifndef HID_H_
#define HID_H_

#include <stdbool.h>


extern uint8_t hid_report[USB_HID_REPORT_SIZE];
extern uint8_t out_hid_report[USB_HID_OUT_REPORT_SIZE];
extern volatile bool out_hid_report_received_SIG;

extern void hid_send_report(void);
extern void hid_wait_until_ready(void);
extern bool hid_is_ready(void);
extern void hid_get_report(void);

inline static void hid_get_report_received_callback(void)
{
	out_hid_report_received_SIG = 0xFF;
}


#endif /* HID_H_ */