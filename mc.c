#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N       100
#define PI      3.1415926

/*
Function to approxmiate PI.
*/
double MonteCarlo(double variable)
{
    return 4 / (variable * variable + 1);
}

int main (int argc, char const *argv[])
{
    int i;
    double total = 0.0;
    double est = 0.0;
    double var;
    double error, aberr;
    
    /*
    This time uses lib's implmentation.
    */
    for (i = 0; i < N; i++)
    {
        var = drand48();
        total += MonteCarlo(var);
    }
    
    est = total/N;
    printf("The final result is %5.10f\t\n", est);
    
    error = est - PI;
    printf("The error is %.10f\t\n", error);
    
    aberr = (double)1/(double)(2 * sqrt(N));
    printf("The absolute error is %.10f\t\n", aberr);
    return 0;
}