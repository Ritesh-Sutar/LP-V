#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Utility to generate random array
void generateArray(vector<int>& arr, int size) {
    arr.resize(size);
    for (int i = 0; i < size; ++i)
        arr[i] = rand() % 10000;
}

// Utility to copy array
vector<int> copyArray(const vector<int>& src) {
    return vector<int>(src);
}

// Sequential Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < n - i - 1; ++j)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// Parallel Bubble Sort
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n; i++) {
        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// Sequential Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right)
        temp[k++] = (arr[i] < arr[j]) ? arr[i++] : arr[j++];
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];

    for (i = left, k = 0; i <= right; ++i, ++k)
        arr[i] = temp[k];
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right, int depth = 0) {
    if (left >= right) return;

    if (depth > 4) { // Limit parallel depth
        mergeSort(arr, left, right);
        return;
    }

    int mid = (left + right) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
        parallelMergeSort(arr, left, mid, depth + 1);

        #pragma omp section
        parallelMergeSort(arr, mid + 1, right, depth + 1);
    }

    merge(arr, left, mid, right);
}

int main() {
    srand(time(0));
    int size = 10000;
    vector<int> baseArray;
    generateArray(baseArray, size);

    // Bubble Sort
    vector<int> arr1 = copyArray(baseArray);
    double start = omp_get_wtime();
    bubbleSort(arr1);
    double end = omp_get_wtime();
    cout << "Sequential Bubble Sort Time: " << end - start << " seconds\n";

    arr1 = copyArray(baseArray);
    start = omp_get_wtime();
    parallelBubbleSort(arr1);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << end - start << " seconds\n";

    // Merge Sort
    arr1 = copyArray(baseArray);
    start = omp_get_wtime();
    mergeSort(arr1, 0, size - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << end - start << " seconds\n";

    arr1 = copyArray(baseArray);
    start = omp_get_wtime();
    parallelMergeSort(arr1, 0, size - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << end - start << " seconds\n";

    return 0;
}
