#include <iostream>
#include <omp.h>

#define MAX_PARALLEL_RECURSIVE_LEVEL 4
#define NITERS (100*1000*1000ULL)

using namespace std;

double pi_recur(double h, unsigned d, unsigned max_d, unsigned long long begin, unsigned long long n){
	if (d<max_d){
		double ar1, ar2;

		#pragma omp task shared(ar1)
		ar1 = pi_recur(h, d+1, max_d, begin, n/2-1);

		#pragma omp task shared(ar2)
		ar2 = pi_recur(h, d+1, max_d, begin+n/2, n/2);

		#pragma omp taskwait

		return ar1+ar2;
	}
	else{
		unsigned long long i;
		double area = 0.0;

		for (i = begin; i<= begin+n; i++){
			double x = h* (i - 0.5);
			area += (4.0/(1.0+x*x));
		}

		return area;
	}
}

double pi(unsigned long long n){
	double res;
	double h = 1.0 / (double)n;

	#pragma omp parallel shared(res)
	{
		#pragma omp single
		res = pi_recur(h, 0, MAX_PARALLEL_RECURSIVE_LEVEL, 1, n);
	}

	return res * h;
}

int main(int argc, char *argv[]){
	cout << "Pi (w/" << NITERS << "iters) is " << pi(NITERS) << endl;
	return 0;
}
