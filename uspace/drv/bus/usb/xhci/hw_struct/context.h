/*
 * Copyright (c) 2017 Ondrej Hlavaty <aearsis@eideo.cz>
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

/** @addtogroup drvusbxhci
 * @{
 */
/** @file
 * Context data structures of the xHC.
 *
 * Most of them are to be initialized to zero and passed ownership to the HC,
 * so they are mostly read-only.
 *
 * Feel free to write a setter when in need.
 */

#ifndef XHCI_CONTEXT_H
#define XHCI_CONTEXT_H

#include <stdint.h>
#include "common.h"

/**
 * Endpoint context: section 6.2.3
 */
typedef struct xhci_endpoint_ctx {
	xhci_dword_t data[5];
	xhci_dword_t reserved[3];

#define XHCI_EP_STATE(ctx)              XHCI_DWORD_EXTRACT((ctx).data[0],  2,  0)
#define XHCI_EP_MULT(ctx)               XHCI_DWORD_EXTRACT((ctx).data[0],  9,  8)
#define XHCI_EP_MAX_P_STREAMS(ctx)      XHCI_DWORD_EXTRACT((ctx).data[0], 14, 10)
#define XHCI_EP_LSA(ctx)                XHCI_DWORD_EXTRACT((ctx).data[0], 15, 15)
#define XHCI_EP_INTERVAL(ctx)           XHCI_DWORD_EXTRACT((ctx).data[0], 23, 16)

#define XHCI_EP_ERROR_COUNT(ctx)        XHCI_DWORD_EXTRACT((ctx).data[1],  2,  1)
#define XHCI_EP_TYPE(ctx)               XHCI_DWORD_EXTRACT((ctx).data[1],  5,  3)
#define XHCI_EP_HID(ctx)                XHCI_DWORD_EXTRACT((ctx).data[1],  7,  7)
#define XHCI_EP_MAX_BURST_SIZE(ctx)     XHCI_DWORD_EXTRACT((ctx).data[1], 15,  8)
#define XHCI_EP_MAX_PACKET_SIZE(ctx)    XHCI_DWORD_EXTRACT((ctx).data[1], 31, 16)

#define XHCI_EP_DCS(ctx)                XHCI_DWORD_EXTRACT((ctx).data[2],  0,  0)
#define XHCI_EP_TR_DPTR(ctx)            XHCI_DWORD_EXTRACT((ctx).data[2], 63,  4)

} __attribute__((packed)) xhci_ep_ctx_t;

/**
 * Slot context: section 6.2.2
 */
typedef struct xhci_slot_ctx {
	xhci_dword_t data [4];
	xhci_dword_t reserved [4];

#define XHCI_SLOT_ROUTE_STRING(ctx)     XHCI_DWORD_EXTRACT((ctx).data[0], 19,  0)
#define XHCI_SLOT_SPEED(ctx)            XHCI_DWORD_EXTRACT((ctx).data[0], 23, 20)
#define XHCI_SLOT_MTT(ctx)              XHCI_DWORD_EXTRACT((ctx).data[0], 25, 25)
#define XHCI_SLOT_CTX_ENTRIES(ctx)      XHCI_DWORD_EXTRACT((ctx).data[0], 31, 27)

#define XHCI_SLOT_MAX_EXIT_LATENCY(ctx) XHCI_DWORD_EXTRACT((ctx).data[1], 15,  0)
#define XHCI_SLOT_ROOT_HUB_PORT(ctx)    XHCI_DWORD_EXTRACT((ctx).data[1], 23, 16)
#define XHCI_SLOT_NUM_OF_PORTS(ctx)     XHCI_DWORD_EXTRACT((ctx).data[1], 31, 24)

#define XHCI_SLOT_TT_HUB_SLOT_ID(ctx)   XHCI_DWORD_EXTRACT((ctx).data[2],  7,  0)
#define XHCI_SLOT_TT_PORT_NUM(ctx)      XHCI_DWORD_EXTRACT((ctx).data[2], 15,  8)
#define XHCI_SLOT_TT_THINK_TIME(ctx)    XHCI_DWORD_EXTRACT((ctx).data[2], 17, 16)
#define XHCI_SLOT_INTERRUPTER(ctx)      XHCI_DWORD_EXTRACT((ctx).data[2], 31, 22)

#define XHCI_SLOT_DEVICE_ADDRESS(ctx)   XHCI_DWORD_EXTRACT((ctx).data[3],  7,  0)
#define XHCI_SLOT_SLOT_STATE(ctx)       XHCI_DWORD_EXTRACT((ctx).data[3], 31, 27)

} __attribute__((packed)) xhci_slot_ctx_t;

/**
 * Device context: section 6.2.1
 */
typedef struct xhci_device_ctx {
	xhci_slot_ctx_t slot_ctx;
	xhci_ep_ctx_t endpoint_ctx [31];
} __attribute__((packed)) xhci_device_ctx_t;

/**
 * Stream context: section 6.2.4 {
 */
typedef struct xhci_stream_ctx {
	uint64_t data [2];
} __attribute__((packed)) xhci_stream_ctx_t;

/**
 * Input control context: section 6.2.5.1
 * TODO: According to section 6.2.5.1 figure 78,
 *       the context size register value in
 *       hccparams1 dictates whether input
 *       control context shall have 32 or 64 bytes.
 *       How to do this?
 * TODO: Add register macros for XHCI_REG_RD/WR for
 *       this data structure.
 */
typedef struct xhci_input_ctrl_ctx {
	uint64_t data [8];
} __attribute__((packed)) xhci_input_ctrl_ctx_t;

/**
 * Input context: section 6.2.5
 */
typedef struct xhci_input_ctx {
	xhci_input_ctrl_ctx_t ctr_ctx;
	xhci_slot_ctx_t slot_ctx;
	xhci_ep_ctx_t endpoint_ctx [31];
} __attribute__((packed)) xhci_input_ctx_t;

#endif
