#ifndef HID_KEYBOARD_H
#define HID_KEYBOARD_H

#include "usbd_core.h"

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

extern volatile uint8_t hid_state;
extern USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t write_buffer[8];

#if defined(__cplusplus)
extern "C" {
#endif

void hid_keyboard_init(uint8_t busid, uint32_t reg_base);
void hid_keyboard_send(uint8_t busid, uint8_t data[6]);

#if defined(__cplusplus)
}
#endif

#endif // HID_KEYBOARD_H