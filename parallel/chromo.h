/*=====================================================
 * chromo.h
 *
 * @Author: Wenchong Chen
 *
 * This header file defines a structure to store
 * a chromosome of any sizes that are a multiple
 * of 8.
 *
 * A chromosome consists of a set of gene segments,
 * each gene segment is represented by unsigned char,
 * so it has 8 bits to represent 8 genes.
 *
 * It also declares function prototypes to manipulate
 * the chromosome structure, including
 * 1) init_chromo() to allocate memory dynamically
 *    for the chromosome, and initialize genes with
 *    random values
 * 2) free_chromo() to free the memories
 * 3) print_chromo() to print the genes in binary form
 *=====================================================*/


#ifndef CHROMO_H_
#define CHROMO_H_


#include <stdio.h>
#include <stdlib.h>


#define CHAR_MAX 255		// max value of unsigned char
#define CHAR_LENGTH 8		// # of bits of unsigned char


/*============ Type Definition ============*/
/*
 * a chromosome consists of a set of gene segments,
 * each gene segment has 8 bits to represent 8 genes
 */
typedef struct
{
	int num_genes;				// # of gene segments
	double fitness;				// fitness of the chromosome
	unsigned char* genes;	// gene segments in chromosome
}Chromo;


/*========== Function Prototype ==========*/
/*
 * allocate memories and initialise genes with
 * random values
 */
Chromo* init_chromo(int num_genes);


/*
 * free memories allocated by malloc()
 */
void free_chromo(Chromo* chr);


/*
 * print the chromosome in binary form
 */
void print_chromo(const Chromo* chr);


#endif