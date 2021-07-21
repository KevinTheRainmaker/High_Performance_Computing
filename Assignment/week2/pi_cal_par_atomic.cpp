# include <iostream>
# include <iomanip>
# include <omp.h>
# include <math.h>

using namespace std;

int num_threads;

double pi_cal_atomic ( int n, double h, int thread){
    int i;
	double sum=0.0, x;

	#pragma omp parallel num_threads(thread)
	{
	#pragma omp for schedule(static) private(i, x)

	for (i=0; i<n; i++)
	{
    	x = -1 + (i+0.5)*h;
		#pragma omp atomic
    	sum += sqrt(1 - x*x)*h;
	}
	
	num_threads = omp_get_num_threads();
	}
    return sum;
}


int main(){
	int n = 1000000000;
	double pi;
	double begin, end;
	int thArr[] = {2,4,8,16};

	double sum = 0.0;
	double h = 2.0 / n;

	for( int i = 0; i < 4; ++i)
	{
		begin = omp_get_wtime();
		sum = pi_cal_atomic(n, h, thArr[i]);
		end = omp_get_wtime();

		pi = sum*2.0;
	
        cout << setw(15) << "n" << setw(15) <<"threads" << setw(15) << "Pi" \
        << setw(15) << "time" << endl;

        cout << setw(15) << n << setw(15) << num_threads << setw(15) << pi \
        << setw(15) << end-begin << endl;
	}

	return 0;
}
