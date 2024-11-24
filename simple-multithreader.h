#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>
#include <pthread.h>
#include <vector>
#include <chrono>

void demonstration(std::function<void()> && lambda);
int user_main(int argc, char **argv);
void* thread1D(void* arg);
void* thread2D(void* arg);
void parallel_for(int low, int high, std::function<void(int)>&& lambda, int numThreads);
void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)>&& lambda, int numThreads);


/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}

// 1d struct
struct ThreadData1D {
    int start;
    int end;
    std::function<void(int)>* lambda;
};

// 2d struct
struct ThreadData2D {
    int start1;
    int end1;
    int low2;
    int high2;
    std::function<void(int, int)>* lambda;
};

// 1d thread fxn
void* thread1D(void* arg) {
    ThreadData1D* data = static_cast<ThreadData1D*>(arg);
    for(int i = data->start; i < data->end; i++) {
        (*(data->lambda))(i);
    }
    return nullptr;
}

// 2d thread fxn
void* thread2D(void* arg) {
    ThreadData2D* data = static_cast<ThreadData2D*>(arg);
    for(int i = data->start1; i < data->end1; i++) {
        for(int j = data->low2; j < data->high2; j++) {
            (*(data->lambda))(i, j);
        }
    }
    return nullptr;
}

// parallel_for accepts a C++11 lambda function and runs the loop body (lambda) in
// parallel by using 'numThreads' number of Pthreads to be created by the simple-multithreader
void parallel_for(int low, int high, std::function<void(int)>&& lambda, int numThreads) {
    auto start_time = std::chrono::high_resolution_clock::now();

    
    int range = high - low;
    numThreads = std::min(numThreads, range);

    std::vector<pthread_t> threads(numThreads - 1);  // -1 because main thread will do work too
    std::vector<ThreadData1D> thread_data(numThreads);

    // Calculate chunk size for each thread
    int chunk_size = range / numThreads;
    int remainder = range % numThreads;

    
    int current_low = low;
    for(int i = 0; i < numThreads - 1; i++) {
        int thread_chunk = chunk_size + (i < remainder ? 1 : 0);
        thread_data[i] = {current_low, current_low + thread_chunk, &lambda};
        pthread_create(&threads[i], nullptr, thread1D, &thread_data[i]);
        current_low += thread_chunk;
    }

    // Main thread does its share of work
    thread_data[numThreads - 1] = {current_low, high, &lambda};
    thread1D(&thread_data[numThreads - 1]);

    // Wait for all threads to complete
    for(auto& thread : threads) {
        pthread_join(thread, nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Parallel execution time: " << duration.count() << " microseconds" << std::endl;
}

// This version of parallel_for is for parallelizing two-dimensional for-loops, i.e., an outter for-i loop and
// an inner for-j loop. Loop properties, i.e. low, high are mentioned below for both outter
// and inner for-loops. The suffixes "1" and "2" represents outter and inner loop properties respectively.
void parallel_for(int low1, int high1, int low2, int high2, std::function<void(int, int)>&& lambda, int numThreads) {
    auto start_time = std::chrono::high_resolution_clock::now();

    
    int range = high1 - low1;
    numThreads = std::min(numThreads, range);

    std::vector<pthread_t> threads(numThreads - 1);  // -1 because main thread will do work too
    std::vector<ThreadData2D> thread_data(numThreads);

    // Calculate chunk size for each thread (outer loop only)
    int chunk_size = range / numThreads;
    int remainder = range % numThreads;

    
    int current_low = low1;
    for(int i = 0; i < numThreads - 1; i++) {
        int thread_chunk = chunk_size + (i < remainder ? 1 : 0);
        thread_data[i] = {current_low, current_low + thread_chunk, low2, high2, &lambda};
        pthread_create(&threads[i], nullptr, thread2D, &thread_data[i]);
        current_low += thread_chunk;
    }

    // Main thread does its share of work
    thread_data[numThreads - 1] = {current_low, high1, low2, high2, &lambda};
    thread2D(&thread_data[numThreads - 1]);

    // Wait for all threads to complete
    for(auto& thread : threads) {
        pthread_join(thread, nullptr);
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Parallel execution time: " << duration.count() << " microseconds" << std::endl;
}

int main(int argc, char **argv) {
  /* 
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be 
   * explicity captured if they are used inside lambda.
   */
  int x=5,y=1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/[/*by value*/ x, /*by reference*/ &y](void) {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
 
  auto /*name*/ lambda2 = [/*nothing captured*/]() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

#define main user_main