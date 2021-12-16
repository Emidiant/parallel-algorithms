#include <iostream>
#include "omp.h"

using namespace std;

class Matrix {
    private:
        int **A;
        int sizeN;
        int sizeM;

    public:
        Matrix(int n, int m) {
            this->sizeN = n;
            this->sizeM = m;
            A = new int*[n];
            for (int i = 0; i < n; i++){
                A[i] = new int[m];
                for (int j = 0; j < m; j++)
                    A[i][j] = 0;
            }
        }

        Matrix(int n) {
            this->sizeN = n;
            this->sizeM = n;
            A = new int*[n];
            for (int i = 0; i < n; i++) {
                A[i] = new int[n];
                for (int j = 0; j < n; j++)
                    A[i][j] = rand() % (2 * n);
            }
        }

        ~Matrix() {
            if (A){
                for (int i = 0; i < sizeN; i++)
                    delete[] A[i];
                delete[] A;
            }
        }

        Matrix dot(const Matrix &mat2, const string& order, const int thread){
            Matrix result(this->sizeN, mat2.sizeM);
#pragma omp parallel num_threads(thread)
            {
                if (order == "ijk") {
#pragma omp for
                    for (int i = 0; i < sizeN; i++)
                        for (int j = 0; j < sizeN; j++)
                            for (int k = 0; k < sizeN; k++)
                                result.A[i][j] += A[i][k] * mat2.A[k][j];
                } else if (order == "jki") {
#pragma omp for
                    for (int j = 0; j < sizeN; j++)
                        for (int k = 0; k < sizeN; k++)
                            for (int i = 0; i < sizeN; i++)
                                result.A[i][j] += A[i][k] * mat2.A[k][j];
                } else if (order == "ikj") {
#pragma omp for
                    for (int i = 0; i < sizeN; i++)
                        for (int k = 0; k < sizeN; k++)
                            for (int j = 0; j < sizeN; j++)
                                result.A[i][j] += A[i][k] * mat2.A[k][j];
                }
            }
            return result;
        }

        friend ostream& operator<<(ostream &out, const Matrix &M);
};

ostream& operator<<(ostream &out, const Matrix &M){
    int max_len = 0;
    for (int i = 0; i < M.sizeN; i++) {
        for (int j = 0; j < M.sizeM; j++) {
            int len_element =1;
            int element = M.A[i][j];
            while ((element/=10) > 0)
                len_element++;
            max_len = max(max_len, len_element);
        }
    }
    for (int i = 0; i < M.sizeN; i++){
        for (int j = 0; j < M.sizeM; j++){
            int len_element =1;
            int element = M.A[i][j];
            while ((element/=10) > 0)
                len_element++;
            out<<M.A[i][j];
            for (int k = 0; k < max_len - len_element + 1; k++)
                out<<" ";
        }
        out<<endl;
    }
    return out;
}

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int num_threads = 10;
    Matrix mat1(n);
    Matrix mat2(n);
    double t1;
    string orders[3] = {"jki", "ijk", "ikj"};
    if (n == 5) {
        cout << mat1 << endl;
        cout << mat2 << endl;
        for (auto & order : orders) {
            printf("Order = %s\n", order.c_str());
            cout << mat1.dot(mat2, order, 1);
        }
    }

    for (auto & order : orders){
        printf("Order = %s\n", order.c_str());
        double time = omp_get_wtime();
        mat1.dot(mat2, order, 1);
        t1 = omp_get_wtime() - time;
        for (int thread = 1; thread <= num_threads; thread++) {
            double total_time = 0;
            for (int i = 0; i < 10; i++) {
                time = omp_get_wtime();
                mat1.dot(mat2, order, thread);
                total_time += omp_get_wtime() - time;
            }
            printf("Threads = %d, Avg time = %f, Efficiency time = %f\n", thread, total_time/10, t1/(total_time/10));
        }
    }
    return 0;
}
