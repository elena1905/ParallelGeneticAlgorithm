/*=====================================================
 * group.h
 *
 * @Author: Wenchong Chen
 *
 * This header file defines a structure to store
 * a group of chromosomes. The number of chromosomes
 * should be even
 *
 * It also declares function prototypes to manipulate
 * the group structure.
 *=====================================================*/


#ifndef GROUP_H_
#define GROUP_H_


#include <stdio.h>
#include <stdlib.h>
#include "chromo.h"


#define CHAR_MAX 255		// max value of unsigned char
#define CHAR_LENGTH 8		// # of bits of unsigned char


/*============ Type Definition ============*/
typedef struct
{
	int num_genes;			// # of gene segments
	int num_chrs;				// # of chromosomes
	double cross_rate;	// crossover rate
	double mutate_rate;	// mutation rate
	double fit_total;		// total fitness of the group
	double* fit_rate;		// relative fitness of each chromosome
	Chromo** chrs;			// a list of chromosomes
}Group;


/*========== Function Prototype ==========*/
/*
 * alloc memories to the group struct and members, and
 * initialise each chromosome in the group
 */
Group* init_group(int num_genes, int num_chrs, double cross_rate, double mutate_rate);


/*
 * update fitness of a chromosome, which counts # of 1's
 * in the binary form of the gene segements
 */
double update_fitness(int num_genes, unsigned char* genes);


/*
 * update relative fitness of all chromosomes in the group
 */
void update_fit_rate(Group* grp);


/*
 * evolution of the group, including the process of
 * select parent, crossover and mutation
 */
void evolve(Group* grp);


/*
 * select parent process that chooses new chromosomes
 * based on relative fitness. The chromosomes with
 * higher fitness rate have bigger chances to be chosen
 */
void select_parent(Group* grp);


/*
 * crossover process that executes crossover of two
 * chromosomes after a randomly chosen bit position
 * if RV is less than crossover rate
 */
void crossover(Group* grp);


/*
 * select a bit from a gene segment of a chromosome
 * for the crossover process
 */
void select_bit(int num_genes, int* gene_pos, int* bit_pos);


/*
 * mutation process that goes through every bit
 * in the chromosomes, and perform mutation to
 * that bit if RV is less than mutation rate
 */
void mutate(Group* grp);


/*
 * free memories of the group and members
 */
void free_group(Group* grp);


/*
 * print chromosomes in binary form
 */
void print_group(const Group* grp);


#endif