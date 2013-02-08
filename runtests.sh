#!/bin/bash
#
# Run test jobs and report timing information.
#

TIME="/usr/bin/time -f '%E real, %U user %S sys'"
NPROCESSES=3
export OMP_NUM_THREADS=$NPROCESSES

eval "$TIME ./mm_serial.exe"
eval "$TIME ./mm_omp.exe"
eval "$TIME mpirun -n $NPROCESSES mm_mpi.exe"

