/*=====================================================
 * group.c
 *
 * @Author: Wenchong Chen
 *
 * This definition file defines functions as declared
 * in the group.h header file.
 *=====================================================*/


#include "group.h"


/*========== Function Definition ==========*/
 /*
 * alloc memories to the group struct and members, and
 * initialise history tactics records, and
 * initialise each chromosome in the group
 */
Group* init_group(int num_genes, int num_chrs, double cross_rate, double mutate_rate)
{
	Group* grp = (Group*)malloc(sizeof(Group));

	grp->num_genes = num_genes;
	grp->num_chrs = num_chrs;
	grp->num_rounds = num_chrs * (num_chrs-1) / 2;
	grp->cross_rate = cross_rate;
	grp->mutate_rate = mutate_rate;
	grp->fit_total = 0.0;
	grp->rules = (unsigned char*)malloc(2 * NUM_RULES * sizeof(unsigned char));
	grp->tactics = (unsigned char*)malloc(num_chrs * sizeof(unsigned char));
	grp->history = (unsigned char**)malloc(grp->num_rounds * sizeof(unsigned char*));
	grp->fit_rate = (double*)malloc(num_chrs * sizeof(double));
	grp->chrs = (Chromo**)malloc(num_chrs * sizeof(Chromo*));

	// hardcode the game rules
	grp->rules[0] = 1;
	grp->rules[1] = 1;
	grp->rules[2] = 5;
	grp->rules[3] = 0;
	grp->rules[4] = 0;
	grp->rules[5] = 5;
	grp->rules[6] = 3;
	grp->rules[7] = 3;

	int i, j;
	for (i = 0; i < grp->num_rounds; i++)
	{
		grp->history[i] = (unsigned char*)malloc(2 * sizeof(unsigned char));

		for (j = 0; j < 2; j++)
		{
			// initialise history tactic records
			grp->history[i][j] = (unsigned char)rand() % NUM_COMBI;
		}
	}

	for (i = 0; i < num_chrs; i++)
	{
		// initialise chromosomes
		grp->chrs[i] = init_chromo(num_genes);
	}

	return grp;
}


/*
 * update relative fitness of all chromosomes in the group
 */
void update_fit_rate(Group* grp)
{
	int i;

	grp->fit_total = 0.0;

	// update total fitness of the group
	for (i = 0; i < grp->num_chrs; i++)
	{
		grp->fit_total += grp->chrs[i]->fitness;
	}

	// calculate relative fitness of each chromosome
	for (i = 0; i < grp->num_chrs; i++)
	{
		grp->fit_rate[i] = grp->chrs[i]->fitness / grp->fit_total;
	}
}


/*
 * evolution of the group, including the process of
 * select parent, crossover and mutation
 */
void evolve(Group* grp)
{
	select_parent(grp);
	crossover(grp);
	mutate(grp);
}


/*
 * select parent process that chooses new chromosomes
 * based on relative fitness. The chromosomes with
 * higher fitness rate have bigger chances to be chosen
 */
void select_parent(Group* grp)
{
	int num_genes = grp->num_genes;
	int num_chrs = grp->num_chrs;

	// tmp chromosome list to hold the old generation
	Chromo* tmp = (Chromo*)malloc(num_chrs * sizeof(Chromo));

	int i, j;
	for (i = 0; i < num_chrs; i++)
	{
		tmp[i].genes = (unsigned char*)malloc(num_genes * sizeof(unsigned char));

		for (j = 0; j < num_genes; j++)
		{
			// copy the genes of the old generation
			tmp[i].genes[j] = grp->chrs[i]->genes[j];
		}
	}

	// iterate through all chromosomes and select
	// new ones using Roulette Wheel Selection
	for (i = 0; i < num_chrs; i++)
	{
		int k = 0;
		double fit_rate_sum = grp->fit_rate[k];
		double rv = (double)rand() / RAND_MAX;

		// check which slot the RV falls
		while (rv > fit_rate_sum)
		{
			k++;
			fit_rate_sum += grp->fit_rate[k];
		}

		// select a new chromosomes to replace old generation
		for (j = 0; j < num_genes; j++)
		{
			grp->chrs[i]->genes[j] = tmp[k].genes[j];
		}
	}

	for (i = 0; i < num_chrs; i++)
	{
		free(tmp[i].genes);
	}
	free(tmp);
}


/*
 * crossover process that executes crossover of two
 * chromosomes after a randomly chosen bit position
 * if RV is less than crossover rate
 */
void crossover(Group* grp)
{
	int num_chrs = grp->num_chrs;
	int num_genes = grp->num_genes;

	int i, j;

	// iterate through chromosomes in pairs
	for (i = 0; i < num_chrs; i += 2)
	{
		double rv = (double)rand() / RAND_MAX;

		// if RV is less than crossover rate, do crossover
		if (rv <= grp->cross_rate)
		{
			int gene_pos;		// index of gene segment
			int bit_pos;		// bit position of gene segment

			// randomly select a gene segment and the bit position
			select_bit(grp->num_genes, &gene_pos, &bit_pos);

			// exchange bits after bit_pos in the same gene segment
			unsigned char gene1 = grp->chrs[i]->genes[gene_pos];
			unsigned char gene2 = grp->chrs[i+1]->genes[gene_pos];

			unsigned char mask1 = CHAR_MAX << (CHAR_LENGTH - bit_pos + 1);
			unsigned char mask2 = CHAR_MAX >> (bit_pos - 1);

			grp->chrs[i]->genes[gene_pos] = (gene1 & mask1) + (gene2 & mask2);
			grp->chrs[i+1]->genes[gene_pos] = (gene2 & mask1) + (gene1 & mask2);
			
			// exchage gene segments after gene_pos
			for (j = gene_pos + 1; j < num_genes; j++)
			{
				unsigned char tmp = grp->chrs[i]->genes[j];
				grp->chrs[i]->genes[j] = grp->chrs[i+1]->genes[j];
				grp->chrs[i+1]->genes[j] = tmp;
			}
		}	// end of if()
	}	// end of i-for()
}


/*
 * select a bit from a gene segment of a chromosome
 * for the crossover process
 */
void select_bit(int num_genes, int* gene_pos, int* bit_pos)
{
	int bit_total = num_genes * CHAR_LENGTH;
	int rv = rand() % bit_total;

	*gene_pos = rv / CHAR_LENGTH;			// index of gene segment
	*bit_pos = rv % CHAR_LENGTH + 1;	// bit position of gene segment
}


/*
 * mutation process that goes through every bit
 * in the chromosomes, and perform mutation to
 * that bit if RV is less than mutation rate
 */
void mutate(Group* grp)
{
	int i, j, bit;

	// iterate through every chromosome
	for (i = 0; i < grp->num_chrs; i++)
	{
		// iterate through every gene segment
		for (j = 0; j < grp->num_genes; j++)
		{
			// iterate through every bit in the gene segment
			for (bit = 0; bit < CHAR_LENGTH; bit++)
			{
				double rv = (double)rand() / RAND_MAX;

				// if RV is less than mutation rate, execute mutation
				if (rv < grp->mutate_rate)
				{
					unsigned char mask = 1 << (CHAR_LENGTH - bit - 1);
					grp->chrs[i]->genes[j] ^= mask;
				}
			}	// end bit-for()
		}	// end j-for()
	}	// end i-for()
}


/*
 * free memories of the group and members
 */
void free_group(Group* grp)
{
	int i;
	for (i = 0; i < grp->num_chrs; i++)
	{
		free_chromo(grp->chrs[i]);
	}

	for (i = 0; i < grp->num_rounds; i++)
	{
		free(grp->history[i]);
	}

	free(grp->chrs);
	free(grp->rules);
	free(grp->tactics);
	free(grp->history);
	free(grp->fit_rate);
	free(grp);
}


/*
 * print chromosomes in binary form
 */
void print_group(const Group* grp)
{
	printf("--- Total fitness = %.0lf\n", grp->fit_total);

	int i;
	for (i = 0; i < grp->num_chrs; i++)
	{
		printf("Chromosome %d of %d:\n", i+1, grp->num_chrs);
		print_chromo(grp->chrs[i]);
		printf("   Fitness = %.0lf\n", grp->chrs[i]->fitness);
		printf("   Fitness rate = %lf\n\n", grp->fit_rate[i]);
	}
}


