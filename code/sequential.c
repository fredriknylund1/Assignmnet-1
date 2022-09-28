#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include "crackme.h"
#include "time.h"

void iterative_crack(unsigned char *guess, int sizepass, int rank, int num_threads, unsigned long long starttime) {

	int num_chars = 128;
	long size = pow(num_chars, sizepass);
	long div = num_chars / num_threads;
	int start = rank * div;
	int limit = start + div;

	long i = start;
	
	while (i < size) {
		
		if (p(sizepass, guess) == 0) {

			unsigned long long end = gettime();

			int total = 0;
			for (int i = 0; i < sizepass; i++) {
			total += guess[i];
			}

			printf("HIT!\n");
			printf("time: %llu\n", (end - starttime));

			return;
		}

		for (int index = sizepass - 1; index >= 0; index--) {

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

	unsigned char guess[sizePass];

	for (int i = 0; i < sizePass; i++) {
		guess[i] = 0;
	}

	unsigned long long start, end;
	start = gettime();
	iterative_crack(guess, sizePass, 0, 1, start);
	
	int total = 0;
	for (int i = 0; i < sizePass; i++) {
		total += guess[i];
	}
	printf("guess: %d\n", total);
	
	return 0;
}