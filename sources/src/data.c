#include "../include/data.h"
#include "../include/utils.h"

#include <stdlib.h>


void PrintFamily(struct Family* f)
{
	trace(1, STD, "Family with %d sets:\n", f->size);
	for (int i = 0; i < f->size; i++)
	{
		PrintSet(&f->sets[i]);
	}
	trace(1, STD, "\n");
} // PrintFamily

void PrintSet(struct Set* s)
{
	trace(1, STD, "\tSet with %d elements:\n", s->size);
	trace(1, STD, "\t\t");
	for (int i = 0; i < s->size; i++)
	{
		trace(1, STD, "%d ", s->elements[i]);
	}
	trace(1, STD, "\n");
} // PrintSet

void DeleteFamily(struct Family* f)
{
	// Delete sets data
	for (int i = 0; i < f->size; i++)
	{
		DeleteSet(&f->sets[i]);
	}

	// Delete sets array
	free(f->sets);
} // DeleteFamily

void DeleteSet(struct Set* s)
{
	// Delete elements array
	free(s->elements);
} // DeleteSet