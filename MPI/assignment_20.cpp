#include <cstdio>
#include <fstream>
//#include "mpi.h"
#import "/usr/local/Cellar/mpich/3.4.3/include/mpi.h"
using namespace std;

#define BUFSIZE 100
int main(int argc, char **argv) {
    int bufsize, num, sum;
    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_File fh;
    char buf[BUFSIZE];
    ofstream f1("file.txt");
    f1 << "Hello, world \nThere is very little left. We will definitely survive.";
    f1.close();

    MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY | MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &fh);
    MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
    sum = 0;
    do {
        MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
        MPI_Get_count(&status, MPI_CHAR, &num);
        printf("buf=%s\n", buf);
        sum += num;
    } while (num >= BUFSIZE);
    MPI_File_close(&fh);
    printf("Sum of symbols: %d\n", sum);
    MPI_Finalize();
}