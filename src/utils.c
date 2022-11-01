#include "../include/utils.h"
#include "../include/defines.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void ExitError(char* message)
{
    trace(0, TRACE_ERROR, "[Error] %s", message);
    system("PAUSE");
    exit(-1);
} // ExitError

void trace(int level, TraceType type, const char* format, ...)
{
	if (level <= gTraceLevel)
	{
		// Parse args list
		va_list args;
		va_start(args, format);

		// Set appropriate output options
		switch (type)
		{
			case STD:
				printf("\x1b[0m");
				break;
			case TRACE_ERROR:
				printf("\x1b[31m");
				break;
			case TRACE_HEADER:
				printf("\x1b[36m");
				break;
			case TRACE_INTERNAL:
				printf("\x1b[32m");
				break;
		}
		
		// Output to console
		vprintf(format, args);

		if (type != STD)
		{
			// Reset color settings to default
			printf("\x1b[0m");
		}
		
		va_end(args);
	}
} // Trace