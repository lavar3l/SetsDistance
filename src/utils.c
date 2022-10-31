#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>

void ExitError(char* message)
{
    printf("[Error] %s", message);
    system("PAUSE");
    exit(-1);
} // ExitError

int Min3(int a, int b, int c)
{
	if(a <= b && a <= c)
	{
		return a;
	}
	else if (b <= a && b <= c)
	{
		return b;
	}
	else
	{
		return c;
	}
} // Min3