#include <cstdio>
#include <iostream>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"

int main(int argc, char **argv) {
    int size, rank1, rank2, number_processes;
    MPI_Status status;
    MPI_Comm intercomm;
    char slave[14] = "slave_18.o";
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_spawn(slave, MPI_ARGV_NULL, 3, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm, MPI_ERRCODES_IGNORE);
    MPI_Recv(&rank1, 1, MPI_INT, 0, 0, intercomm, &status);
    MPI_Recv(&rank2, 1, MPI_INT, 1, 1, intercomm, &status);

    printf("Slaves %d and %d are working\n", rank1, rank2);

    MPI_Recv(&number_processes, 1, MPI_INT, 2, 2, intercomm, &status);
    printf("Number of processes: %d\n", number_processes);
    MPI_Finalize();
    return 0;
}