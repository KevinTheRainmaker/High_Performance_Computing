#include <iostream>
#include <random>
#include <omp.h>

#define TEST 1  // 1: TEST mode to print out, 0: REAL mode

using namespace std;

void Merge(int* A,int l, int m,int r)
{
    int i, j, k;
    int nL=m-l+1;
    int nR=r-m;
    int* L = new int[nL];
    int* R = new int[nR];

    for(i=0;i<nL;i++) {
        L[i]=A[l+i];
    }
    for(j=0;j<nR;j++) {
        R[j]=A[m+j+1];
    }

    i=0,j=0,k=l;
    while(i<nL && j<nR) {
        if(L[i]<R[j]) {
            A[k]=L[i++];
        }
        else {
            A[k]=R[j++];
        }
        k++;
    }

    while(i<nL) {
        A[k++]=L[i++];
    }

    while(j<nR) {
        A[k++]=R[j++];
    }
}

void MergeSort(int* A, int l, int r)
{
    int task = 8;
    if (l<r) {
        int m = (l+r)/2;
        #pragma omp task shared (A) if (r-l+1>task)
        MergeSort(A, l, m);
        #pragma omp task shared (A) if (r-l+1>task)
        MergeSort(A, m+1, r);
        #pragma omp taskwait
        Merge(A, l, m, r);
    }
}

int main ()
{
    // number of array
    int n;
    cout << "Type N: ";
    cin >> n;
    cout << "* n = " <<  n << endl;

    // initialize random seed and generate numbers
    if (TEST) {
        srand (time(NULL));
    }
    int* numberArray = new int[n];
    for (int i=0; i<n; i++) {
        numberArray[i] = rand() % n;
    }

    if (TEST) {
        cout << "Before Merge Sorting: ";
        for (int i=0; i<n; i++) {
            cout << numberArray[i] << " ";
        }
	cout << "\n";
    }

    // call merge sort
    double begin = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        MergeSort(numberArray, 0, n-1);
    }
    double end = omp_get_wtime();
    double elapsed = end - begin;

    if (TEST) {
        cout << "After Merge Sorting: ";
        for (int i=0; i<n; i++) {
            cout << numberArray[i] << " ";
        }
	cout << "\n";
    }
    cout << "Elapsed Time: " << elapsed << "sec" << endl; 
    delete[] numberArray;

    return 0;
}
