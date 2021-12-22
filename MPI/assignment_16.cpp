#include <cstdio>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"

int main(int argc, char **argv) {
    int rank, size, rank1;
    MPI_Init(&argc, &argv);
    MPI_Comm comm_revs;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_split(MPI_COMM_WORLD, (rank % 2), size - rank, &comm_revs);
//    MPI_Comm_split(MPI_COMM_WORLD, (rank % 3), size - rank, &comm_revs);
//    MPI_Comm_split(MPI_COMM_WORLD, 1, size - rank, &comm_revs);
    MPI_Comm_rank(comm_revs, &rank1);

    printf("Rank: %d, Rank1: %d\n", rank, rank1);
    MPI_Comm_free(&comm_revs);
    MPI_Finalize();
}