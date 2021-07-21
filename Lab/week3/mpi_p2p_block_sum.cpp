#include <iostream>
#include <mpi.h>

#define MASTER 0
#define N  100000000

using namespace std;

int main(int argc, char **argv){
	int nprocs, rank;
	// MPI Initialization
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	
	// MPI Execution
	int start_val = rank * (N/nprocs) + 1;
	int end_val = (rank+1) * (N/nprocs);
	int partial_sum, total_sum = 0;

	// master
	if (rank == 0){
		cout<<"MASTER"<<endl;
		// sum on master
		for (int i = start_val; i <= end_val; i++)
			total_sum += i;
		// recieves partial sum from worker and add it
		for (int src_rank = 1; src_rank < nprocs; src_rank++){
			MPI_Recv(&partial_sum, 1, MPI_LONG_LONG_INT, src_rank, 0,\
				       	MPI_COMM_WORLD, &status);
			total_sum += partial_sum;
		}
		cout << "Total sum: " << total_sum << endl;
	}

	// workers
	else{
		cout<<"WORKER"<<endl;
		// calculate partial sum
		for (int i = start_val; i < end_val+1; i++)
			partial_sum += i;
		// sends partial sum to master
		MPI_Send(&partial_sum, 1, MPI_LONG_LONG_INT, MASTER, 0,\
			       	MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	return 0;
}
