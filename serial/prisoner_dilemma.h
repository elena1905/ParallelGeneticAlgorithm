/*=====================================================
 * prisoner_dilemma.h
 *
 * @Author: Wenchong Chen
 *
 * This header file declares function prototypes to
 * play the Prisoner Dilemma game.
 *=====================================================*/


#ifndef PRISONER_DILEMMA_H_
#define PRISONER_DILEMMA_H_


#include <stdio.h>
#include <stdlib.h>
#include "group.h"


#define CHAR_MAX 255		// max value of unsigned char
#define CHAR_LENGTH 8		// # of bits of unsigned char
#define NUM_RULES 4			// # of game rules
#define NUM_COMBI 16		// # of tactic combinations


/*========== Function Prototype ==========*/
/*
 * every two prisoners play against each other
 */
void play_game(Group* grp);


/*
 * select a new tactic for each player
 * based on last two history tactics records
 */
void select_tactics(Group* grp, int A, int B);


/*
 * update history tactics for player A and B
 */
void update_history(Group* grp, int A, int B);


/*
 * get the index of the storage of history tactics
 * by transforming the indexes of player A and B
 */
int transform_index(int num_chrs, int A, int B);


/*
 * update fitnesses of player A and B, where
 * fitness is the score set by the game rules, and
 * the fitness of a player is the total score that
 * is gained by playing against all other players
 */
void update_fitness(Group* grp, int A, int B);


/*
 * reset fitnesses to 0's before every iteration,
 * only use this function inside this file
 */
void reset_fitness(Group* grp);

#endif