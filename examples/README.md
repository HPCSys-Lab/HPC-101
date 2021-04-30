# Examples

Each example has a sequential implementation and also parallel versions using Pthreads, OpenMP, MPI and Cuda.

* [Pi 1 - Calculates pi using an infinite series approach](https://github.com/HPCSys-Lab/HPC-101/tree/main/examples/pi-arctan).

* [Pi 2 - Calculates pi using integral](https://github.com/HPCSys-Lab/HPC-101/tree/main/examples/pi-integral).

* [Vector addition](https://github.com/HPCSys-Lab/HPC-101/tree/main/examples/vec-add).

* [LU decomposition](https://github.com/HPCSys-Lab/HPC-101/tree/main/examples/lu-decomp).

* [Laplace solver using Jacobi iteration](https://github.com/HPCSys-Lab/HPC-101/tree/main/examples/laplace).

## How to compile

To compile the programs, make sure you have a C compiler installed on your system (like gcc, icc, clang, etc). We are using the GNU (gcc) compiler, but you can use any other C compiler. In this case, just remember to edit the Makefile. You will also need an MPI implementation (like Open MPI, MPICH, Intel MPI, MVAPICH, etc) for the MPI versions. And NVCC for cuda programs.

MacOS systems users will likely need to install llvm and libomp libraries:

`brew install llvm libomp`

For an up to date view of OpenMP compilers and tools see [this](https://www.openmp.org/resources/openmp-compilers-tools/)

Now you can compile the programs using:

`make`

This will compile all versions (Sequential, Pthread, OpenMP, MPI and Cuda).

Alternatively, you can compile each version separately:

`make sequential`

`make pthread`

`make openmp`

`make mpi`

`make cuda`

## How to execute

Each version (sequential and parallel) generates an specific executable file after compilation.

* Sequential: `./prog_seq ARG1 .. ARG_N`.

* Pthread: `./prog_pth ARG1 .. ARG_N`.

* OpenMP: `OMP_NUM_THREADS=T ./prog_omp ARG1 .. ARG_N`, where `T` is the number of threads.

* MPI: `mpirun -n P ./prog_mpi ARG1 .. ARG_N`, where `P` is the number of processes.

* Cuda: `./prog_cuda ARG1 .. ARG_N`.
