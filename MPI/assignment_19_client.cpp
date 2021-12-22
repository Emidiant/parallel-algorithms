#include <iostream>
//#include "mpi.h"
#import "/usr/local/Cellar/mpich/3.4.3/include/mpi.h"

int main(int argc, char **argv) {
    int rank, s_m = 25;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Comm intercomm;
    strcpy(port_name, argv[1]);
    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Server connection\n");
    MPI_Send(&s_m, 1, MPI_INT, 0, 0, intercomm);
    printf("Client sent %d\n", s_m);
    MPI_Recv(&s_m, 1, MPI_INT, 0, 0, intercomm, &status);
    printf("Client receive %d\n", s_m);

    MPI_Finalize();
    return 0;
}