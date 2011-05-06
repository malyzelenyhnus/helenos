/*
 * Copyright (c) 2011 Lubos Slovak
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

/** @addtogroup drvusbhid
 * @{
 */
/** @file
 * USB HID subdriver mappings.
 */

#include "subdrivers.h"
#include "usb/classes/hidut.h"
#include "usb/classes/hidpath.h"

#include "lgtch-ultrax/lgtch-ultrax.h"
#include "mouse/mousedev.h"

static usb_hid_subdriver_usage_t path_kbd[] = {
	{USB_HIDUT_PAGE_KEYBOARD, 0}, 
	{0, 0}
};

static usb_hid_subdriver_usage_t path_mouse[] = {
	{USB_HIDUT_PAGE_GENERIC_DESKTOP, USB_HIDUT_USAGE_GENERIC_DESKTOP_MOUSE},
	{USB_HIDUT_PAGE_GENERIC_DESKTOP, 
	 USB_HIDUT_USAGE_GENERIC_DESKTOP_POINTER},
	{0, 0}
};

static usb_hid_subdriver_usage_t path_mouse2[] = {
	{USB_HIDUT_PAGE_GENERIC_DESKTOP, USB_HIDUT_USAGE_GENERIC_DESKTOP_X},
	{0, 0}
};

static usb_hid_subdriver_usage_t lgtch_path[] = {
	{0xc, 0},
	{0, 0}
};

const usb_hid_subdriver_mapping_t usb_hid_subdrivers[] = {
	{
		path_kbd,
		-1,
		USB_HID_PATH_COMPARE_END 
		| USB_HID_PATH_COMPARE_USAGE_PAGE_ONLY,
		-1,
		-1,
		{
			.init = usb_kbd_init,
			.deinit = usb_kbd_deinit,
			.poll = usb_kbd_polling_callback,
			.poll_end = NULL
		},
		
	},
	{
		lgtch_path,
		1,
		USB_HID_PATH_COMPARE_END 
		| USB_HID_PATH_COMPARE_USAGE_PAGE_ONLY,
		0x046d,
		0xc30e,
		{
			.init = usb_lgtch_init,
			.deinit = usb_lgtch_deinit,
			.poll = usb_lgtch_polling_callback,
			.poll_end = NULL
		}
	},
	{
		path_mouse,
		-1,
		USB_HID_PATH_COMPARE_COLLECTION_ONLY,
		-1,
		-1,
		{
			.init = usb_mouse_init,
			.deinit = usb_mouse_deinit,
			.poll = usb_mouse_polling_callback,
			.poll_end = NULL
		}
	},
	{
		path_mouse2,
		-1,
		USB_HID_PATH_COMPARE_END 
		| USB_HID_PATH_COMPARE_USAGE_PAGE_ONLY,
		-1,
		-1,
		{
			.init = usb_mouse_init,
			.deinit = usb_mouse_deinit,
			.poll = usb_mouse_polling_callback,
			.poll_end = NULL
		}
	},
	{NULL, -1, 0, -1, -1, {NULL, NULL, NULL, NULL}}
};

/**
 * @}
 */
