#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include "crackme.h"
#include "time.h"

void iterative_crack(unsigned char *guess, int sizepass, int rank, int num_threads, unsigned long long starttime) {

	/* Each process calculates what part of the possible characters to check. */
	int num_chars = 128;
	long size = pow(num_chars, sizepass);
	long div = num_chars / num_threads;
	int start = rank * div;
	int limit = start + div;

	// printf("start: %d\nlimit: %d\n", start, limit);
	long i = start;
	

	while (i < size) {
		

		/* Checks if current guess is the correct guess. */
		if (p(sizepass, guess) == 0) {

			unsigned long long end = gettime();
			
			int total = 0;
			for (int i = 0; i < sizepass; i++) {
			total += guess[i];
			}
			printf("guess: %d\n", total);

			printf("HIT!\n");
			printf("time: %llu\n", (end - starttime));

			/* If guess is correct, all processes are aborted. */
			MPI_Abort(MPI_COMM_WORLD, 0);
			return;
		}

		/* Iterates through each index in the char array, starting with the upmost index. */
		// for (int index = sizepass - 1; index >= 0; index--) {

		// 	// if (rank == 0) 
		// 	// 	printf("index: %d, guess[index]: %d | num_chars - 1: %d\n", index, (int)guess[index], num_chars - 1);

		// 	// 	for (int i = 0; i < sizepass; i++) {
		// 	// 		printf("%d\n", guess[i]);
		// 	// 	}
		// 	// 	printf("\n");

		// 	if (index == sizepass - 1) {

		// 		if (guess[index] < limit - 1) {
		// 			guess[index] += 1;
		// 			break;
		// 		} else {
		// 			guess[index] = start;
		// 		}

		// 	} else {

		// 		if (guess[index] < num_chars - 1) {
		// 			guess[index] += 1;
		// 			break;
		// 		} else {
		// 			guess[index] = 0;
		// 		}

		// 	}
		// }

		for (int index = 0; index >= sizepass - 1; index++) {

			// if (rank == 0) 
			// 	printf("index: %d, guess[index]: %d | num_chars - 1: %d\n", index, (int)guess[index], num_chars - 1);

			// 	for (int i = 0; i < sizepass; i++) {
			// 		printf("%d\n", guess[i]);
			// 	}
			// 	printf("\n");

			if (index == 0) {

				if (guess[index] < limit - 1) {
					guess[index] += 1;
					break;
				} else {
					guess[index] = start;
				}

			} else {

				if (guess[index] < num_chars - 1) {
					guess[index] += 1;
					break;
				} else {
					guess[index] = 0;
				}

			}
		}

		i++;
	}
}

int main(int argc, char **argv){
	if (argc != 2){
		printf("Wrong usage: %s <size of password to generate>\n", argv[0]);
		exit(1);
	}
	int sizePass = atoi(argv[1]);
	
	int world_rank, world_size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	
	unsigned char guess[sizePass];

	for (int i = 0; i < sizePass; i++) {
		guess[i] = 0;
	}

	// if (world_rank == 0) {
	// 	iterative_crack(guess, sizePass, world_rank, world_size);
	// } else if (world_rank == 1) {
	// 	iterative_crack(guess, sizePass, world_rank, world_size);
	// }
	unsigned long long start, end;
	start = gettime();
	iterative_crack(guess, sizePass, world_rank, world_size, start);
	// end = gettime();
	
	

	// int total = 0;
	// for (int i = 0; i < sizePass; i++) {
	// 	total += guess[i];
	// }
	// printf("guess: %d\n", total);
	
	MPI_Finalize();
	return 0;
}