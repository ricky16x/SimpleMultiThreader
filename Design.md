# SimpleMultiThreader Design Document

## Project Overview

## Team Members & Contribution Summary
- Karan Kumar - Design Document, parallel_for()
- Rounak Dey - thread1D(), thread2d()

## File Descriptions
- `.gitignore`: Specifies files and directories to be ignored by Git.
- `Design.md`: Contains the design document for the project.
- `Makefile`: Contains the build instructions for the project.
- `matrix.cpp`: Implements the matrix multiplication using multi-threading.
- `README.md`: Provides a brief overview for the assignment.
- `simple-multithreader.h`: Contains the multi-threading utility functions and structures.
- `vector.cpp`: Implements the vector addition using multi-threading.

## Design and Implementation Details

### Overview
The SimpleMultiThreader project demonstrates the use of multi-threading to perform matrix multiplication and vector addition in parallel. The project uses Pthreads to create multiple threads and execute the tasks concurrently.

### Matrix Multiplication (`matrix.cpp`)
1. **Initialization**:
   - The number of threads (`numThread`) and the size of the matrices (`size`) are initialized based on command-line arguments.
   - Three matrices `A`, `B`, and `C` are dynamically allocated.

2. **Matrix Allocation and Initialization**:
   - The `parallel_for` function is used to allocate and initialize the matrices `A` and `B` with 1s and `C` with 0s in parallel.

3. **Matrix Multiplication**:
   - The `parallel_for` function is used again to perform the matrix multiplication in parallel. Each thread computes a portion of the result matrix `C`.

4. **Verification**:
   - The result matrix `C` is verified to ensure that each element is equal to the size of the matrices.

5. **Cleanup**:
   - The allocated memory for matrices `A`, `B`, and `C` is cleaned up using the `parallel_for` function.

### Vector Addition (`vector.cpp`)
1. **Initialization**:
   - The number of threads (`numThread`) and the size of the vectors (`size`) are initialized based on command-line arguments.
   - Three vectors `A`, `B`, and `C` are dynamically allocated.

2. **Vector Initialization**:
   - The vectors `A` and `B` are initialized with 1s and `C` with 0s.

3. **Vector Addition**:
   - The `parallel_for` function is used to perform the vector addition in parallel. Each thread computes a portion of the result vector `C`.

4. **Verification**:
   - The result vector `C` is verified to ensure that each element is equal to 2.

5. **Cleanup**:
   - The allocated memory for vectors `A`, `B`, and `C` is cleaned up.

### Multi-threading Utility (`simple-multithreader.h`)
1. **Thread Data Structures**:
   - `ThreadData1D` and `ThreadData2D` structures are defined to hold the data required for 1D and 2D parallel loops, respectively.

2. **Thread Functions**:
   - `thread1D` and `thread2D` functions are defined to execute the lambda functions in parallel for 1D and 2D loops, respectively.

3. **Parallel For Functions**:
   - `parallel_for` functions are defined to divide the work among multiple threads and execute the lambda functions in parallel. The functions also measure and print the execution time.

4. **Demonstration Function**:
   - `demonstration` function is provided to demonstrate how to pass and execute lambda functions.

### Conclusion
The SimpleMultiThreader project effectively demonstrates the use of multi-threading to perform computationally intensive tasks in parallel, thereby improving performance and efficiency. The project showcases the use of modern C++ features such as lambda functions and the Pthreads library for multi-threading.
