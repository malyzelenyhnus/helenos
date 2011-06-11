/*
 * Copyright (c) 2010 Lubos Slovak
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * - The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/** @addtogroup libusbhid
 * @{
 */
/** @file
 * @brief USB HID Usage Tables - LED page.
 */
#ifndef LIBUSBHID_UTLED_H_
#define LIBUSBHID_UTLED_H_

typedef enum {
	USB_HID_LED_UNDEFINED = 0,
	USB_HID_LED_NUM_LOCK,
	USB_HID_LED_CAPS_LOCK,
	USB_HID_LED_SCROLL_LOCK,
	USB_HID_LED_COMPOSE,
	USB_HID_LED_KANA,
	USB_HID_LED_POWER,
	USB_HID_LED_SHIFT,
	USB_HID_LED_DND,
	USB_HID_LED_MUTE,
	USB_HID_LED_TONE_ENABLE,
	USB_HID_LED_HIGH_CUT_FILTER,
	USB_HID_LED_LOW_CUT_FILTER,
	USB_HID_LED_EQ_ENABLE,
	USB_HID_LED_SOUND_FIELD_ON,
	USB_HID_LED_SURROUND_ON,
	USB_HID_LED_REPEAT,
	USB_HID_LED_STEREO,
	USB_HID_LED_SAMPLING_RATE_DETECT,
	USB_HID_LED_SPINNING,
	USB_HID_LED_CAV,
	USB_HID_LED_CLV,
	USB_HID_LED_RECORDING_FORMAT_DETECT,
	USB_HID_LED_OFF_HOOK,
	USB_HID_LED_RING,
	USB_HID_LED_MESSAGE_WAITING,
	USB_HID_LED_DATA_MODE,
	USB_HID_LED_BATTERY_OPERATION,
	USB_HID_LED_BATTERY_OK,
	USB_HID_LED_BATTERY_LOW,
	USB_HID_LED_SPEAKER,
	USB_HID_LED_HEAD_SET,
	USB_HID_LED_HOLD,
	USB_HID_LED_MICRO,
	USB_HID_LED_COVERAGE,
	USB_HID_LED_NIGHT_MODE,
	USB_HID_LED_SEND_CALLS,
	USB_HID_LED_CALL_PICKUP,
	USB_HID_LED_CONFERENCE,
	USB_HID_LED_STAND_BY,
	USB_HID_LED_CAMERA_ON,
	USB_HID_LED_CAMERA_OFF,
	USB_HID_LED_ON_LINE,
	USB_HID_LED_OFF_LINE,
	USB_HID_LED_BUSY,
	USB_HID_LED_READY,
	USB_HID_LED_PAPER_OUT,
	USB_HID_LED_PAPER_JAM,
	USB_HID_LED_REMOTE,
	USB_HID_LED_FORWARD,
	USB_HID_LED_REVERSE,
	USB_HID_LED_STOP,
	USB_HID_LED_REWIND,
	USB_HID_LED_FAST_FORWARD,
	USB_HID_LED_PLAY,
	USB_HID_LED_PAUSE,
	USB_HID_LED_RECORD,
	USB_HID_LED_ERROR,
	USB_HID_LED_USAGE_SELECTED_IND,
	USB_HID_LED_USAGE_IN_USE_IND,
	USB_HID_LED_USAGE_MULTI_MODE_IND,
	USB_HID_LED_IND_ON,
	USB_HID_LED_IND_FLASH,
	USB_HID_LED_IND_SLOW_BLINK,
	USB_HID_LED_IND_FAST_BLINK,
	USB_HID_LED_IND_OFF,
	USB_HID_LED_FLASH_ON_TIME,
	USB_HID_LED_SLOW_BLINK_ON_TIME,
	USB_HID_LED_SLOW_BLINK_OFF_TIME,
	USB_HID_LED_FAST_BLINK_ON_TIME,
	USB_HID_LED_FAST_BLINK_OFF_TIME,
	USB_HID_LED_USAGE_IND_COLOR,
	USB_HID_LED_IND_RED,
	USB_HID_LED_IND_GREEN,
	USB_HID_LED_IND_AMBER,
	USB_HID_LED_GENERIC_IND,
	USB_HID_LED_SYSTEM_SUSPEND,
	USB_HID_LED_EXTERNAL_POWER
} usb_hid_usage_led_t;

#endif /* LIBUSB_UTLED_H_ */
/**
 * @}
 */