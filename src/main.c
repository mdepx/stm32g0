#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>

int
main(void)
{

	while (1) {
		printf("%s: hello world\n", __func__);
		mdx_usleep(1000000);
	}

	return (0);
}
