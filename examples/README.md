# Examples

Each example has a sequential implementation and also parallel versions using Pthreads, OpenMP, MPI and Cuda.

* [Pi - Calculates pi using an infinite series approach](https://github.com/HPCSys-Lab/HPC-101/tree/main/examples/pi).

## How to compile

To compile the programs, make sure you have a C compiler installed on your system (like gcc, icc, clang, etc). 
We are using the GNU (gcc) compiler, but you can use any other C compiler. In this case, just remember to edit the Makefile. 

Now you can, compile the programs using:

`make`

This will compile all versions (Sequential, Pthread, OpenMP, MPI and Cuda).

Alternatively, you can compile each version separately:

`make sequential`

`make pthread`

`make openmp`

`make mpi`

`make cuda`
