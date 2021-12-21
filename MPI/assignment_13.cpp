#include <cstdlib>
#include <iostream>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"

using namespace std;

int main(int argc, char **argv) {
    int rank, size, n=500;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int **C = new int*[n];
    int **A = new int*[n];
    int **B = new int*[n];
    for (int i = 0; i < n; i++) {
        A[i] = new int[n];
        B[i] = new int[n];
        C[i] = new int[n];
        for (int j = 0; j < n; j++){
            A[i][j] = rand() % (2 * n);
            B[i][j] = rand() % (2 * n);
            C[i][j] = 0;
        }
    }
    double start_time[size];
    double total_time[size];
    for (int i = 0; i < size; i++)
        total_time[i] = 0;
    MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization

    start_time[rank] = MPI_Wtime();
    for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
            for (int j = 0; j < n; j++)
                C[i][j] += A[i][k] * B[k][j];
    total_time[rank] += MPI_Wtime() - start_time[rank];

    printf("Rank: %d, Elapsed time: %f\n", rank, total_time[rank]);
    if (n == 3){
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                cout << C[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    MPI_Finalize();
}