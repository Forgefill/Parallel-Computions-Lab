#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
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

void PartialOddEvenSort(int* a, int start, int end)
{
    int n = end - start + 1;
    if (n == 1) return;

    int phase, i, tmp, firstOdd;

    if (start % 2 == 0) firstOdd = start + 1;
    else firstOdd = start;

    for (phase = 0; phase < n; phase++)
    {
        if (phase % 2 == 0)
        {

            for (i = firstOdd; i < n; i += 2)
            {
                if (a[i - 1] > a[i])
                {
                    tmp = a[i - 1];
                    a[i - 1] = a[i];
                    a[i] = tmp;
                }
            }
        }
        else
        {
            for (i = firstOdd; i < n - 1; i += 2)
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
}

void l2g(int a[], int b[], int size) {
    int i;
    for (i = 0; i < size; i++)
        a[i] = b[i];
}

void p2a(int a[], int* b, int size) {
    int i;
    for (i = 0; i < size; i++) {
        a[i] = b[i];
    }
}

int* mergeArrays(int a[], int b[], int n, int m) {
    int size = n + m;
    int* c = new int[size];
    int i = 0, j = 0, k = 0;
    while (i <= n - 1 && j <= m - 1) {
        if (a[i] <= b[j]) {
            c[k++] = a[i++];
        }
        else {
            c[k++] = b[j++];
        }
    }
    while (i <= n - 1) {
        c[k++] = a[i++];
    }
    while (j <= m - 1) {
        c[k++] = b[j++];
    }
    return c;
}

int main(int argc, char* argv[]) {
    int NUM = atof(argv[1]);;
    int i, a_size = NUM, local_size;
    int numtasks, rank, dest, source, rc, count, tag = 1, j;
    int* a = new int[NUM];
    int* global = new int[NUM];
    int* comp;
    MPI_Status Stat;
    MPI_Request req;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int* local = new int[(NUM / numtasks)]; //Local array for every process

    generate_list(a, NUM); //Setup array with random numbers

    //Scatter and split array evenly for each process
    MPI_Scatter(a, NUM / numtasks, MPI_INT, local, NUM / numtasks, MPI_INT, 0, MPI_COMM_WORLD);
    local_size = NUM / numtasks;

    if (rank == 0) {//Parent Process
        
        double time_spent;
        auto start = high_resolution_clock::now();
        PartialOddEvenSort(local, 0, local_size - 1); //Sequential Odd-Even sort

        l2g(global, local, local_size); //Push sorted local array to global array
        int recv_size = local_size;
        int* buff = new int[recv_size];
        for (int j = 0; j < numtasks - 1; j++) {

            MPI_Recv(buff, recv_size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &Stat); //Receive sorted array from child process

            comp = mergeArrays(global, buff, local_size, recv_size); //Merge received array and global array together 
            local_size += recv_size;
            p2a(global, comp, local_size); //Pointer to Array
        }
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(end - start);

        cout << "time: " << duration.count() << endl;
    }
    else {//Child process
        PartialOddEvenSort(local, 0, local_size - 1);   //Sequential mergesort the given array from scatter
        MPI_Send(local, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD);    //send the sorted array to the parent process
    }
    MPI_Finalize();
}
