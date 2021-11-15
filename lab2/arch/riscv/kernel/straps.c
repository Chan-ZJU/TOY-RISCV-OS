#include "put.h"

static int count = 0;
static int count0 = 0;
int strap_c()
{
	count0++;
	if(count0 >= 100)
	{
		count0 = 0;
		puts("[S] Supervisor Mode Timer Interrupt ");
		puti(count);
		count++;
		puts("\n");
	}
	return 0;
}
