#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define A           33614
#define M           2147483647 // (2<<31 - 1)
#define NUMBERS     10
#define PI      3.1415926

int seed, q, p;
int lcg32;

/*
The return value lies in the interval [0, 2<<32]
*/
int lcg()
{
    int temp;
    temp = A * seed;
    q = temp;
    q = q >> 1;
    p = temp >> 32;
    lcg32 = p + q;
    
    /*
    0x7FFFFFFF is equal to 2<<31 - 1,
    so lcg32 & 0x7FFFFFFFF actually is
    lcg32 mod 2<<31
    */
    if (lcg32 & 0x80000000)
    {
        lcg32 = lcg32 & 0x7FFFFFFF;
        lcg32++;
    }
    seed = lcg32;
    return lcg32;
}

/*
The return value lies in the interval [0.0, 1.0)
*/
double lcg01()
{
    int temp;
    temp = lcg();
    return ((double)temp/M);
}

/*
Test function for Monte Carlo Methods.
This one is also used to approximate PI.
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
    
    seed = lrand48();
    
    for (i = 0; i < NUMBERS; i++)
    {
        var = lcg01();
        total += MonteCarlo(var);
    }
    
    est = total/NUMBERS;
    printf("The final result is %5.10f\t\n", est);
    
    error = est - PI;
    printf("The error is %.10f\t\n", error);
    
    aberr = (double)1/(double)(2 * sqrt(NUMBERS));
    printf("The absolute error is %.10f\t\n", aberr);
    
    return 0;
}