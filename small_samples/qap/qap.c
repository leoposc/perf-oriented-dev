
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define bool int
#define true 1
#define false 0

// A quadratic matrix, dynamically sized
typedef struct _qmatrix
{
	int size;
	int data[];
} qmatrix;

qmatrix *qm_create(int size)
{
	qmatrix *res = (qmatrix *)malloc(sizeof(qmatrix) + size * size * sizeof(int));
	res->size = size;
	return res;
}

void qm_del(qmatrix *matrix)
{
	free(matrix);
}

#define get(M, I, J) M->data[(I) * (M)->size + (J)]

// a struct describing a QAP instance
typedef struct _problem
{
	int size;	 // the size of the problem
	qmatrix *A;	 // the weight matrix (size x size)
	qmatrix *B;	 // the distance matrix (size x size)
	int optimum; // the value of the optimal solution
} problem;

problem *qap_load(char *file);

void qap_del(problem *problem)
{
	qm_del(problem->A);
	qm_del(problem->B);
	free(problem);
}

#define getA(P, I, J) get(P->A, I, J)
#define getB(P, I, J) get(P->B, I, J)

// a struct representing a (partial) solution to the problem
typedef struct _solution
{
	struct _solution *head; // the solution is forming a linked list
	int pos;				// the location the current facility is assigned to
} solution;

solution *empty(void) { return 0; }

void print(solution *solution)
{
	if (!solution)
		return;
	print(solution->head);
	printf("-%d", solution->pos);
}

int solve_rec(problem *problem, solution *partial, int plant, int used_mask, int cur_cost, int best_known)
{
	// terminal case
	if (plant >= problem->size)
	{
		return cur_cost;
	}

	if (cur_cost >= best_known)
	{
		return best_known;
	}

	// fix current position
	int best = best_known;
	for (int i = 0; i < problem->size; i++)
	{
		// check whether current spot is a free spot
		if (!(1 << i & used_mask))
		{
			// extend solution
			solution tmp = {partial, i};

			// compute additional cost of current assignment
			int new_cost = 0;

			int cur_plant = plant;
			solution *cur = &tmp;
			while (cur)
			{
				int other_pos = cur->pos;

				// add costs between current pair of plants
				new_cost += getA(problem, plant, cur_plant) * getB(problem, i, other_pos);
				new_cost += getA(problem, cur_plant, plant) * getB(problem, other_pos, i);

				// go to next plant
				cur = cur->head;
				cur_plant--;
			}

			// compute recursive rest
			int cur_best = solve_rec(problem, &tmp, plant + 1, used_mask | (1 << i), cur_cost + new_cost, best);
			if (cur_best < best)
				best = cur_best;
		}
	}

	return best;
}

int solve(problem *problem)
{
	solution *map = empty();
	return solve_rec(problem, map, 0, 0, 0, 1 << 30);
}

int main(int argc, char **argv)
{

	char *problem_file = "problems/chr10a.dat";
	if (argc >= 2)
	{
		problem_file = argv[1];
	}

	// load problem
	problem *p = qap_load(problem_file);

	// run solver
	printf("Run solver ...\n");
	int best = solve(p);
	printf("Done!\n");
	printf("Best Result: %d\n", best);

	// verify result
	bool success = (best == p->optimum);
	printf("Verification: %s\n", (success ? "OK" : "ERR"));

	// free problem
	qap_del(p);

	return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

problem *qap_load(char *file)
{

	FILE *fp = fopen(file, "r");
	printf("Loading Problem File %s ..\n", file);

	// get problem size
	int problemSize;
	int read = fscanf(fp, "%d", &problemSize);
	assert(read > 0);
	printf("  - problem size: %d\n", problemSize);

	// create problem instance
	problem *res = (problem *)malloc(sizeof(problem));
	res->size = problemSize;
	res->A = qm_create(problemSize);
	res->B = qm_create(problemSize);

	// load matrix A
	for (int i = 0; i < problemSize; i++)
	{
		for (int j = 0; j < problemSize; j++)
		{
			read = fscanf(fp, "%d", &getA(res, i, j));
			assert(read > 0);
		}
	}

	// load matrix B
	for (int i = 0; i < problemSize; i++)
	{
		for (int j = 0; j < problemSize; j++)
		{
			read = fscanf(fp, "%d", &getB(res, i, j));
			assert(read > 0);
		}
	}

	// load optimum
	read = fscanf(fp, "%d", &(res->optimum));
	assert(read > 0);
	printf("  - optimum: %d\n", res->optimum);

	if (read == 0)
	{
		printf("Error reading input!\n");
	}

	fclose(fp);

	return res;
}
