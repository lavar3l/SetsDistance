#include "../include/hungarian.h"

#define INF 1000000000 // unreachable (inifite) value

int gSize; // number of workers and assignments
int** gCost; // matrix with the cost of performing each assignment by each worker
int gMaxMatch; // number of vertices in the current matching

int* lx; // labels of X part
int* ly; // labels of Y part

int* xy; // xy[x] - vertex matched with x,
int* yx; // yx[y] - vertex matched with y

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

	// Reverse values in order to transform minimum-weighted matching problem
	// into maximum-weighted matching problem
	ReverseValues();

	// Initalize auxiliary structures
	lx = (int*)malloc(gSize * sizeof(int)); // labels of X part
	ly = (int*)malloc(gSize * sizeof(int)); // labels of Y part
	
	xy = (int*)malloc(gSize * sizeof(int)); // xy[x] - vertex matched with x
	yx = (int*)malloc(gSize * sizeof(int)); // yx[y] - vertex matched with y

	S = (bool*)malloc(gSize * sizeof(bool)); // set S in the algorithm
	T = (bool*)malloc(gSize * sizeof(bool)); // set T in the algorithm

	slack = (int*)malloc(gSize * sizeof(int)); // as in the algorithm description
	slackX = (int*)malloc(gSize * sizeof(int)); // slackx[y] such a vertex, that l(slackX[y]) + l(y) - w(slackX[y], y) = slack[y]

	prev = (int*)malloc(gSize * sizeof(int)); // Alternating paths

	// Prepare initial state
	result = 0; // optimal cost of the assignment
	gMaxMatch = 0; // number of vertices in the current matching

	// Initialize labels
	memset(xy, -1, sizeof(xy));
	memset(yx, -1, sizeof(xy));
	InitializeLabels();

	// Realize the Hungarian algorith
	Iterate();

	// Compute the optimal cost of the assignment
	for (int x = 0; x < gSize; x++)
	{
		result += gCost[x][xy[x]];
	}

	// Free memory	
	free(lx);
	free(ly);
	
	free(xy);
	free(yx);
	
	free(S);
	free(T);
	
	free(slack);
	free(slackX);
	
	free(prev);

	// Reverse back the values
	ReverseValues();

	// Return result (multiplied by -1 to get answer for minimum cost problem)
	return -result;
} // AssignmentProblemSolver

void Iterate()
{
	int* q;
	int write, read;
	int root;

	// Check whether all tasks are assigned to workers
	if (gMaxMatch == gSize)
	{
		return;
	}

	// Initialize queue for BFS traversal
	q = (int*)malloc(gSize * sizeof(int));
	write = 0;
	read = 0;

	// Initialize sets and alternating path
	memset(S, false, sizeof(S));
	memset(T, false, sizeof(T));
	memset(prev, -1, sizeof(prev));

	// Find tree root
	for (int x = 0; x < gSize; x++)
	{
		if (xy[x] == -1)
		{
			q[write++] = root = x;
			prev[x] = -2;
			S[x] = true;
			break;
		}
	}

	// Initialize slack array
	for (int y = 0; y < gSize; y++)
	{
		slack[y] = lx[root] + ly[y] - gCost[root][y];
		slackX[y] = root;
	}

	// Main interation cycle
	int y = 0;
	int x = 0;
	while (true)
	{
		// Build tree with BFS traversal
		while (read < write)
		{
			// Retrieve next vertex from queue
			x = q[read++];

			// Iterate through all edges in graph
			for (y = 0; y < gSize; y++)
			{
				if (gCost[x][y] == lx[x] + ly[y] && T[y] == false)
				{
					if (yx[y] == -1)
					{
						// Found exposed vertex in Y part, as a result
						// augmenting path is found.
						break;
					}

					// In other case, add the vertex to queue
					T[y] = true;
					q[write++] = yx[y];
					
					// Add edge between (x, y) and (y, yx[y]) to the tree
					AddToTree(yx[y], x);
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
		
		// Clear queue
		write = 0;
		read = 0;

		//
		for (y = 0; y < gSize; y++)
		{
			if (T[y] == false && slack[y] == 0)
			{
				if (yx[y] == -1)
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
					if (S[yx[y]] == false)
					{
						// Add vertex to queue
						q[write++] = yx[y];
						
						// Add edge between (x, y) and (y, yx[y]) to the tree
						AddToTree(yx[y], slackX[y]);
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

	// Check if augmenting path is found
	if (y < gSize)
	{
		// Increment matching
		gMaxMatch++;
		
		// Inverse edges along the augmentating path
		for (int nX = x, nY = y, tempY; nX != -2; nX = prev[nX], nY = tempY)
		{
			tempY = xy[nX];
			yx[nY] = nX;
			xy[nX] = nY;
		}

		// Reiterate the algorithm
		Iterate();
	}
	
	// Free memory
	free(q);
}

void InitializeLabels()
{
	memset(lx, 0, sizeof(lx));
	memset(ly, 0, sizeof(ly));
	for (int x = 0; x < gSize; x++)
	{
		for (int y = 0; y < gSize; y++)
		{
			lx[x] = max(lx[x], gCost[x][y]);
		}
	}
}

void UpdateLabels()
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
			lx[x] -= delta;
		}
	}

	// Update Y labels
	for (int y = 0; y < gSize; y++)
	{
		if (T[y] == true)
		{
			ly[y] += delta;
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
}

void AddToTree(int currentVertex, int prevVertex)
{
	// Add current vertex to S set
	S[currentVertex] = true;

	// Update ancestor of current vertex in alternating path
	prev[currentVertex] = prevVertex;

	// Update slack on modification of S set
	for (int y = 0; y < gSize; y++)
	{
		if (lx[currentVertex] + ly[y] - gCost[currentVertex][y] < slack[y])
		{
			slack[y] = lx[currentVertex] + ly[y] - gCost[currentVertex][y];
			slackX[y] = currentVertex;
		}
	}
}

void ReverseValues()
{
	for (int i = 0; i < gSize; i++)
	{
		for (int j = 0; j < gSize; j++)
		{
			gCost[i][j] = -gCost[i][j];
		}
	}
}