#include "../include/fileParser.h"

void ReadFile(char* filename, struct Family* f1, struct Family* f2)
{
	FILE* inputFile;
	errno_t err_input;
	
	// Open the file
	err_input = fopen_s(&inputFile, filename, "r");
	if (err_input != 0)
	{
		ExitError("Cannot open the input file.\n");
	}

	// Read first and second family
	ReadFamily(f1, inputFile);
	ReadFamily(f2, inputFile);

	// Close the file
	fclose(inputFile);
} // ReadFile

void ReadFamily(struct Family* f, FILE* inputFile)
{
	// Read number of sets
	fscanf_s(inputFile, "%d", &f->size);

	// Allocate memory for the sets
	f->sets = (struct Set*)malloc(f->size * sizeof(struct Set));
	if (f->sets == NULL)
	{
		ExitError("Error allocating memory for family sets!\n");
	}

	// Read the sets
	for (int i = 0; i < f->size; i++)
	{
		ReadSet(&f->sets[i], inputFile);
	}
} // ReadFamily

void ReadSet(struct Set* s, FILE* inputFile)
{
	// Read number of elements
	fscanf_s(inputFile, "%d", &s->size);

	// Allocate memory for the elements
	s->elements = (int*)malloc(s->size * sizeof(int));
	if (s->elements == NULL)
	{
		ExitError("Error allocating memory for family set elements!\n");
	}

	// Read the elements
	for (int i = 0; i < s->size; i++)
	{
		fscanf_s(inputFile, "%d", &s->elements[i]);
	}
} // ReadSet