#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "udpserver.h"


int main(void)
{

	udpserver_Init();

	while(1)
	{
		sleep(1);
	}

	return 0;
}


