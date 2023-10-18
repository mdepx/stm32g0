#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>

#include <arm/stm/stm32g0.h>

extern struct stm32f4_gpio_softc gpio_sc;

int
main(void)
{

	/* Disable power. */
	pin_set(&gpio_sc, PORT_B, 0, 0); /* VCC_3v3 */
	pin_set(&gpio_sc, PORT_A, 0, 0); /* VCC_1v8 */
	pin_set(&gpio_sc, PORT_A, 1, 0); /* VCC_0v9 */
	pin_set(&gpio_sc, PORT_A, 12, 0); /* RESET */

	mdx_usleep(100000);

	/* Enable power. */
	pin_set(&gpio_sc, PORT_B, 0, 1); /* VCC_3v3 */
	pin_set(&gpio_sc, PORT_A, 0, 1); /* VCC_1v8 */
	mdx_usleep(2000);
	pin_set(&gpio_sc, PORT_A, 1, 1); /* VCC_0v9 */
	mdx_usleep(64000);
	pin_set(&gpio_sc, PORT_A, 12, 1); /* RESET */

	/* Sleep forever. */
	while (1)
		mdx_usleep(500000);

	return (0);
}
