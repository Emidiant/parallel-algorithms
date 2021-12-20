#include <iostream>
#include <cmath>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"
using namespace std;
#define VECTOR_SIZE 1000000;

int main(int argc, char **argv) {
    int rank, n, sent_size, sum_sub, *a;
    int vec_size = VECTOR_SIZE;
    double start_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
//    Cascade
    if (rank == 0) {
        a = new int[vec_size]();
        for (int i = 0; i < vec_size; i++)
            a[i] = 1;
        start_time = MPI_Wtime();
    }
    sent_size = vec_size / n;
    int *a_sub = new int[sent_size]();
    MPI_Scatter(a, sent_size, MPI_INT, a_sub, sent_size, MPI_INT, 0, MPI_COMM_WORLD);

    int sent_vec_sum = 0;
    for (int i = 0; i < sent_size; i++)
        sent_vec_sum += a_sub[i];

    for (int cascade_step = 2; cascade_step <= n; cascade_step *= 2) {
        if (rank % cascade_step == 0) {
            if (rank + cascade_step / 2 < n) {
                MPI_Recv(&sum_sub, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                sent_vec_sum += sum_sub;
                printf("Cascade level: %d, Rank %d:, Source: #%d, Sub sum: %d, Vec sum: %d\n", (int)log2(cascade_step), rank, status.MPI_SOURCE, sum_sub, sent_vec_sum);
            }
        } else {
            MPI_Send(&sent_vec_sum, 1, MPI_INT, rank - rank % cascade_step, 0, MPI_COMM_WORLD);
            break;
        }
    }

    if (rank == 0){
        cout << "Cascade sum: " << sent_vec_sum << ", Elapsed time: " << MPI_Wtime() - start_time << endl;
        delete[] a;
    }

//    Reduce
    sent_vec_sum = 0;
    if (rank == 0) {
        a = new int[vec_size];
        for (int i = 0; i < vec_size; i++)
            a[i] = 1;
        start_time = MPI_Wtime();
    }
    int *sub_a = new int[sent_size];
    int sub_sum = 0;
    MPI_Scatter(a, sent_size, MPI_INT, sub_a, sent_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < sent_size; i++)
        sub_sum += sub_a[i];

    MPI_Reduce(&sub_sum, &sent_vec_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Reduce sum: " << sent_vec_sum << ", Elapsed time: "<< MPI_Wtime() - start_time << endl;
        delete[] a;
    }

    MPI_Finalize();
    return 0;
}