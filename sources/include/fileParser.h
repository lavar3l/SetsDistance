#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "data.h"

#include <stdio.h>
#include <stdlib.h>

void ReadFile(char* filename, struct Family* f1, struct Family* f2);
void ReadFamily(struct Family* f, FILE* inputFile);
void ReadSet(struct Set* s, FILE* inputFile);

#endif // FILE_PARSER_H