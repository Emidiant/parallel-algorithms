#include <cstdio>
#include <fstream>
//#include "mpi.h"
#import "/usr/local/Cellar/mpich/3.4.3/include/mpi.h"
using namespace std;
#define BUFSIZE 40

int main(int argc, char **argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_File fh;
    ofstream f1("file.txt");
    f1 << "Hello, world. There is very little left. We will definitely survive."
          "These laboratory works are not difficult, but they take a lot of time due to the quantity.";
    f1.close();
    char buf[BUFSIZE];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY | MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, rank * BUFSIZE, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    MPI_File_read_all(fh, buf, BUFSIZE, MPI_CHAR, MPI_STATUS_IGNORE);
    printf("process %d, buf=%s\n", rank, buf);
    MPI_File_close(&fh);
    MPI_Finalize();
}