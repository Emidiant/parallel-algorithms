#include <iostream>
#include <cstdio>
//#include "omp.h"
#include "/usr/local/Cellar/libomp/13.0.0/include/omp.h"

using namespace std;

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int* v= new int[n];
    int num_threads = 10;

    for (int i = 0; i < n; i++)
    {
        v[i] = rand() % n;
    }
    if (n == 10) {
        for (int i = 0; i < n; i++)
            cout << v[i] << " ";
        cout << "\n";
    }
    for (int num = 1; num <= num_threads; num++) {
        double total_time = 0;
        int *max_per_threads = new int[num];
        for (int j = 0; j < 10; j++) {
            int max_element = v[0];
            double time = omp_get_wtime();
#pragma omp parallel firstprivate(max_element) num_threads(num)
#pragma omp for
            for (int i = 0; i < n; i++) {
                if (v[i] > max_element) {
                    max_element = v[i];
                    max_per_threads[omp_get_thread_num()] = max_element;
                }
            }
            total_time += omp_get_wtime() - time;
        }
            printf("Number of threads %d\n", num);
//        for (int i = 0; i < num; i++)
//            printf("Ind: %d, Max: %d \n", i, max_per_threads[i]);
            printf("Max element: %d, Avg time exec: %f\n", *std::max_element(max_per_threads, max_per_threads + num), total_time/10);
    }

    delete[] v;
    return 0;
}
