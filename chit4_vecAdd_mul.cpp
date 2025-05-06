#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>
using namespace std;

// ---------- Vector Functions ----------
void fill_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
    }
}

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

void vector_add_omp(int *a, int *b, int *result, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        result[i] = a[i] + b[i];
    }
}

// ---------- Matrix Functions ----------
void initialize_matrix(int *array, int rows, int cols){
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            array[i*cols + j] = rand() % 10;
        }
    }
}

void print_matrix(int *array, int rows, int cols){
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            cout << array[i*cols + j] << " ";
        }
        cout << endl;
    }
}

void matrix_multiplication_omp(int *a, int *b, int *c, int common, int c_rows, int c_cols){
    #pragma omp parallel for collapse(2)
    for(int i = 0; i < c_rows; i++){
        for(int j = 0; j < c_cols; j++){
            int sum = 0;
            for(int k = 0; k < common; k++){
                sum += a[i*common + k] * b[k*c_cols + j];
            }
            c[i*c_cols + j] = sum;
        }
    }
}

// ---------- Main Program ----------
int main() {
    srand(time(0));
    int choice;
    cout << "Select operation:\n1. Vector Addition\n2. Matrix Multiplication\nEnter choice: ";
    cin >> choice;

    if (choice == 1) {
        int size;
        cout << "Enter size of vectors: ";
        cin >> size;

        int *arr1 = new int[size];
        int *arr2 = new int[size];
        int *result = new int[size];

        fill_array(arr1, size);
        cout << "Array 1: ";
        print_array(arr1, size);

        fill_array(arr2, size);
        cout << "Array 2: ";
        print_array(arr2, size);

        double start_time = omp_get_wtime();
        vector_add_omp(arr1, arr2, result, size);
        double end_time = omp_get_wtime();

        cout << "Result (OpenMP): ";
        print_array(result, size);
        cout << "Elapsed Time = " << (end_time - start_time) * 1000 << " milliseconds" << endl;

        delete[] arr1;
        delete[] arr2;
        delete[] result;
    }
    else if (choice == 2) {
        int A_rows, A_cols, B_cols;
        cout << "Enter number of rows for Matrix A: ";
        cin >> A_rows;
        cout << "Enter number of columns for Matrix A: ";
        cin >> A_cols;

        cout << "Enter number of columns for Matrix B: ";
        cin >> B_cols;

        int B_rows = A_cols;
        int C_rows = A_rows;
        int C_cols = B_cols;

        int *A = new int[A_rows * A_cols];
        int *B = new int[B_rows * B_cols];
        int *C = new int[C_rows * C_cols];

        initialize_matrix(A, A_rows, A_cols);
        cout << "Matrix A:\n";
        print_matrix(A, A_rows, A_cols);

        initialize_matrix(B, B_rows, B_cols);
        cout << "Matrix B:\n";
        print_matrix(B, B_rows, B_cols);

        double start = omp_get_wtime();
        matrix_multiplication_omp(A, B, C, A_cols, C_rows, C_cols);
        double end = omp_get_wtime();

        cout << "Result (OpenMP):\n";
        print_matrix(C, C_rows, C_cols);
        cout << "Elapsed Time (OpenMP) = " << (end - start) * 1000 << " milliseconds" << endl;

        delete[] A;
        delete[] B;
        delete[] C;
    }
    else {
        cout << "Invalid choice." << endl;
    }

    return 0;
}
