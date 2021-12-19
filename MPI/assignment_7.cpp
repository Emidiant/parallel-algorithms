#include <iostream>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"

#define VECTOR_SIZE 1000000
using namespace std;

int count_element(int n, int i) {
    if (i != n - 1) {
        return VECTOR_SIZE / (n - 1);
    } else {
        return VECTOR_SIZE / (n - 1) + VECTOR_SIZE % (n - 1);
    }
}

int main(int argc, char* argv[]) {
    int total_sum_vec = 0;
    int rank, n, i;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &n);
    int message_a[VECTOR_SIZE/(n-1)+1];
    int message_b[VECTOR_SIZE/(n-1)+1];
    int step = VECTOR_SIZE / (n - 1);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        int* a = new int[VECTOR_SIZE];
        int* b = new int[VECTOR_SIZE];
        for (i = 0; i < VECTOR_SIZE; i++){
            a[i] = 1;
            b[i] = 1;
        }
        int sum_vec = 0;
        for (i = 1; i < n; i++) {
            int count = count_element(n, i);
            MPI_Send(&a[(i - 1) * step], count, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[(i - 1) * step], count, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        for (i = 1; i < n; i++) {
            MPI_Recv(&sum_vec, 1, MPI_INT, i, 2,MPI_COMM_WORLD, &status);
            total_sum_vec += sum_vec;
        }
        cout << "Scalar product for two vectors = " << total_sum_vec << endl;
    }
    else {
        int count = count_element(n, rank);
        int sum_part_vec = 0;
        MPI_Recv(&message_a[0], count, MPI_INT, MPI_ANY_SOURCE, 0,MPI_COMM_WORLD, &status);
        MPI_Recv(&message_b[0], count, MPI_INT, MPI_ANY_SOURCE, 1,MPI_COMM_WORLD, &status);
        for (i = 0; i < count; i++)
            sum_part_vec += message_a[i] * message_b[i];
        MPI_Send(&sum_part_vec, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}