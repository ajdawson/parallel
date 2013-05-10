#!/bin/bash
#
# Run test jobs and report timing information.
#

# build the executables
make

# determine the number of processes (MPI workers or OpenMP threads)
# used for the tests
NPROCESSES=3
export OMP_NUM_THREADS=$NPROCESSES

# define the timing command
TIME="/usr/bin/time -f '%E real, %U user %S sys'"

# evaluate each program in turn
eval "$TIME ./mm_serial.exe"
eval "$TIME ./mm_omp.exe"
eval "$TIME mpirun -n $NPROCESSES mm_mpi.exe"

