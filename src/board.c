/*-
 * Copyright (c) 2023 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/thread.h>
#include <sys/smp.h>

#include <machine/cpufunc.h>
#include <machine/cpuregs.h>
#include <machine/scs.h>

#include <dev/intc/intc.h>
#include <dev/uart/uart.h>

#include <arm/stm/stm32g0.h>
#include <arm/arm/nvic.h>

struct stm32g0_rcc_softc rcc_sc;
struct stm32l4_usart_softc usart_sc;
struct stm32f4_gpio_softc gpio_sc;
struct stm32f4_timer_softc timer_sc;

struct arm_nvic_softc nvic_sc;
struct mdx_device dev_nvic = { .sc = &nvic_sc };

void
udelay(uint32_t usec)
{
	int i;

	/* TODO: implement me */

	for (i = 0; i < usec * 100; i++)
		;
}

#if 0
void
usleep(uint32_t usec)
{

	mdx_usleep(usec);
}
#endif

static void
uart_putchar(int c, void *arg)
{
	struct stm32l4_usart_softc *sc;

	sc = arg;

	if (c == '\n')
		stm32l4_usart_putc(sc, '\r');

	stm32l4_usart_putc(sc, c);
}

static const struct gpio_pin uart_pins[] = {
	{ PORT_A, 2, MODE_ALT, 1, FLOAT }, /* USART2_TX */
	{ PORT_A, 3, MODE_ALT, 1, FLOAT }, /* USART2_RX */
	{ PORT_A, 0, MODE_OUT, 0, FLOAT }, /* VCC_1v8 */
	{ PORT_A, 1, MODE_OUT, 0, FLOAT }, /* VCC_0v9 */
	{ PORT_A, 12, MODE_OUT, 0, FLOAT }, /* RESET */
	{ PORT_B, 0, MODE_OUT, 0, FLOAT }, /* VCC_3v3 */
#if 0
	{ PORT_B, 6, MODE_ALT, 0, FLOAT }, /* USART1_TX */
	{ PORT_B, 7, MODE_ALT, 0, FLOAT }, /* USART1_RX */
#endif
	{ -1, -1, -1, -1, -1 }
};

void
board_init(void)
{
	struct rcc_config cfg;

	cfg.ahbenr = 0;
	cfg.apbenr1 = APBENR1_PWREN | APBENR1_RTCAPBEN | APBENR1_USART2EN;
	cfg.apbenr2 = APBENR2_USART1EN | APBENR2_TIM1EN;
	cfg.iopenr = IOPENR_GPIOAEN | IOPENR_GPIOBEN;

	stm32g0_rcc_init(&rcc_sc, RCC_BASE);
	stm32g0_rcc_setup(&rcc_sc, &cfg);

	stm32f4_gpio_init(&gpio_sc, GPIO_BASE);
	pin_configure(&gpio_sc, uart_pins);

	stm32l4_usart_init(&usart_sc, USART2_BASE, 16000000, 115200);
	mdx_console_register(uart_putchar, (void *)&usart_sc);

	stm32f4_timer_init(&timer_sc, TIM1_BASE, 16000000);
	arm_nvic_init(&dev_nvic, NVIC_BASE);
	mdx_intc_setup(&dev_nvic, 14, stm32f4_timer_intr, &timer_sc);
	mdx_intc_enable(&dev_nvic, 14);

#if 0
	malloc_init();
	malloc_add_region((void *)0x20020000, 0x20000);
#endif
}
