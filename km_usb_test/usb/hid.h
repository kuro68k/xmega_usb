/*
 * hid.h
 *
 */


#ifndef HID_H_
#define HID_H_


extern void hid_send_report(void);
extern void hid_set_report_size(uint8_t size);
extern uint8_t *hid_get_report_buffer(void);


#endif /* HID_H_ */