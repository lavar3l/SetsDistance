#include <stdio.h>
#include <stdlib.h>

#include "include/defines.h"
#include "include/utils.h"
#include "include/data.h"

#include "include/editDistance.h"
#include "include/jaccardDistance.h"

FILE* inputFile;
int gTraceLevel = 0;

int main()
{
    char filename[MAX_FILE_NAME];
	struct Family f1, f2;
    int distance1 = 0;
    double distance2 = 0;

    // Read the file name from the user
    printf("Enter the name of the file with the set data: ");
    scanf_s("%s", filename, (unsigned)_countof(filename));
    printf("%s\n", filename);

    // Specify debug level
	printf("Enter the debug level (0-3): ");
    scanf("%d", &gTraceLevel);

    // Parse data from file
	ReadFile(filename, &f1, &f2);
    printf("Input file successfully parsed.\n");

	// Debug print
    PrintFamily(&f1);
	PrintFamily(&f2);

	// Compute the distance between families
    distance1 = ComputeFamilyDistance_Edit(&f1, &f2);
    distance2 = ComputeFamilyDistance_Jaccard(&f1, &f2);
    printf("Distance using metric 1. (distance based on Levenshtein distance and Hungarian assignment problem): %d\n", distance1);
    printf("Distance using metric 2. (distance based on Jaccard metric and set conversion): %f\n", distance2);

	// Free memory
    DeleteFamily(&f1);
    DeleteFamily(&f2);

    return 0;
}
