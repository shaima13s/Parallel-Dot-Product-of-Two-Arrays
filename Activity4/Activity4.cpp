#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            cout << "Run with 4 processes!" << endl;
        MPI_Finalize();
        return 0;
    }

    int A[16], B[16];

    if (rank == 0) {
        cout << "Array A: ";
        for (int i = 0; i < 16; i++) {
            A[i] = i + 1;
            cout << A[i] << " ";
        }
        cout << endl;

        cout << "Array B: ";
        for (int i = 0; i < 16; i++) {
            B[i] = i + 1;
            cout << B[i] << " ";
        }
        cout << endl;
    }

    int local_A[4], local_B[4];

    MPI_Scatter(A, 4, MPI_INT, local_A, 4, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, 4, MPI_INT, local_B, 4, MPI_INT, 0, MPI_COMM_WORLD);

    int local_dot = 0;
    for (int i = 0; i < 4; i++) {
        local_dot += local_A[i] * local_B[i];
    }

    cout << "Process " << rank << " local dot = " << local_dot << endl;

    int global_dot;
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Final Dot Product = " << global_dot << endl;
    }

    MPI_Finalize();
    return 0;
}