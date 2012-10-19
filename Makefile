#
# Build parallel test case programs.
#

CC=gcc
MPICC=mpicc.openmpi
CFLAGS=-c -Wall
LDFLAGS=
OMP_CFLAGS=-fopenmp
OMP_LDFLAGS=-lgomp

.PHONY: clean all

all: mm_omp.exe mm_serial.exe mm_mpi.exe

mm_serial.exe: mm_serial.o util.o
	$(CC) $(LDFLAGS) mm_serial.o util.o -o mm_serial.exe

mm_omp.exe: mm_omp.o util.o
	$(CC) $(LDFLAGS) $(OMP_LDFLAGS) mm_omp.o util.o -o mm_omp.exe

mm_mpi.exe: mm_mpi.o util.o
	$(MPICC) $(LDFLAGS) mm_mpi.o util.o -o mm_mpi.exe

mm_serial.o: mm_serial.c
	$(CC) $(CFLAGS) mm_serial.c

mm_omp.o: mm_omp.c
	$(CC) $(CFLAGS) $(OMP_CFLAGS) mm_omp.c

mm_mpi.o: mm_mpi.c
	$(MPICC) $(CFLAGS) mm_mpi.c

util.o: util.c
	$(CC) $(CFLAGS) util.c

clean:
	rm -f *.o *.exe

