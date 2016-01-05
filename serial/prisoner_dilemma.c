/*=====================================================
 * prisoner_dilemma.c
 *
 * @Author: Wenchong Chen
 *
 * This definition file defines functions as declared
 * in the prisoner_dilemma.h header file.
 *=====================================================*/


#include "prisoner_dilemma.h"


/*========== Function Definition ==========*/
/*
 * every two prisoners play against each other
 */
void play_game(Group* grp)
{
	int A, B;		// indexes of player A and B
	int num_players = grp->num_chrs;

	// reset fitness to 0 before every iteration
	reset_fitness(grp);

	for (A = 0; A < num_players; A++)
	{
		for (B = A + 1; B < num_players; B++)
		{
			select_tactics(grp, A, B);
			update_fitness(grp, A, B);
			update_history(grp, A, B);
		}
	}
}


/*
 * select a new tactic for each player
 * based on last two history tactics records
 */
void select_tactics(Group* grp, int A, int B)
{
	int i;

	// get the index of the storage of history tactics
	// by the indexes of player A and B
	int index = transform_index(grp->num_chrs, A, B);

	for (i = 0; i < 2; i++)
	{
		// get the value of history tactics
		unsigned char hist = grp->history[index][i];

		// get position of the tactic to be chosen
		// in the strategy list/chromosome
		int gene_pos = hist / CHAR_LENGTH;
		int bit_pos = hist % CHAR_LENGTH;

		unsigned char mask = 1 << (CHAR_LENGTH - bit_pos - 1);

		if (0 == i)
		{
			// set tactic for player A
			mask &= grp->chrs[A]->genes[gene_pos];
			grp->tactics[A] = mask >> (CHAR_LENGTH - bit_pos - 1);
		}
		else
		{
			// set tactic for player B
			mask &= grp->chrs[B]->genes[gene_pos];
			grp->tactics[B] = mask >> (CHAR_LENGTH - bit_pos - 1);
		}
	}
}


/*
 * get the index of the storage of history tactics
 * by transforming the indexes of player A and B
 */
int transform_index(int num_chrs, int A, int B)
{
	return (2 * num_chrs - A - 1) * A / 2 + (B - A - 1);
}


/*
 * update history tactics for player A and B
 */
void update_history(Group* grp, int A, int B)
{
	unsigned char tac_A = grp->tactics[A];	// current tactic
	unsigned char tac_B = grp->tactics[B];	// current tactic
	unsigned char mask;

	int i;

	// get the index of the storage of history tactics
	// by the indexes of player A and B
	int index = transform_index(grp->num_chrs, A, B);
	
	// copy bits 3&4 to bits 1&2, copy new tactics to bits 3&4
	for (i = 0; i < 2; i++)
	{
		// make new tactics
		if (0 == i)
			mask = (tac_A << 1) + tac_B;
		else
			mask = (tac_B << 1) + tac_A;

		mask <<= 2;

		// the last tactics become the second last tactics
		grp->history[index][i] >>= 2;

		// add new tactics to the position of last tactics
		grp->history[index][i] += mask;
	}
}


/*
 * update fitnesses of player A and B, where
 * fitness is the score set by the game rules, and
 * the fitness of a player is the total score that
 * is gained by playing against all other players
 */
void update_fitness(Group* grp, int A, int B)
{
	int i = (grp->tactics[A] << 1) + grp->tactics[B];

	grp->chrs[A]->fitness += grp->rules[2 * i];
	grp->chrs[B]->fitness += grp->rules[2 * i + 1];
}


/*
 * reset fitnesses to 0's before every iteration,
 * only use this function inside this file
 */
void reset_fitness(Group* grp)
{
	int i;
	for (i = 0; i < grp->num_chrs; i++)
	{
		grp->chrs[i]->fitness = 0.0;
	}
}

