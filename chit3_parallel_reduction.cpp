#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <omp.h>

using namespace std;

int main() {
    const int SIZE = 1000000;
    vector<int> arr(SIZE);

    // Initialize random data
    srand(time(0));
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = rand() % 10000;
    }

    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    long long sum = 0;
    double avg = 0.0;

    double start = omp_get_wtime();

    #pragma omp parallel for reduction(min:minVal) reduction(max:maxVal) reduction(+:sum)
    for (int i = 0; i < SIZE; ++i) {
        if (arr[i] < minVal)
            minVal = arr[i];
        if (arr[i] > maxVal)
            maxVal = arr[i];
        sum += arr[i];
    }

    avg = static_cast<double>(sum) / SIZE;

    double end = omp_get_wtime();

    // Display results
    cout << "Parallel Reduction Results:\n";
    cout << "Min: " << minVal << endl;
    cout << "Max: " << maxVal << endl;
    cout << "Sum: " << sum << endl;
    cout << "Average: " << avg << endl;
    cout << "Time taken: " << end - start << " seconds" << endl;

    return 0;
}
