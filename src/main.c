#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>

#include <arm/stm/stm32g0.h>

extern struct stm32f4_gpio_softc gpio_sc;

static void
gpio_set(int port, int pin, int val)
{

	pin_set(&gpio_sc, port, pin, val);
}

int
main(void)
{

	/* Gate power rails. */
	gpio_set(PORT_B, 0, 0); /* VCC_3v3 */
	gpio_set(PORT_A, 0, 0); /* VCC_1v8 */
	gpio_set(PORT_A, 1, 0); /* VCC_0v9 */
	gpio_set(PORT_A, 12, 0); /* RESET */

	mdx_usleep(10000);

	/* Ungate power. */
	gpio_set(PORT_B, 0, 1); /* VCC_3v3 */
	gpio_set(PORT_A, 0, 1); /* VCC_1v8 */
	mdx_usleep(2000);
	gpio_set(PORT_A, 1, 1); /* VCC_0v9 */
	mdx_usleep(64000);
	gpio_set(PORT_A, 12, 1); /* RESET */

	/*
	 * Sleep forever. Note this disables JTAG clock.
	 * Use hardware reset in the OpenOCD config:
	 *   'reset_config srst_only connect_assert_srst'
	 */
	while (1)
		mdx_usleep(500000);

	return (0);
}
