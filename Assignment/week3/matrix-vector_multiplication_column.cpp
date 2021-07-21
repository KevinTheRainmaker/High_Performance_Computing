#include <iostream>
#include <cstdlib>     // has exit(), etc.
#include <ctime>
#include <mpi.h>
#define MASTER 0

using namespace std;

int main(int argc, char **argv){
  // Just simply set N(4) X M(4) matrix and M(4) X 1 vector
  int N=4, M=4;
  int A[N][M], Apart[N], B[M], Bpart, X[N];
  int Xpart[N], root=0;
  int nproc, rank;

  // initialize for MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  if (rank == MASTER) {
	  for (int j=0; j<M; j++){
		  for (int i=0; i<N; i++)
			  A[i][j] = (4*i) + (j+1);
		  B[j] = j + 1;
	  }
  }

  int sendCnts[N] = {1,1,1,1};
  int displ[N]= {1};

  for (int i=0; i<N; i++){
	  for (int j=0; j<M; j++)
		  displ[j] = i*M + j;
	  MPI_Scatterv(&A, sendCnts, displ, MPI_INT, &Apart[i], 1, MPI_INT, MASTER, MPI_COMM_WORLD);
  }

  MPI_Scatter(&B, 1, MPI_INT, &Bpart, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

  for (int i=0; i<N; i++)
	  Xpart[i] = Apart[i] * Bpart;

  MPI_Reduce(&Xpart, &X, N, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);

  if (rank == MASTER) {
    for(int i=0; i<N; i++)
      cout << "X[" << i << "]=" << X[i] << endl;
  }

//========== MPI finalize ==========
  MPI_Finalize();  // MPI finalize

  return 0;     // Exit

}
