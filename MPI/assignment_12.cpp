#include <iostream>
//#include "mpi.h"
#import "/usr/local/Cellar/open-mpi/4.1.2/include/mpi.h"
using namespace std;

int main(int argc, char **argv) {
    int rank, size, prev, next;
    int rbuf[2], sbuf[2];
    int completed = 0;
    MPI_Init(&argc, &argv);
    MPI_Request reqs[4];
    MPI_Status stats[4];
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    prev = rank - 1;
    next = rank + 1;
    if (rank == 0)
        prev = size - 1;
    if (rank == size - 1)
        next = 0;
    MPI_Recv_init(&rbuf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &reqs[0]);
    MPI_Recv_init(&rbuf[1], 1, MPI_INT, next, 6, MPI_COMM_WORLD, &reqs[1]);
    MPI_Send_init(&sbuf[0], 1, MPI_INT, prev, 6, MPI_COMM_WORLD, &reqs[2]);
    MPI_Send_init(&sbuf[1], 1, MPI_INT, next, 5, MPI_COMM_WORLD, &reqs[3]);
    for (int & i : sbuf)
        i = rank;
    MPI_Startall(4, reqs);
    for (int i = 0; i < 4; i++)
        MPI_Wait(&reqs[i], &stats[i]);

    MPI_Testall(4, reqs, &completed, stats);
    if (completed)
        printf("Rank: %d, Prev: %d, Next: %d\n", rank, rbuf[0], rbuf[1]);

    for (auto & req : reqs)
        MPI_Request_free(&req);

    MPI_Finalize();
}