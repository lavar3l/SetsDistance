#ifndef UTILS_H
#define UTILS_H

#include "data.h"

typedef enum TraceType_t
{
	STD, // Standard output
	TRACE_ERROR, // Error output
	TRACE_HEADER, // Header output
	TRACE_INTERNAL // Internal result output
} TraceType;

void ExitError(char* message);

void trace(int level, TraceType type, const char* format, ...);

#endif //UTILS_H
