#include <iostream>
#include <cstdio>
#include "/usr/local/Cellar/libomp/13.0.0/include/omp.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int* v= new int[n];
    for (int i = 0; i < n; i++)
    {
        v[i] = rand() % n;
    }
    if (n == 10) {
        for (int i = 0; i < n; i++)
            cout << v[i] << " ";
        cout << "\n";
    }
    double time = omp_get_wtime();
    int count = 0;
    int max_element = v[0];
#pragma omp parallel reduction(max:max_element)
    count++;
    for (int i = 0; i < n; i++)
        max_element = max(max_element, v[i]);

    delete[] v;
    printf("Max element = %d\n", max_element);
    printf("Time = %f\n", (omp_get_wtime() - time));
    printf("Number of threads = %d\n", count);

    return 0;
}
