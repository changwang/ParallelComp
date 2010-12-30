#include <stdio.h>
#include <stdlib.h>

/*
This one is used to approach pi by a sphere in the cube.
the volume ratio is pi/6.
*/
int main(int argc, char *argv[])
{
    int count;
    int i;
    int n;
    double pi;
    unsigned short xi[3];
    double x, y, z;
    
    
    if(argc != 5)
    {
        printf("Correct command line: ");
        printf("%s <# samples> <seed0> <seed1> <seed2>\n", argv[0]);
        return -1;
    }
    
    n = atoi(argv[1]);
    for (i = 0; i < 3; i++)
    {
        xi[i] = atoi(argv[i+2]);
    }
    
    count = 0;
    for (i = 0; i < n; i++)
    {
        x = erand48(xi);
        y = erand48(xi);
        z = erand48(xi);
        if (x*x + y*y + z*z <= 1.0) count++;
    }
    pi = 6.0 * (double)count / (double)n;
    printf("Samples: %d    Estimate of pi: %7.5f\n", n, pi);
}
