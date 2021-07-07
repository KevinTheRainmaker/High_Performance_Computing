#include <iostream>
#include <stdio.h>
#include <omp.h>

using namespace std;

int main()
{
	#pragma omp parallel
	{
	int id = omp_get_thread_num();
	printf("Hello World from ID=%d!\n",id);
	}

	return 0;
}
