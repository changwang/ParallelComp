#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ITER        10000000

#define VERTS       4
#define DIMENSION   3

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

void RandomVertex(double *vert, unsigned short seed[3])
{
    int i;
    for (i = 0; i < DIMENSION; i++)
    {
        vert[i] = erand48(seed);
    }
}

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
    double total, result;
    double mat[VERTS][DIMENSION];
    
    unsigned short seed[VERTS][DIMENSION] = {
        {1, 2, 3},
        {5, 6, 7},
        {9, 10, 11},
        {13, 14, 15}
    };
    
    total = 0.0;
    
    for (j = 0; j < ITER; j++)
    {
        for (i = 0; i < VERTS; i++)
        {
            RandomVertex(mat[i], seed[i]);
        }
        total += Determinant(mat);
    }
    
    result = total/(double)ITER;
    printf("The final result is %.10f\n", result/6);
    return 0;
}
