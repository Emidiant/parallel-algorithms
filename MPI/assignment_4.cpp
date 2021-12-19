#include <iostream>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"

/*
    #26
    In the main process, a set of real numbers is given; the number of numbers is equal to the number of slave processes.
    Using the MPI_Send function, send one number to each of the slave processes
    (the first number to process 1, the second to process 2, etc.)
    and output the received numbers in the slave processes.
 */
using namespace std;

int main(int argc, char* argv[]) {
    double time_start, time_finish;
    MPI_Init(&argc, &argv);
    int rank, n, i;
    double message;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    time_start = MPI_Wtime();
    if (rank == 0) {
        double a[n-1];
        for (i = 0; i < n-1; i++){
            a[i] = (double) rand() / RAND_MAX;
            cout << a[i] << " ";
        }
        cout << "\n";
        for (i = 1; i < n; i++) {
            MPI_Send(&a[i-1], 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(&message, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        cout << "Received message: " << message << endl;
    }
    time_finish = MPI_Wtime();
    cout << "process " << rank << ", time = " << time_finish - time_start << endl;
    MPI_Finalize();
    return 0;
}