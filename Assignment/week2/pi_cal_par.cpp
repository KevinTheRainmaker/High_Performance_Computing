# include <iostream>
# include <omp.h>
# include <iomanip>
# include <math.h>

using namespace std;
int num_threads;

double pi_cal_reduction(int n, double h, int thNum){
    double sum = 0, x = 0;

    #pragma omp parallel reduction(+:sum) num_threads(thNum)
    {
    #pragma omp for schedule(static, 100) private(i, x)
    for (int i = 0; i<n; i++){
        x = -1 + (i+0.5)*h;
        sum += sqrt(1-x*x)*h;
    }
    num_threads = omp_get_num_threads();
    }
    return sum;
}

int main(){
    int n = 1000000000;
    double pi, sum =0;
    double h = 2.0/n;

    double begin, end;

    int thArr[] = {2,4,8,16};

    for(int thNum : thArr){
    	begin = omp_get_wtime();
        sum = pi_cal_reduction(n, h, thNum);
        end = omp_get_wtime();

        pi = sum * 2.0;

        cout << setw(15) << "n" << setw(15) <<"threads" << setw(15) << "Pi" \
        << setw(15) << "time" << endl;

        cout << setw(15) << n << setw(15) << num_threads << setw(15) << pi \
        << setw(15) << end-begin << endl;
    }
    
    return 0;
}
