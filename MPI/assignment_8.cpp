#include <iostream>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"
#define NTIMES 10

using namespace std;

double bandwidth(int rank, int i) {
    MPI_Status status;
    double time_start, time_finish = 0;
    for (int j = 0; j < NTIMES; j++) {
        int* a = new int[i];
        int* b = new int[i];
        if (rank == 0) {
            time_start = MPI_Wtime();
            MPI_Send(&a[0], i, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&a[0], i, MPI_INT, 1, 1,MPI_COMM_WORLD, &status);
            time_finish += MPI_Wtime() - time_start;
        } else if (rank == 1) {
            MPI_Recv(&b[0], i, MPI_INT, 0, 0,MPI_COMM_WORLD, &status);
            MPI_Send(&b[0], i, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }
    return time_finish / NTIMES;
}

int main(int argc, char* argv[]) {
    int rank, n, i;
    double T, R, L;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for (i = 1; i <= 1000000; i *= 10){
        T = bandwidth(rank, i);
        if (rank == 0) {
            L = (double) i * sizeof(i) / 1024 / 1024;
            R = (double) 2 * L / T ;
            printf("N: %d, Message size: %f Mb, Bandwidth: %f Mb/s, Avg time: %f\n", i, L, R, T/2);
        }
    }
    T = bandwidth(rank, 0);
    if (rank == 0) {
        double s = T / 2;
        printf("Latency: %f\n", s);
    }
    MPI_Finalize();
    return 0;
}