#include <cstdio>
//#include "mpi.h"
#import "/usr/local/Cellar/mpich/3.4.3/include/mpi.h"

int main(int argc, char **argv) {
    int r, s_m = 42;
    MPI_Init(&argc, &argv);
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Status status;
    MPI_Comm intercomm;
    MPI_Open_port(MPI_INFO_NULL, port_name);
    printf("portname: %s\n", port_name);
    printf("Wait for the client\n");
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
    printf("Client connected\n");
    MPI_Send(&s_m, 1, MPI_INT, 0, 0, intercomm);
    printf("Server sent %d\n", s_m);
    MPI_Recv(&s_m, 1, MPI_INT, 0, 0, intercomm, &status);
    printf("Server receive %d\n", s_m);
    MPI_Comm_free(&intercomm);
    MPI_Close_port(port_name);
    MPI_Finalize();
    return 0;
}