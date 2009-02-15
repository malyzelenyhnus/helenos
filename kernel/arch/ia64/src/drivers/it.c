/*
 * Copyright (c) 2005 Jakub Jermar
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

/** @addtogroup ia64	
 * @{
 */
/** @file
 */

/** Interval Timer driver. */
 
#include <arch/drivers/it.h>
#include <arch/interrupt.h>
#include <arch/register.h>
#include <arch/asm.h>
#include <arch/barrier.h>
#include <time/clock.h>
#include <ddi/irq.h>
#include <ddi/device.h>
#include <arch.h>

#define IT_SERVICE_CLOCKS	64

#define FREQ_NUMERATOR_SHIFT	32
#define FREQ_NUMERATOR_MASK	0xffffffff00000000ULL

#define FREQ_DENOMINATOR_SHIFT	0
#define FREQ_DENOMINATOR_MASK	0xffffffffULL

uint64_t it_delta;

static irq_t it_irq;

static irq_ownership_t it_claim(void *);
static void it_interrupt(irq_t *irq);

/** Initialize Interval Timer. */
void it_init(void)
{
	cr_itv_t itv;
	
	if (config.cpu_active == 1) {
		irq_initialize(&it_irq);
		it_irq.inr = INTERRUPT_TIMER;
		it_irq.devno = device_assign_devno();
		it_irq.claim = it_claim;
		it_irq.handler = it_interrupt;
		irq_register(&it_irq);
		
		uint64_t base_freq;
		base_freq  = ((bootinfo->freq_scale) & FREQ_NUMERATOR_MASK) >>
		    FREQ_NUMERATOR_SHIFT;
		base_freq *= bootinfo->sys_freq;
		base_freq /= ((bootinfo->freq_scale) & FREQ_DENOMINATOR_MASK) >>
		    FREQ_DENOMINATOR_SHIFT;
		
		it_delta = base_freq / HZ;
	}
	
	/* initialize Interval Timer external interrupt vector */
	itv.value = itv_read();
	itv.vector = INTERRUPT_TIMER;
	itv.m = 0;
	itv_write(itv.value);

	/* set Interval Timer Counter to zero */
	itc_write(0);
	
	/* generate first Interval Timer interrupt in IT_DELTA ticks */
	itm_write(IT_DELTA);

	/* propagate changes */
	srlz_d();
}

/** Always claim ownership for this IRQ.
 *
 * Other devices are responsible to avoid using INR 0.
 *
 * @return Always IRQ_ACCEPT.
 */
irq_ownership_t it_claim(void *instance)
{
	return IRQ_ACCEPT;
}

/** Process Interval Timer interrupt. */
void it_interrupt(irq_t *irq)
{
	int64_t c;
	int64_t m;
	
	eoi_write(EOI);
	
	m = itm_read();
	
	while (1) {
		c = itc_read();
		c += IT_SERVICE_CLOCKS;

		m += IT_DELTA;
		if (m - c < 0)
			CPU->missed_clock_ticks++;
		else
			break;
	}
	
	itm_write(m);
	srlz_d();				/* propagate changes */

	/*
	 * We are holding a lock which prevents preemption.
	 * Release the lock, call clock() and reacquire the lock again.
	 */
	spinlock_unlock(&irq->lock);	
	clock();
	spinlock_lock(&irq->lock);
}

/** @}
 */
