#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include "crackme.h"

void recursive_crack(int rank, int depth, char *guess, int size, int num_threads, int hit) {

    int new_depth = depth + 1;

	if (new_depth == 1) {

		int div = 256 / num_threads;
		int start = rank * div;
        int limit = start + div; 

		printf("div: %d\nstart: %d\nlimit: %d\n", div, start, limit);
		int i = 0;
		while (i < limit) {
			// printf("i: %d\n", i);
			guess[depth] = i;
		
			recursive_crack(rank, new_depth, guess, size, num_threads, hit);
			if (hit == 1)
				return;

			i++;
		}

    } else {

		if (depth >= size) {
			// printf("depth >= size");

			if (p(size, guess) == 0) {
				printf("HIT\n");
				hit = 1;
				return;
			} 
			return;
		}
	
		for (int i = 0; i < 256; i++) {
			// printf("i: %d\n", i);
		
			guess[depth] = i;
			
			recursive_crack(rank, new_depth, guess, size, num_threads, hit);
			if (hit == 1)
				return;
		}
	}
}

void iterative_crack(unsigned char *guess, int sizepass, int rank, int num_threads) {

	int num_chars = 128;
	long size = pow(num_chars, sizepass);
	long div = num_chars / num_threads;
	int start = rank * div;
	int limit = start + div;

	printf("start: %d\nlimit: %d\n", start, limit);
	long i = start;
	
	while (i < size) {
		
		if (p(sizepass, guess) == 0) {

			int total = 0;
			for (int i = 0; i < sizepass; i++) {
			total += guess[i];
			}
			printf("guess: %d\n", total);

			MPI_Abort(MPI_COMM_WORLD, 0);
			return;
		}

		for (int index = sizepass - 1; index >= 0; index--) {

			// if (rank == 0) 
			// 	printf("index: %d, guess[index]: %d | num_chars - 1: %d\n", index, (int)guess[index], num_chars - 1);

			// 	for (int i = 0; i < num_chars; i++) {
			// 		printf("%d", guess[i]);
			// 	}
			// 	printf("\n");

			if (index == sizepass - 1) {

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

	iterative_crack(guess, sizePass, world_rank, world_size);
	
	int total = 0;
	for (int i = 0; i < sizePass; i++) {
		total += guess[i];
	}
	printf("guess: %d\n", total);
	
	MPI_Finalize();
	return 0;
}