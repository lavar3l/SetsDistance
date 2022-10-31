#include "../include/jaccardDistance.h"
#include <stdio.h>
#include <stdlib.h>

int ComputeFamilyDistance_Jaccard(struct Family* f1, struct Family* f2)
{
	int distance = 0;
	int size;
	int maxValue;
	struct JaccardFamily jf1, jf2;

	// Compute the size of the Jaccard family
	size = max(f1->size, f2->size);

	// Get the maximum value from the sets
	maxValue = max(GetMaxValueFromSets(f1), GetMaxValueFromSets(f2));

	// Convert the families to sets of numbers
	ConvertToJaccardFamily(f1, &jf1, size, maxValue);
	ConvertToJaccardFamily(f2, &jf2, size, maxValue);

	printf("Printing family 1.\n");
	for (int i = 0; i < jf1.size; i++)
	{
		printf("%s\n", jf1.sets[i]);
	}
	
	printf("Printing family 2.\n");
	for (int i = 0; i < jf2.size; i++)
	{
		printf("%s\n", jf2.sets[i]);
	}
	
	return 0;
} // ComputeFamilyDistance_Jaccard

int GetMaxValueFromSets(struct Family* f)
{
	int max = 0;

	for (int i = 0; i < f->size; i++)
	{
		for (int j = 0; j < f->sets[i].size; j++)
		{
			if (f->sets[i].elements[j] > max)
			{
				max = f->sets[i].elements[j];
			}
		}
	}

	return max;
} // GetMaxValueFromSets

void ConvertToJaccardFamily(struct Family* f, struct JaccardFamily* jf, int targetSize, int maxValue)
{
	// Allocate memory for the sets
	jf->size = targetSize;
	jf->sets = (char**)malloc(targetSize * sizeof(char*));

	// Allocate memory for the elements
	for (int i = 0; i < targetSize; i++)
	{
		jf->sets[i] = (char*)malloc((maxValue + 2) * sizeof(char));
	}

	// Covert the sets to Jaccard sets
	for (int i = 0; i < f->size; i++)
	{
		InitializeEmptyJaccardSet(jf->sets[i], maxValue);
		ConvertToJaccardSet(&f->sets[i], jf->sets[i], maxValue);
	}

	// Fill the remaining sets with 0
	for (int i = f->size; i < targetSize; i++)
	{
		InitializeEmptyJaccardSet(jf->sets[i], maxValue);
	}

	// Sort the sets
	SortJaccardSets(jf, targetSize, maxValue);
} // ConvertToJaccardFamily

void InitializeEmptyJaccardSet(char* js, int maxValue)
{
	// Initialize the set to 0
	for (int i = 0; i <= maxValue; i++)
	{
		js[i] = '0';
	}

	// Terminate the string
	js[maxValue + 1] = '\0';
	
} // InitializeEmptyJaccardSet

void ConvertToJaccardSet(struct Set* s, char* js, int maxValue)
{
	// Set appropriate elements' bits to 1
	for (int i = 0; i < s->size; i++)
	{
		js[maxValue - s->elements[i]] = '1';
	}
} // ConvertToJaccardSet

void SortJaccardSets(struct JaccardFamily* jf, int targetSize, int maxValue)
{
	// Sort the sets using QuickSort algorithm
	qsort(jf->sets, targetSize, sizeof(char*), CompareJaccardSets);
} // SortJaccardSets

int CompareJaccardSets(const void* a, const void* b)
{
	// Use standard string comparison
	return strcmp(*(char**)a, *(char**)b);
} // CompareJaccardSets