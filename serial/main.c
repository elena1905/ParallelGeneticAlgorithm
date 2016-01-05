/*=====================================================
 * main.c
 *
 * @Author: Wenchong Chen
 *
 * This file is for testing Prisoner's Dilemma game
 * using serial genetic algorithm.
 *=====================================================*/


#include <stdio.h>
#include <stdlib.h>
#include "group.h"
#include "prisoner_dilemma.h"


/*========== Function Prototype ==========*/
/*
 * print usage and help info
 */
void print_usage(void);


/*============== main() =================*/
int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	/* check arguments length */
	int numArgv = 13;

	if (numArgv != argc)
	{
		print_usage();  // print usage and help info
		exit (1);
	}

	/* get values from command line */
	int num_genes;				// # of gene segments
	int num_players;			// # of chromosomes/prisoners
	int num_gen;					// # of generations
	int num_iters;				// # of game iterations
	double cross_rate;		// crossover rate
	double mutate_rate;		// mutation rate

	// the order of arguments is arbitrary
	int i, j;
	for (i = 1; i < numArgv; i+=2)
	{
		if (0 == strcmp("-s",argv[i]))
			num_genes = atoi(argv[i+1]);
		else if (0 == strcmp("-p",argv[i]))
			num_players = atoi(argv[i+1]);
		else if (0 == strcmp("-g",argv[i]))
			num_gen = atoi(argv[i+1]);
		else if (0 == strcmp("-i",argv[i]))
			num_iters = atoi(argv[i+1]);
		else if (0 == strcmp("-c",argv[i]))
			cross_rate = atof(argv[i+1]);
		else if (0 == strcmp("-m",argv[i]))
			mutate_rate = atof(argv[i+1]);
		else
		{
			print_usage();  // print usage and help info
			exit (2);
		}
	}

	/* Prisoner's Dilemma Game */
	Group* players = init_group(num_genes, num_players, cross_rate, mutate_rate);

	// run for num_gen generations
	for (i = 0; i < num_gen; i++)
	{
		// play PD game for num_iters times
		for (j = 0; j < num_iters; j++)
		{
			play_game(players);
		}

		update_fit_rate(players);
		evolve(players);
	}

	free_group(players);

	return 0;
}


/*========== Function Definition ==========*/
/*
 * print usage and help info
 */
void print_usage(void)
{
	printf("=== Usage: ./main -s 2 -p 4 -g 100 -i 50 -c 0.95 -m 0.001\n");
	printf("    -s  # of gene segments, e.g. 2\n");
	printf("    -p  size of population/chromosomes/players, e.g. 4\n");
	printf("    -g  # of generations, e.g. 100\n");
	printf("    -i  # of iterations, e.g. 50\n");
	printf("    -c  crossover rate, very high, e.g. >=0.95\n");
	printf("    -m  mutation rate, very low, e.g. <=0.001\n");
}

