#include <stdio.h>
#include <limits.h>
#include <mpi.h>

#define NORM  2.328306549295727688e-10
#define A 16807
#define MAX_INT (2<<31 -1)

#define CHUNK 100000

#define SEED_TAG            0
#define RANDOM_NUMBER_TAG   1

typedef struct RandWithIter
{
    double random_number;
    int iter;
} Randwi;

void InitializeSeeds(unsigned long seeds[], int total)
{
    int i;
    for (i = 0; i < CHUNK; i++)
    {
        //seeds[i] = random();
        seeds[i] = i+1;
    }
}

int main (int argc, char *argv[])
{
    double seeds[CHUNK] = {0};
    unsigned long tempSeeds[CHUNK] = {0};
    unsigned long seed;
    int nprocs, myrank;
    int i;
    // The default iteration is 1000
    int nt = 0, iter = 1000;
    MPI_Status status;
    
    Randwi rwi;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    
    InitializeSeeds(tempSeeds, iter * nprocs);
    
    if(myrank == 0)
    {
        seed = tempSeeds[0];
        for(i = 1; i < nprocs; ++i)
        {
            MPI_Send(&tempSeeds[i], 1, MPI_LONG, i, SEED_TAG, MPI_COMM_WORLD);
        }
        MPI_Recv(&rwi, 1, sizeof(Randwi), MPI_ANY_SOURCE, RANDOM_NUMBER_TAG, MPI_COMM_WORLD, &status);
    }
    else
    {
        MPI_Recv(&seed, 1, MPI_LONG, 0, SEED_TAG, MPI_COMM_WORLD, &status);
        // printf("For process %d, the seed is: %ld\n", myrank, seed);
        seed = A*seed % MAX_INT;
        // printf("For process %d, the random num is: %20.20f\n", myrank, (double)seed*NORM);
        while (nt < iter)
        {
            nt++;
            rwi.random_number = seed;
            rwi.iter = iter;
            MPI_Send(&rwi, 1, sizeof(Randwi), 0, RANDOM_NUMBER_TAG, MPI_COMM_WORLD);
        }
    }
    
    if(myrank == 0)
    {
        // for (i = 0; i < nprocs; i++)
        // {
        //     printf("%20.20f\t", seeds[i]);
        // }
        // printf("\n");
        printf("%20.20f\n", rwi.random_number);
    }
    
    MPI_Finalize();
    return 0;
}