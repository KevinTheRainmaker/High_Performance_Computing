#include <iostream>
#include <iomanip>
#include <mpi.h>
#define MASTER 0
#define SEED 35791246

using namespace std;

int main(int argc, char **argv)
{
    int nprocs, rank, npoints, circle_count=0, num, val;
    double x, y, pi, stime, etime,r;

    // MPI Init and rank
    // Your code
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == MASTER)
        stime = MPI_Wtime(); // start time

    // Get number of points
    npoints = 10000000;
    if (argc >= 2)
        npoints = atoi(argv[1]);

    // Calculate number of points for each rank
    num=npoints/nprocs;
    

    // You should consider npoints is not divided nprocs

    // Random numbers and count the points of inside of the circle
    srand(SEED+rank);   // Give rand() a seed value
    // Your code
    for (int i=0;i<num;i++){
	   x=(double) rand()/RAND_MAX;
	   y=(double) rand()/RAND_MAX;
	   r=x*x+y*y;
           if (r<=1.0) circle_count+=1;
    }
    cout<<x<<"   "<<y<<"  "<<r<<"   "<<circle_count<<'\n';
    val=123;
    if (rank == MASTER) // I am MASTER
    {
        // Your code
	int my_circle_count=circle_count;
	for (int i=1; i<nprocs;i++){
		MPI_Recv(&circle_count , 1, MPI_INT,MPI_ANY_SOURCE , MPI_ANY_TAG,MPI_COMM_WORLD,&status);
        	my_circle_count+=circle_count;
	}
	pi=4.0*(double) my_circle_count/npoints;
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
    else  // I am WORKER
    {
        MPI_Send(&circle_count, 1, MPI_INT, 0, val, MPI_COMM_WORLD);// Your code
    }

    // MPI finalize
    MPI_Finalize();
}
