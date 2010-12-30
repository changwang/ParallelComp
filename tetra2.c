#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define VERTS       4
#define DIMENSION   3

/*
Calculate the determinant by given matrix
*/
double Determinant(double mat[][DIMENSION])
{
    double positive_sum = 0.0, nagtive_sum = 0.0;
    
    positive_sum = mat[0][0] * mat[1][1] * mat[2][2]
                 + mat[0][0] * mat[1][2] * mat[3][1]
                 + mat[0][0] * mat[2][1] * mat[3][2]
                 + mat[0][1] * mat[1][0] * mat[3][2]
                 + mat[0][1] * mat[1][2] * mat[2][0]
                 + mat[0][2] * mat[1][0] * mat[2][1]
                 + mat[0][2] * mat[1][0] * mat[3][1]
                 + mat[0][2] * mat[1][1] * mat[3][0]
                 + mat[0][2] * mat[2][0] * mat[3][1]
                 + mat[1][0] * mat[2][2] * mat[3][1]
                 + mat[1][1] * mat[2][0] * mat[3][2]
                 + mat[1][2] * mat[2][1] * mat[3][0];
                 
    nagtive_sum = mat[0][0] * mat[1][1] * mat[3][2]
                + mat[0][0] * mat[1][2] * mat[2][1]
                + mat[0][0] * mat[2][2] * mat[3][1]
                + mat[0][1] * mat[1][0] * mat[2][2]
                + mat[0][1] * mat[1][2] * mat[3][0]
                + mat[0][1] * mat[2][0] * mat[3][2]
                + mat[0][1] * mat[2][2] * mat[3][0]
                + mat[0][2] * mat[1][1] * mat[2][0]
                + mat[0][2] * mat[2][1] * mat[3][0]
                + mat[1][0] * mat[2][1] * mat[3][2]
                + mat[1][1] * mat[2][2] * mat[3][0]
                + mat[1][2] * mat[2][0] * mat[3][1];
    return fabs(positive_sum - nagtive_sum)/6.0;
}

/*
Assign random number to x, y and z axis.
*/
void RandomVertex(double *vert, unsigned short seed[3])
{
    int i;
    for (i = 0; i < DIMENSION; i++)
    {
        vert[i] = erand48(seed);
    }
}

/*
Print out the vertices in Tetrahedron,
aid to debug.
*/
void PrintTetra(double mat[][DIMENSION])
{
    int i, j;
    for (i = 0; i < VERTS; i++)
    {
        for (j = 0; j < DIMENSION; j++)
        {
            printf("The value at index [%d, %d] is: %.10f\t", i, j, mat[i][j]);
        }
        printf("\n");
    }
}

int main (int argc, char *argv[])
{
    int i, j;
    int n;
    int numprocs, myid;
    double ptotal, total, result;
    double mat[VERTS][DIMENSION];
    unsigned short seed[DIMENSION];
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    
    seed[0] = myid + 1;
    seed[1] = 2 * (myid + 1);
    seed[2] = 3 * (myid + 1);
        
    while (1)
    {
        total = 0.0;
        if (myid == 0)
        {
            printf("Enter the number of intervals: (0 quits)");
            scanf("%d", &n);
        }
        // Broadcast the interval to all processors in the world
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0)
            break;
        else
        {
            // Each processor only needs to calculate n/numprocs times
            ptotal = 0.0;
            for (i = 0; i < n/numprocs; i++)
            {
                for (j = 0; j < VERTS; j++)
                {
                    RandomVertex(mat[j], seed);
                }
                ptotal += Determinant(mat);
            }
            printf("Processor-%d: partial sum is %.10f\n", myid, ptotal);
            // Gether the partial sum to total
            MPI_Reduce(&ptotal, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
            if(myid == 0)
            {
                printf("total is approximately %.10f\n", total/n);
            }
        }
    }
    MPI_Finalize();
    return 0;
}