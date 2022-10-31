#include <stdio.h>
#include <stdlib.h>

#include "include/defines.h"
#include "include/utils.h"
#include "include/data.h"

#include "include/editDistance.h"
#include "include/jaccardDistance.h"

FILE* inputFile;

int main()
{
    char filename[MAX_FILE_NAME];
	struct Family f1, f2;
    double distance1 = 0, distance2 = 0;

    // Read the file name from the user
    printf("Enter the name of the file with the set data: ");
    scanf_s("%s", filename, (unsigned)_countof(filename));
    printf("%s\n", filename);

    // Parse data from file
	ReadFile(filename, &f1, &f2);
    printf("Input file successfully parsed.\n");

	// Debug print
    PrintFamily(&f1);
	PrintFamily(&f2);

	// Compute the distance between families
    distance1 = ComputeFamilyDistance_Edit(&f1, &f2);
    //distance2 = ComputeFamilyDistance_Jaccard(&f1, &f2);


	// Free memory
    DeleteFamily(&f1);
    DeleteFamily(&f2);

    return 0;
}
