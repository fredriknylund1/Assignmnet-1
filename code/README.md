# Assignment-1

To compile: 
mpicc mainMPI.c -lm time.c crackme.o -o {executable name}

To run:
mpiexec -n {number of processes} ./{executable name} {password size}

