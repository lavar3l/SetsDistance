#include "../include/utils.h"
#include "../include/defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void ExitError(char* message)
{
    printf("[Error] %s", message);
    system("PAUSE");
    exit(-1);
} // ExitError

void trace(int level, const char* format, ...)
{
	if (level <= gTraceLevel)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
} // Trace