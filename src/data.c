#include "../include/data.h"

void PrintFamily(struct Family* f)
{
	printf("Family with %d sets:\n", f->size);
	for (int i = 0; i < f->size; i++)
	{
		PrintSet(&f->sets[i]);
	}
}

void PrintSet(struct Set* s)
{
	printf("\tSet with %d elements:\n", s->size);
	printf("\t\t");
	for (int i = 0; i < s->size; i++)
	{
		printf("%d ", s->elements[i]);
	}
	printf("\n");
}

void DeleteFamily(struct Family* f)
{
	// Delete sets data
	for (int i = 0; i < f->size; i++)
	{
		DeleteSet(&f->sets[i]);
	}

	// Delete sets array
	free(f->sets);
}

void DeleteSet(struct Set* s)
{
	// Delete elements array
	free(s->elements);
}