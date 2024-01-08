# Parallel Bucket Sort

## Introduction:

Parallel Bucket Sort is a project aimed at comparing the performance of sequential, parallel, and optimized versions of the Bucket Sort algorithm. Bucket Sort is a sorting algorithm that divides the input array into a number of buckets, where each bucket is then sorted individually. The sequential version serves as a baseline, while the parallel and optimized versions aim to enhance the sorting process through concurrent execution and optimization strategies.

## Installation:
Clone the repository:
```bash
git clone https://github.com/rcampos27/parallel-bucket-sort.git
cd parallel-bucket-sort
```

Compile the code:

```bash
gcc -fopenmp main.c -o parallel_bucket_sort
```
Run the program:

```bash
./parallel_bucket_sort [num_threads]
```

## Usage:
The main.c file contains the main algorithm for the bucket sort. The program takes an optional argument num_threads to specify the number of threads for parallel execution.

## Components:
**Sequential Bucket Sort:**
The main function in main.c implements the sequential bucket sort algorithm. It utilizes a basic bucketing strategy and bubble sort for each bucket.

**Parallel Bucket Sort:**
An extension of the sequential version, the parallel bucket sort employs OpenMP to parallelize the bucket sorting process. The input array is distributed among multiple threads to enhance sorting efficiency.

**Optimized Bucket Sort:**
Building upon the parallel version, the optimized bucket sort further refines the algorithm. It optimizes memory access patterns, reduces contention, and enhances overall performance.

## How it Works:
**Initialization:** An array A of random integers is generated.

**Bucket Allocation:** The array A is divided into buckets based on the value of each element and the number of threads.

**Parallel Sorting:** Each thread concurrently sorts its assigned buckets using bubble sort.

**Global Sorting:** The sorted buckets are merged to reconstruct the sorted array.

**Performance Metrics:** The time taken for sorting and the correctness of the result are measured and reported.

The project provides insights into the impact of parallelization and optimization on the performance of the Bucket Sort algorithm.
