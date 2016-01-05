/*=====================================================
 * chromo.c
 *
 * @Author: Wenchong Chen
 *
 * This definition file defines functions as declared
 * in the chromo.h header file, including
 * 1) init_chromo() to allocate memory dynamically
 *    for the chromosome, and initialize genes with
 *    random values
 * 2) free_chromo() to free the memories
 * 3) print_chromo() to print the genes in binary form
 *=====================================================*/


#include "chromo.h"


/*========== Function Definition ==========*/
/*
 * allocate memories and initialise genes with
 * random values
 */
Chromo* init_chromo(int num_genes)
{
	Chromo* chr = (Chromo*)malloc(sizeof(Chromo));

	chr->fitness = 0.0;
	chr->num_genes = num_genes;
	chr->genes = (unsigned char*)malloc(num_genes *
								sizeof(unsigned char));

	int i;
	for (i = 0; i < num_genes; i++)
	{
		// initialise each gene segment with random values
		// in the range [0, 255]
		chr->genes[i] = (unsigned char)rand() % (CHAR_MAX + 1);
	}

	return chr;
}


/*
 * free memories allocated by malloc()
 */
void free_chromo(Chromo* chr)
{
	free(chr->genes);
	free(chr);
}


/*
 * print the chromosome in binary form
 */
void print_chromo(const Chromo* chr)
{
	int i, j;
	for (i = 0; i < chr->num_genes; i++)
	{
		printf("    Gene %d ", i+1);
	}

	printf("\n   ");

	// print each gene segment in the chromosome
	for (i = 0; i < chr->num_genes; i++)
	{
		// print each bit in a gene segment in the order
		// of from highest bit to lowest bit
		for (j = 0; j < CHAR_LENGTH; j++)
		{
			// shift the current bit to the lowest bit
			unsigned char current_bit = chr->genes[i] >> (CHAR_LENGTH - j - 1);

			// print the value of the current bit
			printf("%d", current_bit & 1);
		}

		printf("   ");
	}

	printf("\n");
}


