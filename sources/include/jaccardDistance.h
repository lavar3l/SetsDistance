#ifndef JACCARD_DISTANCE_H
#define JACCARD_DISTANCE_H

#include "data.h"

struct JaccardFamily
{
	int size;
	char** sets;
};

double ComputeFamilyDistance_Jaccard(struct Family* f1, struct Family* f2);
int    GetMaxValueFromSets(struct Family* f);

void   ConvertToJaccardFamily(struct Family* f, struct JaccardFamily* jf, int maxValue);
void   InitializeEmptyJaccardSet(char* js, int maxValue);
void   ConvertToJaccardSet(struct Set* s, char* js, int maxValue);

void   SortJaccardSets(struct JaccardFamily* jf, int maxValue);
int    CompareJaccardSets(const void* a, const void* b);

int    UnionJaccardFamilies(struct JaccardFamily* jf1, struct JaccardFamily* jf2);
int    IntersectionJaccardFamilies(struct JaccardFamily* jf1, struct JaccardFamily* jf2);

void   PrintJaccardFamily(struct JaccardFamily* jf, int no);


#endif // JACCARD_DISTANCE_H