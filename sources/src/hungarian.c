#include "../include/hungarian.h"

#include <stdlib.h>
#include <stdbool.h>

#define INF 1000000000 // unreachable (infinite) value

int gSize; // number of workers and assignments
int** gCost; // matrix with the cost of performing each assignment by each worker
int gMaxMatch; // number of vertices in the current matching

int* lx; // labels of X part
int* ly; // labels of Y part

int* matchX; // xy[x] - vertex matched with x,
int* matchY; // yx[y] - vertex matched with y

bool* S; // set S in the algorithm
bool* T; // set T in the algorithm

int* slack; // as in the algorithm description
int* slackX; // slackx[y] such a vertex, that l(slackX[y]) + l(y) - w(slackX[y], y) = slack[y]

int* prev; // alternating paths

int AssignmentProblemSolver(int** costMatrix, int size)
{
	int result;

	// Initialize the global variables
	gSize = size;
	gCost = costMatrix;

	// Initialize auxiliary structures
	lx = (int*)malloc(gSize * sizeof(int)); // labels of X part
	ly = (int*)malloc(gSize * sizeof(int)); // labels of Y part

	matchX = (int*)malloc(gSize * sizeof(int)); // xy[x] - vertex matched with x
	matchY = (int*)malloc(gSize * sizeof(int)); // yx[y] - vertex matched with y

	S = (bool*)malloc(gSize * sizeof(bool)); // set S in the algorithm
	T = (bool*)malloc(gSize * sizeof(bool)); // set T in the algorithm

	slack = (int*)malloc(gSize * sizeof(int)); // as in the algorithm description
	slackX = (int*)malloc(gSize * sizeof(int)); // slackx[y] such a vertex, that l(slackX[y]) + l(y) - w(slackX[y], y) = slack[y]

	prev = (int*)malloc(gSize * sizeof(int)); // Alternating paths

	if (lx == NULL || ly == NULL || matchX == NULL || matchY == NULL || S == NULL || T == NULL || slack == NULL || slackX == NULL || prev == NULL)
	{
		ExitError("Error allocating memory for Hungarian algorithm auxiliary structures!\n");
	}

	// Prepare matching and labels 
	InitializeMatching();
	InitializeLabels();

	// Prepare initial state
	gMaxMatch = 0;
	FirstMatching();

	// Realize the Hungarian algorithm
	result = ComputeAssignmentCost();

	// Free memory	
	free(lx);
	free(ly);

	free(matchX);
	free(matchY);

	free(S);
	free(T);

	free(slack);
	free(slackX);

	free(prev);

	// Return result
	return result;
} // AssignmentProblemSolver

int ComputeAssignmentCost(void)
{
	int* q;
	int start = 0, end = 0;
	int root, x = 0, y = 0;
	int result;

	// Initialize queue for BFS traversal
	q = (int*)malloc(gSize * sizeof(int));
	if (q == NULL)
	{
		ExitError("Error allocating memory for BFS queue!\n");
	}

	// Iterate until all tasks are assigned to workers
	while (gMaxMatch != gSize)
	{
		// Clear queue
		start = 0;
		end = 0;
		
		// Initialize sets
		InitializeSets();

		// Find tree root
		root = 0;
		for (x = 0; x < gSize; x++)
		{
			if (matchX[x] == -1)
			{
				q[end++] = x;
				root = x;
				prev[x] = -2;
				S[x] = true;

				break;
			}
		}
		
		// Initialize slack array
		for (int y = 0; y < gSize; y++)
		{
			slack[y] = gCost[root][y] - lx[root] - ly[y];
			slackX[y] = root;
		}
		
		// Main iteration cycle
		while (true)
		{
			// Build tree with BFS traversal
			while (start < end)
			{
				// Retrieve next vertex from queue
				x = q[start++];
				
				// Iterate through all edges in graph
				for (y = 0; y < gSize; y++)
				{
					if (gCost[x][y] == lx[x] + ly[y] && T[y] == false)
					{
						if (matchY[y] == -1)
						{
							// Found exposed vertex in Y part, as a result
							// augmenting path is found.
							break;
						}
						
						// In other case, add the vertex to queue
						T[y] = true;
						q[end++] = matchY[y];

						// Add edge between (x, y) and (y, yx[y]) to the tree
						AddToTree(matchY[y], x);
					}

				}
				
				if (y < gSize)
				{
					// Found augmenting path
					break;
				}
			}
			
			if (y < gSize)
			{
				// Found augmenting path
				break;
			}
			
			// If there is no augmenting path, then improve the labeling
			UpdateLabels();

			for (y = 0; y < gSize; y++)
			{
				if (T[y] == false && slack[y] == 0)
				{
					if (matchY[y] == -1)
					{
						// Found exposed vertex in Y part, as a result
						// augmenting path is found.
						x = slackX[y];
						break;
					}
					else
					{
						// In other case, add the vertex to queue
						T[y] = true;
						if (S[matchY[y]] == false)
						{
							// Add vertex to queue
							q[end++] = matchY[y];

							// Add edge between (x, y) and (y, yx[y]) to the tree
							AddToTree(matchY[y], slackX[y]);
						}
					}
				}
			}
			
			if (y < gSize)
			{
				// Found augmenting path
				break;
			}
		}

		// Increment matching
		gMaxMatch++;

		// Inverse edges along the augmentating path
		for (int newX = x, newY = y, tempY; newX != -2; newX = prev[newX], newY = tempY)
		{
			tempY = matchX[newX];
			matchY[newY] = newX;
			matchX[newX] = newY;
		}
	}

	// Free memory
	free(q);

	// Compute the optimal cost of the assignment based on the generated matching
	result = 0;
	trace(2, TRACE_INTERNAL, "Assignment based on Hungarian algorithm:\n");
	trace(2, STD, "\tFamily 1\tFamily 2\tEdit distance\n");
	for (int i = 0; i < gSize; i++)
	{
		trace(2, STD, "\t%d\t\t%d\t\t%d\n", i, matchX[i], gCost[i][matchX[i]]);
		result += gCost[i][matchX[i]];
	}
	trace(2, STD, "\n\n");

	return result;
} // ComputeAssignmentCost

void InitializeMatching(void)
{
	for (int i = 0; i < gSize; i++)
	{
		matchX[i] = -1;
		matchY[i] = -1;
	}
} // InitializeMatching

void FirstMatching(void)
{
	for (int x = 0; x < gSize; x++)
	{
		for (int y = 0; y < gSize; y++)
		{
			if (gCost[x][y] != lx[x] + ly[y] || matchY[y] != -1) continue;
			matchX[x] = y;
			matchY[y] = x;
			gMaxMatch++;
			break;
		}
	}
} // FirstMatching

void InitializeSets(void)
{
	for (int i = 0; i < gSize; i++)
	{
		S[i] = false;
		T[i] = false;
	}
} // InitializeSets

void InitializeLabels(void)
{
	for (int x = 0; x < gSize; x++)
	{
		int minRowCost = gCost[x][0];
		for (int y = 0; y < gSize; y++)
		{
			minRowCost = min(minRowCost, gCost[x][y]);
			if (minRowCost == 0)
			{
				break;
			}
		}
		lx[x] = minRowCost;
	}

	for (int y = 0; y < gSize; y++)
	{
		int minColCost = gCost[0][y] - lx[0];
		for (int x = 0; x < gSize; x++)
		{
			minColCost = min(minColCost, gCost[x][y] - lx[x]);
			if (minColCost == 0)
			{
				break;
			}
		}
		ly[y] = minColCost;
	}
} // InitializeLabels

void UpdateLabels(void)
{
	int delta = INF;

	// Calulate delta based on slack
	for (int y = 0; y < gSize; y++)
	{
		if (T[y] == false)
		{
			delta = min(delta, slack[y]);
		}
	}

	// Update X labels
	for (int x = 0; x < gSize; x++)
	{
		if (S[x] == true)
		{
			lx[x] += delta;
		}
	}

	// Update Y labels
	for (int y = 0; y < gSize; y++)
	{
		if (T[y] == true)
		{
			ly[y] -= delta;
		}
	}

	// Update slack array
	for (int y = 0; y < gSize; y++)
	{
		if (T[y] == false)
		{
			slack[y] -= delta;
		}
	}
} // UpdateLabels

void AddToTree(int currentVertex, int prevVertex)
{
	// Add current vertex to S set
	S[currentVertex] = true;

	// Update ancestor of current vertex in alternating path
	prev[currentVertex] = prevVertex;

	// Update slack on modification of S set
	for (int y = 0; y < gSize; y++)
	{
		if (gCost[currentVertex][y] - lx[currentVertex] - ly[y] < slack[y])
		{
			slack[y] = gCost[currentVertex][y] - lx[currentVertex] - ly[y];
			slackX[y] = currentVertex;
		}
	}
} // AddToTree