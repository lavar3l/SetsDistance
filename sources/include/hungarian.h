#ifndef HUNGARIAN_H
#define HUNGARIAN_H

#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Implementation of the Hungarian algorithm for the assignment problem.
// O(n^3) algorithm based on the article: https://www.topcoder.com/thrive/articles/Assignment%20Problem%20and%20Hungarian%20Algorithm

int  AssignmentProblemSolver(int** costMatrix, int size);

int  ComputeAssignmentCost(void);

void InitializeMatching(void);
void FirstMatching(void);

void InitializeSets(void);

void InitializeLabels(void);
void UpdateLabels(void);

void AddToTree(int currentVertex, int prevVertex);

#endif // HUNGARIAN_H