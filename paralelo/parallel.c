#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// 24 buckets
// num variavel de threads
#define numBuckets 24
#define dim 2400000
#define MAX_INT 2147483647

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

struct bucket
{
    int numElements;
    int start; // starting point in B array
    int index; // [start : numElements)
    int c;
};

int main(int argc, char *argv[])
{

    int *A, *B, *temp;
    int i, w, j, min;
    struct bucket *buckets; // array of buckets
    double start, total;

    int load = dim / numBuckets;

    // printf("Each bucket will have %d elements\n", load);
    A = (int *)malloc(sizeof(int) * dim);
    B = (int *)malloc(sizeof(int) * dim);

    buckets = (struct bucket *)calloc(numBuckets, sizeof(struct bucket));

    for (i = 0; i < dim; i++)
    {
        A[i] = random() % dim;
        B[i] = A[i];
        if (dim < 20)
            printf("%d ", A[i]);
    }
    if (dim < 20)
        printf("\n");

    start = omp_get_wtime();
    int element;

    buckets[0].numElements = load;
    for (i = 1; i < numBuckets; i++)
    {
        buckets[i].start = buckets[i - 1].start + load;
        buckets[i].index = buckets[i - 1].index + load;
        buckets[i].numElements = load;
    }

#pragma omp parallel
    {

#pragma omp for private(i)
        for (i = 0; i < numBuckets; i++)
        {
            // printf("sorting from %d to %d\n", buckets[i].start, buckets[i].start + load);
            bubbleSort(B + buckets[i].start, buckets[i].numElements);
        }
    }

    int pos = 0;
    for (i = 0; i < dim; i++)
    {
        min = MAX_INT;
        pos = 0;

        for (j = 0; j < numBuckets; j++)
        {
            if (buckets[j].index >= buckets[j].start + load)
                continue;

            element = B[buckets[j].index];
            if (element < min)
            {
                min = element;
                pos = j;
            }
        }

        buckets[pos].index++;
        A[i] = min;
    }

    total = omp_get_wtime() - start;
    // for (int i = 0; i < dim; i++)
    // {
    //     printf("%d ", A[i]);
    // }
    // printf("\n");

    printf("Sorting %d elements took %f seconds\n", dim, total);

    int sorted = 1;
    for (i = 0; i < dim - 1; i++)
    {
        if (A[i] > A[i + 1])
            sorted = 0;
    }
    if (!sorted)
        printf("/!\\ -The data is not sorted!!!\n");
    else
        printf(" OK - Data has been sorted!\n");
}
