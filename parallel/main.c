/*=====================================================
 * main.c
 *
 * @Author: Wenchong Chen
 *
 * This file is for testing parallel Genetic Algorithm
 * using Master-Slave model.
 *=====================================================*/


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "chromo.h"
#include "group.h"


int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));

	int i, rank, size;
	int job_id;
	int num_slaves;             // # of slaves
	int num_gen = 100;          // # of generations
	int num_genes = 2;          // # of gene segments
	int num_chrs = 8;           // # of chromosomes
	double cross_rate = 0.95;   // crossover rate
	double mutate_rate = 0.001; // mutation rate
	double fitness;
	unsigned char* genes = (unsigned char*)malloc(num_genes * sizeof(unsigned char));
	Group* grp = init_group(num_genes, num_chrs, cross_rate, mutate_rate);
	MPI_Status stat;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* master is here */
	if (0 == rank)
	{
		num_slaves = size - 1;
		job_id = 0;

		for (i = 1; i < size; i++)
		{
			// send next job i, send grp->chrs[i]->genes
			MPI_Send(grp->chrs[job_id]->genes, num_genes, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
			job_id++;
		}

		while (job_id < num_chrs)
		{
			i = job_id % num_slaves + 1;

			// get result s, recv grp->chrs[s]->genes
			MPI_Recv(&(grp->chrs[job_id-num_slaves]->fitness), 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &stat);

			// send next job s, send grp->chrs[s]->genes
			MPI_Send(grp->chrs[job_id]->genes, num_genes, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
			job_id++;
		}

		int job_left = num_chrs % num_slaves;
		for (i = 0; i < num_genes; i++)
		{
			genes[i] = 0;
		}

		for (i = 1; i < size; i++)
		{
			int pid = (i + job_left - 1) % num_slaves + 1;

			// get result s, recv grp->chrs[s]->fitness
			MPI_Recv(&(grp->chrs[job_id-num_slaves]->fitness), 1, MPI_DOUBLE, pid, 0, MPI_COMM_WORLD, &stat);
			job_id++;

			// no more jobs s, send genes with all 0 values to represent no job
			MPI_Send(genes, num_genes, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		/* slaves are here */
		while (1)
		{
			// get next job s, recv grp->chrs[s]->genes
			MPI_Recv(genes, num_genes, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, &stat);

			// stop process if no job
			unsigned char sum = 0;
			for (i = 0; i < num_genes; i++)
			{
			sum += genes[i];
			}

			// if sum of values of all gene segments is 0,
			// it means no job and should stop
			if (0 == sum)
			break;

			// do work
			fitness = update_fitness(num_genes, genes);

			// send result s, send fitness
			MPI_Send(&fitness, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		}
	}

	/* Genetic Algorithm Process done by master */
	if (0 == rank)
	{
		for (i = 0; i < num_gen; i++)
		{
			update_fit_rate(grp);
			evolve(grp);
		}
	}

	free(genes);
	free_group(grp);

	MPI_Finalize();



	return 0;
}
