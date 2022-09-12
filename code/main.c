#include "crackme.h"
#include <stdlib.h>
#include <stdio.h>

int SIZE = 3;

int hit = 0;

void crack(int rank, char *guess) {

    int new_rank = rank + 1;

    if (rank >= SIZE) {

        if (p(SIZE, guess) == 0) {
            printf("HIT\n");
            hit = 1;
            return;
        } 
        return;
    }
   
    for (int i = 0; i < 256; i++) {
        
        guess[rank] = i;
        

        crack(new_rank, guess);
        if (hit == 1)
            return;
    }
}

int main(int argc, char *argv[]) {
    
    char guess[SIZE];
   
    crack(0, guess);

    printf("string: %s\n", guess);
    // for (int i = 0; i < 256; i++) {

    //     printf("%d\n", i);
    //     guess[0] = i + '0';

    //     for (int j = 0; j < 256; j++) {
    //         guess[1] = j + '0';

    //         for (int k = 0; k < 256; k++) {
    //             guess[2] = k + '0';

    //             for (int l = 0; l < 256; l++) {
    //                 guess[3] = l + '0';

    //                 int t = p(SIZE, guess);
    //                 // printf("i: %d | test: %d\n", i, t);
    //                 if (t == 0) {
    //                     printf("HIT");
    //                     return;
    //                 }
    //             }
    //         }
    //     }
    // }

}


