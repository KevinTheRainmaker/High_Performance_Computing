#include <iostream>
#include <iomanip>
#include <mpi.h>
#define MASTER 0
#define SEED 35791246

using namespace std;

int main(int argc, char **argv)
{
    int nprocs, rank, npoints, circle_count=0, num, val=0;
    double x, y, pi, stime, etime;

    // MPI Init and rank
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == MASTER)
        stime = MPI_Wtime(); // start time

    // Get number of points
    npoints = 100000000;
    if (argc >= 2)
        npoints = atoi(argv[1]);

    // Calculate number of points for each rank
    if (num % nprocs != 0)
        num = npoints / (nprocs-1);
    else
        num = npoints / nprocs;
    
    // Random numbers and count the points of inside of the circle
    srand(SEED+rank);   // Give rand() a seed value
    for(int i=0 ; i < num ; i++){
        x = ((double) rand() / (RAND_MAX)) ;
        y = ((double) rand() / (RAND_MAX)) ; 
            
        if ( (x- (double) 1/2) * (x- (double) 1/2) + (y-(double) 1/2) * (y-(double)1/2) <= (double) 1/4 )
            circle_count += 1 ; 
    }

    MPI_Reduce(&circle_count, &val, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);

    if (rank == MASTER){
        cout<<"MASTER"<<endl;

        pi = (double) 4 * val / npoints;
        
        etime = MPI_Wtime(); // end time
        cout << "  " << setw(10) << "npoints"
             << "  " << setw(10) << "pi"
             << "  " << setw(10) << "nprocs"
             << "  " << setw(30) << "elapsed wall-clock time" << "\n";
        cout << "  " << setw(10) << npoints
             << "  " << setw(10) << pi
             << "  " << setw(10) << nprocs
             << "  " << setw(30) << etime-stime << "\n";
    }

    else{
    cout<<"WORKER"<<endl;
    
    }

    // MPI finalize
    MPI_Finalize();
}