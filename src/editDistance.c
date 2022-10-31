#include "../include/editDistance.h"
#include <stdio.h>
#include <stdlib.h>

int ComputeFamilyDistance_Edit(struct Family* f1, struct Family* f2)
{
	int size;
	int distance;
	int** editDistance;

	// Compute the size of the families
	size = max(f1->size, f2->size);

	// Initialize the edit distance matrix
	editDistance = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++)
	{
		editDistance[i] = (int*)malloc(size * sizeof(int));
	}

	// Compute editorial distance between sets
	ComputeDistanceMatrix(f1, f2, editDistance, size);
	PrintEditDistanceMatrix(editDistance, size);

	// Compute editorial distance between families
	distance = ComputeEditDistanceFamilies(editDistance, size);

	// Free memory
	for (int i = 0; i < size; i++)
	{
		free(editDistance[i]);
	}
	free(editDistance);

	return distance;
} // ComputeFamilyDistance_Edit

void ComputeDistanceMatrix(struct Family* f1, struct Family* f2, int** distance, int size)
{	
	// Compute editorial distance for all pairs of sets from F1 and F2
	// families. If one family has more sets than the other, the remaining
	// sets from the larger family are paired with empty sets.
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i < f1->size && j < f2->size)
			{
				// Compute the editorial distance between the sets
				distance[i][j] = ComputeEditDistanceSets(&f1->sets[i], &f2->sets[j]);
			}
			else if (i < f1->size)
			{
				// Editorial distance between a set and an empty set 
				// is equal to the size of the nonempty set
				distance[i][j] = f1->sets[i].size;
			}
			else
			{
				// Editorial distance between a set and an empty set 
				// is equal to the size of the nonempty set
				distance[i][j] = f2->sets[j].size;
			}
		}
	}
} // ComputeDistanceMatrix

int ComputeEditDistanceSets(struct Set* s1, struct Set* s2)
{
	int** distance;
	int editDistance;
	int elementSubstitution;

	// Initialize the editorial distance matrix
	distance = (int**)malloc((s2->size + 1) * sizeof(int*));
	for (int i = 0; i < s2->size + 1; i++)
	{
		distance[i] = (int*)malloc((s1->size + 1 * sizeof(int)));
	}

	// Initialize the edge cases
	distance[0][0] = 0;
	
	for (int i = 1; i <= s2->size; i++)
	{
		distance[i][0] = distance[i - 1][0] + 1;
	}
	
	for (int j = 1; j <= s1->size; j++)
	{
		distance[0][j] = distance[0][j - 1] + 1;
	}

	// Compute the Levenshtein editorial distance
	for (int i = 1; i <= s2->size; i++)
	{
		for (int j = 1; j <= s1->size; j++)
		{
			elementSubstitution = 0;
			if (s1->elements[j - 1] != s2->elements[i - 1])
			{
				elementSubstitution = 1;
			}
			
			distance[i][j] = min(min(distance[i - 1][j] + 1, // Deletion of the element
								     distance[i][j - 1] + 1), // Insertion of the element
								     distance[i - 1][j - 1] + elementSubstitution); // Substitution of the element
		}
	}
	editDistance = distance[s2->size][s1->size];

	// Free memory
	free(distance);

	return editDistance;
	
} // ComputeEditDistance

int ComputeEditDistanceFamilies(int** matrix, int size)
{
	return 0;
}

void PrintEditDistanceMatrix(int** matrix, int size)
{
	printf("Edit distance matrix:\n");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
} // PrintEditDistanceMatrix