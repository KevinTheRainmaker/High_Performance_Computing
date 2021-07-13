# include <iostream>
# include <omp.h>
# include <ctime>
# include <math.h>
# include <iomanip>

using namespace std;

int main(){
    int n = 1000000000;
    double pi, sum = 0;
    double h = 2.0 / n;

    clock_t begin = clock();
    for(int i = 0 ; i<n ; i++){
        double x = -1 + (i+0.5)*h;
        sum += sqrt(1 - x*x)*h;
    }
    pi = sum*2.0;
    clock_t end = clock();

    double time = double(end-begin) / CLOCKS_PER_SEC;
    cout<<setw(15)<<"n"<<setw(15)<<"Pi"<<setw(15)<<"time"<<endl;
    cout<<setw(15)<<n<<setw(15)<<pi<<setw(15)<<time<<endl;
}