#include <sys/cdefs.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/spinlock.h>

int
main(void)
{

	printf("%s: hello world\n", __func__);

	return (0);
}
