#include <iostream>
#include<chrono>
using namespace std::chrono;
using namespace std;

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++)
        cout << arr[i] << " ";
    cout << endl;
}

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

void OddEvenSort(int* a, int n)
{
    int phase, i, tmp;
    for (phase = 0; phase < n; phase++)
    {
        if (phase % 2 == 0)
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
    auto start = high_resolution_clock::now();
    OddEvenSort(arr, size);
    auto stop = high_resolution_clock::now();

    //cout << "Sorted array: \n";
    //printArray(arr, size);
    auto duration = duration_cast<microseconds>(stop - start);

    delete[] arr;
    cout << "\n\n time:" << duration.count();
    return 0;
}