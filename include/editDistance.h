#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

#include "data.h"

int  ComputeFamilyDistance_Edit(struct Family* f1, struct Family* f2);
void ComputeDistanceMatrix(struct Family* f1, struct Family* f2, int** distance, int size);
int  ComputeEditDistance(struct Set* s1, struct Set* s2);

void PrintEditDistanceMatrix(int** matrix, int size);

#endif // EDIT_DISTANCE_H