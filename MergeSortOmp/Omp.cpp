#include <iostream>
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<chrono>
using namespace std::chrono;
using namespace std;

void generate_list(int* x, int n) {
    int i, j, t;
    for (i = 0; i < n; i++)
        x[i] = i;
    for (i = 0; i < n; i++) {
        j = rand() % n;
        t = x[i];
        x[i] = x[j];
        x[j] = t;
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

void OddEvenSort(int *a, int n)
{
    int phase, i, tmp;
#pragma omp parallel default(none) shared(a, n) private(i,tmp, phase) 
    for (phase = 0; phase < n; phase++)
    {
        if(phase%2 == 0) 
#pragma omp for
            for (i = 1; i < n; i += 2)
            {
                if (a[i - 1] > a[i])
                {
                    tmp = a[i - 1];
                    a[i - 1] = a[i];
                    a[i] = tmp;
                }
            }
        else
#pragma omp for
            for (i = 1; i < n - 1; i += 2)
            {
                if (a[i] > a[i + 1])
                {
                    tmp = a[i + 1];
                    a[i + 1] = a[i];
                    a[i] = tmp;
                }
            }
    }
}

int main() {
    int size = 100000;
    int* arr = new int[size];
    generate_list(arr, size);

    //printArray(arr, size);
    omp_set_num_threads(32);

    auto start = high_resolution_clock::now();
    OddEvenSort(arr, size);
    auto stop = high_resolution_clock::now();

    //cout << "Sorted array: \n";
    //printArray(arr, size);

    auto duration = duration_cast<microseconds>(stop - start);
    cout << "\n\n time:" << duration.count();
    return 0;
}