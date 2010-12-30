#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N       1000000
#define ACT     2.0/3.0

/*
Test 3 dimentional monte carlo methods.
The function here is x^2 + y^2,
which is approxmiately 2/3.
*/
double MonteCarlo2D(double variable1, double variable2)
{
    return variable1 * variable1 + variable2 * variable2;
}

/*
Test 4 dimensional monte carlo methods.
The function here is x^2 + y^2 + z^2,
which is approximately 1.
*/
double MonteCarlo3D(double variable1, double variable2, double variable3)
{
    return variable1 * variable1 + variable2 * variable2 + variable3 * variable3;
}

int main (int argc, char const *argv[])
{
    int i;
    double total = 0.0;
    double est = 0.0;
    double var1, var2, var3;
    double error, aberr;
    // Initialize the seed
    unsigned short xi[3] = {1, 2, 3};
    unsigned short yi[3] = {4, 5, 6};
    unsigned short zi[3] = {7, 8, 9};
    for (i = 0; i < N; i++)
    {
        var1 = erand48(xi);
        var2 = erand48(yi);
        var3 = erand48(zi);
        total += MonteCarlo2D(var1, var2);
        // total += MonteCarlo3D(var1, var2, var3);
    }
    
    est = total/N;
    printf("The final result is %5.10f\t\n", est);
    
    error = fabs(est - ACT);
    printf("The error is %.10f\t\n", error);
    
    aberr = (double)1/(double)(2 * sqrt(N));
    printf("The absolute error is %.10f\t\n", aberr);
    
    return 0;
}
