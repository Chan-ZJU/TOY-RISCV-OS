#include "test.h"
#define SIFIVE_TEST 0x100000
#define VIRT_TEST_FINISHER_PASS 0x5555

int start_kernel()
{
	os_test();
	task_init();
	dead_loop();
	asm volatile("sh %0, 0(%1)" : : "r" (VIRT_TEST_FINISHER_PASS), "r" (SIFIVE_TEST));

	return 0;
}
