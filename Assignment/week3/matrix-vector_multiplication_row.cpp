#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>
#define MASTER 0

using namespace std;

int main(int argc, char **argv){
  // Just simply set N(4) X M(4) matrix and M(4) X 1 vector
  int N=4, M=4;
  int A[N*M], Apart[M], B[M], X[N];
  int Xpart = 0, root=0;
  int nproc, rank;

  // MPI init
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Initialize matrix
  if (rank == MASTER) {
    for(int i=0 ; i<N ; i++)
        B[i] = i;

    for(int i=0 ; i<N*M ; i++)
      A[i] = i;
  }
    
  MPI_Scatter(&A, M, MPI_INT, &Apart, M, MPI_INT, MASTER, MPI_COMM_WORLD);

  MPI_Bcast(&B, M, MPI_INT, MASTER, MPI_COMM_WORLD);
  
  for(int i=0 ; i<M ; i++)
    Xpart += Apart[i] * B[i];
  
  MPI_Gather(&Xpart, 1, MPI_INT, &X, 1, MPI_INT, MASTER,MPI_COMM_WORLD);

  if (rank == MASTER)
    for(int i=0; i<N; i++)
      cout << "X[" << i << "]=" << X[i] << endl;  

  // MPI Finalize
  MPI_Finalize();
  return 0;
}
