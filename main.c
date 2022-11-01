#include <stdio.h>
#include <stdlib.h>

#include "include/defines.h"
#include "include/utils.h"
#include "include/data.h"

#include "include/editDistance.h"
#include "include/jaccardDistance.h"

#include "lib/argparse.h"

FILE* inputFile;
int gTraceLevel = 0;

static const char* const usage[] = 
{
    "SetsDitance.exe --path [path to family data]",
    "SetsDitance.exe --path [path to family data] --verbose [0-3 verbose level]",
    NULL,
};

int main(int argc, const char **argv)
{
    const char* filename = NULL;
	struct Family f1, f2;
    int distance1 = 0;
    double distance2 = 0;

    // ---------------- Parse command line arguments ----------------
    // Define available command line options
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Essential options"),
        OPT_STRING('p', "path", &filename, "Path to the file with the family data"),
		OPT_GROUP("Additional options"),
        OPT_INTEGER('v', "verbose", &gTraceLevel, "Verbose level"),
		OPT_END()
	};

    // Parse command line arguments
	struct argparse argparse;
	argparse_init(&argparse, options, usage, 0);
    argparse_describe(&argparse, MAIN_DESCRIPTION, NULL);
    argc = argparse_parse(&argparse, argc, argv);

	// Set default data filename if none was provided
    if (filename == NULL)
    {
        filename = FALLBACK_FILE_NAME;
    }

    // Print parsed arguments
    trace(0, STD, MAIN_DESCRIPTION);
    trace(0, STD, EXTRA_DESCRIPTION);
	
    trace(1, TRACE_HEADER, "Parsed command line arguments:\n");
	trace(1, STD, "* Data file path: %s\n", filename);
	trace(1, STD, "* Verbose level: %d\n\n", gTraceLevel);
	
	
    // -------------------- Parse data from file --------------------
    // Parse data from file and initialize families
	ReadFile(filename, &f1, &f2);

	// Print parsed family data
    trace(1, TRACE_HEADER, "Parsed family data:\n");
    PrintFamily(&f1);
	PrintFamily(&f2);

	
    // ------------ Compute the distance between families -----------
    distance1 = ComputeFamilyDistance_Edit(&f1, &f2);
    distance2 = ComputeFamilyDistance_Jaccard(&f1, &f2);
    trace(0, TRACE_HEADER, "Final results:\n");
    trace(0, STD, "Distance using metric 1. (distance based on Levenshtein distance and Hungarian assignment problem): %d\n", distance1);
    trace(0, STD, "Distance using metric 2. (distance based on Jaccard metric and set conversion): %f\n", distance2);


	// -------------------- Free allocated memory --------------------
    DeleteFamily(&f1);
    DeleteFamily(&f2);

    return 0;
} // main
