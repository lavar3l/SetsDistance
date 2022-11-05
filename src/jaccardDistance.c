#include "../include/jaccardDistance.h"
#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>

double ComputeFamilyDistance_Jaccard(struct Family* f1, struct Family* f2)
{
	struct JaccardFamily jf1, jf2;
	int size, maxValue;
	int sum, intersection;
	double distance;

	// Compute the size of the Jaccard family
	size = max(f1->size, f2->size);

	// Get the maximum value from the sets
	maxValue = max(GetMaxValueFromSets(f1), GetMaxValueFromSets(f2));

	// Convert the families to sets of numbers
	ConvertToJaccardFamily(f1, &jf1, size, maxValue);
	ConvertToJaccardFamily(f2, &jf2, size, maxValue);

	// Print converted families
	trace(2, TRACE_INTERNAL, "Families converted to sets of binary numbers:\n");
	PrintJaccardFamily(&jf1, 1);
	PrintJaccardFamily(&jf2, 2);

	// Calculate Jaccard distance between families
	sum = UnionJaccardFamilies(&jf1, &jf2);
	intersection = IntersectionJaccardFamilies(&jf1, &jf2);

	if (sum > 0)
	{
		distance = ((double)sum - (double)intersection) / (double)sum;
	}
	else
	{
		// If sum is equal to 0, then both families are empty.
		// In this case, the distance is 0, because the families are identical.
		distance = 0;
	}

	trace(2, TRACE_INTERNAL, "Jaccard distance internal results:\n");
	trace(2, STD, "\t Union of families: %d\n", sum);
	trace(2, STD, "\t Intersection of families: %d\n", intersection);
	trace(2, STD, "\t Jaccard distance: %f\n\n", distance);
	
	return distance;
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
	if (jf->sets == NULL)
	{
		ExitError("Error allocating memory for Jaccard family sets!\n");
	}

	// Allocate memory for the elements
	for (int i = 0; i < targetSize; i++)
	{
		jf->sets[i] = (char*)malloc((maxValue + 2) * sizeof(char));
		if (jf->sets[i] == NULL)
		{
			ExitError("Error allocating memory for Jaccard family sets!\n");
		}
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

int UnionJaccardFamilies(struct JaccardFamily* jf1, struct JaccardFamily* jf2)
{
	int count = 0;
	int i = 0, j = 0;
	int cmp;

	while (i < jf1->size && j < jf2->size)
	{
		cmp = strcmp(jf1->sets[i], jf2->sets[j]);

		if (cmp < 0)
		{
			count++;
			i++;
		}
		else if (cmp > 0)
		{
			count++;
			j++;
		}
		else
		{
			count++;
			i++;
			j++;
		}
	}

	return count;
} // UnionJaccardFamilies

int IntersectionJaccardFamilies(struct JaccardFamily* jf1, struct JaccardFamily* jf2)
{
	int count = 0;
	int i = 0, j = 0;
	int cmp;
	
	while (i < jf1->size && j < jf2->size)
	{
		cmp = strcmp(jf1->sets[i], jf2->sets[j]);
		
		if (cmp < 0) i++;
		else if (cmp > 0) j++;
		else
		{
			count++;
			i++;
			j++;
		}
	}
	
	return count;
} // IntersectionJaccardFamilies

void PrintJaccardFamily(struct JaccardFamily* jf, int no)
{
	trace(2, STD, "Converted family %d:\n", no);
	for (int i = 0; i < jf->size; i++)
	{
		trace(2, STD, "Set %d: %s\n", i + 1, jf->sets[i]);
	}
	trace(2, STD, "\n");
}