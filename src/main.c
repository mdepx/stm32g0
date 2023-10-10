#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>

#include <arm/stm/stm32g0.h>

extern struct stm32f4_gpio_softc gpio_sc;

int
main(void)
{
	int flag;

	flag = 0;

	while (1) {
		printf("%s: hello world\n", __func__);
		mdx_usleep(500000);
		flag = flag ? 0 : 1;
		pin_set(&gpio_sc, PORT_B, 0, flag);
	}

	return (0);
}
