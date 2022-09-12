#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "crackme.h"

void crack(int rank, int depth, char *guess, int size, int num_threads, int hit) {

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
		
			crack(rank, new_depth, guess, size, num_threads, hit);
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
			
			crack(rank, new_depth, guess, size, num_threads, hit);
			if (hit == 1)
				return;
		}
	}
}

// void crack2(int depth, char *guess, int size) {
// 	int new_depth = depth + 1;

//     if (depth >= size) {

//         if (p(size, guess) == 0) {
//             printf("HIT\n");
//             hit = 1;
//             return;
//         } 
//         return;
//     }
   
//     for (int i = 0; i < 256; i++) {
        
//         guess[depth] = i;
        

//         crack2(new_depth, guess, size);
//         if (hit == 1)
//             return;
//     }
// }

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

	printf("num threads: %d\n", world_size);
	printf("word size: %d\n", sizePass);

	// if (!world_rank){
	// 	printf("PID %d, %d\n", world_rank, p(sizePass, "ToTest0"));
	// }
	// else if (world_rank == 1){
	// 	printf("PID %d, %d\n", world_rank, p(sizePass, "ToTest1"));
	// }

	if (world_rank == 0) {
		char guess[sizePass];
		int hit = 0;
		crack(world_rank, 0, guess, sizePass, world_size, hit);
		// crack2(0, guess, sizePass);
	} else if (world_rank == 1) {
		char guess[sizePass];
		int hit = 0;
		crack(world_rank, 0, guess, sizePass, world_size, hit);
	}
	
	MPI_Finalize();
	return 0;
}