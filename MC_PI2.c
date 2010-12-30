#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>

#define CHUNKSIZE   1000
#define TOTAL       1000000

#define REQUEST_TAG     1
#define REPLY_TAG       2

int main (int argc, char *argv[])
{
    
    int iter;
    int in, out, i, ierr, ranks[1], done;
    double x, y, Pi, error, epsilon;
    double start_time, end_time;
    int numprocs, master, myrank, workerrank; 
    int totalin, totalout;
    long max, total;
    int rands[CHUNKSIZE], request;
    
    MPI_Comm world, workers;    // two communicators
    MPI_Group world_group, worker_group;    // two groups
    MPI_Status status;
    
    if (MPI_Init(&argc, &argv) != MPI_SUCCESS)
    {
        fprintf(stderr, "MPI initialization error\n");
        exit(-1);
    }
    
    // world is the MPI_COMM_WORLD communicator
    world = MPI_COMM_WORLD;     
    // get the processors within world communicator
    MPI_Comm_size(world, &numprocs);  
    // assign the rank for each processor in the communicator  
    MPI_Comm_rank(world, &myrank);
    
    if (argc <= 1) 
    {
        fprintf(stderr, "Usage: mpirun -np number_of_process ./mcpi number_of_epsilon number_of_iterations\n");
        MPI_Finalize();
        exit(-1);
    }
    
    // assign to master the rank of the last process in the world communicator
    master = numprocs - 1;      
    // if it is the first process, read the input from console
    if(myrank == 0)
    {
        sscanf(argv[1], "%lf", &epsilon);
        //sscanf(argv[2], "%ld", &total);
    }

    // then send the epsilon to all the worker process
    MPI_Bcast(&epsilon, 1, MPI_DOUBLE, 0, world);
    //MPI_Bcast(&total, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    // create the world group associated with world communicator
    MPI_Comm_group(world, &world_group);
    ranks[0] = master;
    // create the worker group from world group, but exclude the master process
    MPI_Group_excl(world_group, 1, ranks, &worker_group);
    // create a new communicator from existing communicator and group
    // this is the communicator we will use when we wish to do collective operations
    // that don not involve the master process
    MPI_Comm_create(world, worker_group, &workers);
    // free the two groups, we don't need them any more,
    // left work will be handled by communicators
    MPI_Group_free(&worker_group);
    MPI_Group_free(&world_group);
    
    
    // record the start time
    start_time = MPI_Wtime();
    // the master process is in charge of producing the random points,
    // then send the points to worker processes.
    if(myrank == master)
    {
        #if RANDOM_SEED
        struct timeval time;
        gettimeofday(&time, 0);
        // initialize the random number generator
        srandom((int)(time.tv_usec * 1000000 + time.tv_sec));
        #endif
        do 
        {
            // check whether the worker processes need new random points
            MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST_TAG, world, &status);
            if(request)
            {
                for(i = 0; i < CHUNKSIZE;)
                {
                    rands[i] = random();
                    if (rands[i] <= INT_MAX) i++;
                }
                MPI_Send(rands, CHUNKSIZE, MPI_INT, status.MPI_SOURCE, REPLY_TAG, world);
            }
        }
        while (request > 0);
    }
    // if it is a worker process
    else
    {
        // if request is 1, the worker process ask master process for random points
        request = 1;
        done = in = out = 0;
        max = (1 << 31) - 1;      // max int, for normalization
        MPI_Send(&request, 1, MPI_INT, master, REQUEST_TAG, world);
        // get the rank of current process in worker communicator
        MPI_Comm_rank(workers, &workerrank);
        // iter = 0;
        while (!done)
        {
            iter++;
            request = 1;
            MPI_Recv(rands, CHUNKSIZE, MPI_INT, master, REPLY_TAG, world, &status);
            for (i = 0; i < CHUNKSIZE-1;)
            {
                #if QUARTER_CIRCLE
                x = ((double) rands[i++])/max;
                y = ((double) rands[i++])/max;
                #else
                // avoid x and y have the same number
                x = (((double) rands[i++])/max) * 2 - 1;
                y = (((double) rands[i++])/max) * 2 - 1;
                #endif
                // if the point is in the circle, increase in
                if(x*x + y*y < 1.0)
                {
                    in++;
                }
                // if the point is out of the circle, increase out
                else
                {
                    out++;
                }
            }
            // combine the result from each worker process, 
            // and then send back to each worker process
            MPI_Allreduce(&in, &totalin, 1, MPI_INT, MPI_SUM, workers);
            MPI_Allreduce(&out, &totalout, 1, MPI_INT, MPI_SUM, workers);
            
            // if stop condition is not satisfied,
            // ask the master process to send points again
            if(myrank == 0)
            {
                Pi = (4.0 * totalin)/(totalin + totalout);
                error = fabs(Pi - M_PI);
                // if the error is acceptable or the total points is larger than 1000000,
                // stop the process
                done = (error < epsilon || (totalin + totalout) > TOTAL);
                request = (done) ? 0 : 1;
                MPI_Send(&request, 1, MPI_INT, master, REQUEST_TAG, world);
                MPI_Bcast(&done, 1, MPI_INT, 0, workers);
            }
            else
            {
                MPI_Bcast(&done, 1, MPI_INT, 0, workers);
                if(request)
                {
                    MPI_Send(&request, 1, MPI_INT, master, REQUEST_TAG, world);
                }
            }
        }
        MPI_Comm_free(&workers);
    }
    end_time = MPI_Wtime();
    
    if(myrank == 0)
    {
        printf("pi is: %23.20f\n", Pi);
        printf("\ntotal points: %d\nin: %d, out: %d, <ret> to exit\n", totalin+totalout, totalin, totalout);
        printf("It took %f seconds\n", end_time - start_time);
        getchar();
    }
    
    // MPE_Close_graphics(&graph);
    MPI_Finalize();
    
    return 0;
}
