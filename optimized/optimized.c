#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// 24 buckets
// num variavel de threads
#define dim 2400000
#define MAX_INT 2147483647
struct bucket
{
    int numElements;
    int index; // [start : numElements)
    int start; // starting point in B array
    int offset;
};

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

int main(int argc, char *argv[])
{

    int *A, *B, *temp;
    int i, load, numThreads, workload, b_index;
    struct bucket *buckets; // array of buckets
    double t1;              // Timing variable
    float total;            // total time

    int *tmp;
    A = (int *)malloc(sizeof(int) * dim);
    B = (int *)malloc(sizeof(int) * dim);

    for (i = 0; i < dim; i++)
    {
        A[i] = random() % dim;
    }

    if (dim <= 40)
    {
        printf("Unsorted data \n");
        for (i = 0; i < dim; i++)
        {
            printf("%d ", A[i]);
        }
        printf("\n");
    }

    int threadsParam;
    if (argv[1] == 0)
        threadsParam = 1;
    else
        threadsParam = atoi(argv[1]);

    numThreads = threadsParam;
    omp_set_num_threads(numThreads);
    int numBuckets = numThreads;

    // global buckets
    int global_n_elem[numBuckets];            // number of elements in each bucket
    int global_starting_position[numBuckets]; // starting position in A for each bucket
    memset(global_n_elem, 0, sizeof(int) * numBuckets);
    memset(global_starting_position, 0, sizeof(int) * numBuckets);

    load = dim / numBuckets;

    // local buckets, n_buckets for each thread
    buckets = (struct bucket *)calloc(numBuckets * numThreads, sizeof(struct bucket));

    // ****************************
    // Starting the main algorithm
    // ****************************

    t1 = omp_get_wtime();
#pragma omp parallel
    {
#pragma omp master
        {
            printf("Using %d threads\n", omp_get_num_threads());
        }

        numThreads = omp_get_num_threads();

        int j, k;
        int local_index;       // [0 : n_buckets)
        int real_bucket_index; // [0 : n_buckets * num_threads)
        int id = omp_get_thread_num();
        int prevoius_index;

#pragma omp for private(i, local_index)
        for (i = 0; i < dim; i++)
        {
            local_index = A[i] / load;
            real_bucket_index = local_index + id * numBuckets;
            buckets[real_bucket_index].numElements++;
        }

        int local_sum = 0;
        for (j = id; j < numBuckets * numThreads; j = j + numThreads)
        {
            local_sum += buckets[j].numElements;
        }
        global_n_elem[id] = local_sum;

#pragma omp barrier

#pragma omp master
        {
            for (j = 1; j < numBuckets; j++)
            {
                global_starting_position[j] = global_starting_position[j - 1] + global_n_elem[j - 1];
                buckets[j].start = buckets[j - 1].start + global_n_elem[j - 1];
                buckets[j].index = buckets[j - 1].index + global_n_elem[j - 1];
            }
        }

#pragma omp barrier
        for (j = id + numBuckets; j < numBuckets * numThreads; j = j + numThreads)
        {
            int prevoius_index = j - numBuckets;
            buckets[j].start = buckets[prevoius_index].start + buckets[prevoius_index].numElements;
            buckets[j].index = buckets[prevoius_index].index + buckets[prevoius_index].numElements;
        }
#pragma omp barrier

#pragma omp for private(i, b_index)
        for (i = 0; i < dim; i++)
        {
            j = A[i] / load;
            k = j + id * numBuckets;
            b_index = buckets[k].index++;
            B[b_index] = A[i];
        }

#pragma omp barrier

#pragma omp for private(i)
        for (i = 0; i < numBuckets; i++)
            bubbleSort(B + global_starting_position[i], global_n_elem[i]);
    }
    total = omp_get_wtime() - t1;
    tmp = A;
    A = B;
    B = tmp;

    if (dim <= 40)
    {
        for (i = 0; i < dim; i++)
        {
            printf("%d ", A[i]);
        }
        printf("\n");
    }
    printf("Sorting %d elements took %f seconds\n", dim, total);

    int sorted = 1;
    for (i = 0; i < dim - 1; i++)
    {
        if (A[i] > A[i + 1])
            sorted = 0;
    }
    if (!sorted)
        printf("The data is not sorted!!!\n");
    else
    {
        printf("The data is sorted!\n");
    }
}
