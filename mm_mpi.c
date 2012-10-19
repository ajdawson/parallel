/******************************************************************************
 * File: mm_mpi.c
 * Description:
 *   Matrix multiplication (MPI version).
 * Author: Andrew Dawson
 * Credit: Based on code by Blaise Barney
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "util.h"

#define NRA 5000    /* number of rows in matrix A */
#define NCA 1000    /* number of columns in matrix A */
#define NCB 1000    /* number of columns in matrix B */

/* whether or not to print the result to the screen, should be off for large
 * matrix dimensions, can set using -DPRINT=1 at compile time */
#ifndef PRINT
#define PRINT 0
#endif

#define MASTER 0         /* task id of first task */
#define FROM_MASTER 1    /* message type setting */
#define FROM_WORKER 2    /* message type setting */

int main (int argc, char *argv[])
{
    int i, j, k;    /* loop indices */
    
    /* matrices to be multiplied and the resut */
    double **a, **b, **c;

    int numtasks,              /* number of tasks in partition */
        taskid,                /* a task identifier */
        numworkers,            /* number of worker tasks */
        source,                /* task id of message source */
        dest,                  /* task id of message destination */
        mtype,                 /* message type */
        rows,                  /* rows of matrix A sent to each worker */
        averow, extra, offset, /* used to determine rows sent to each worker */
        rc;

    MPI_Status status;
    
    /* allocate memory */
    a = array2d(NRA, NCA);
    b = array2d(NCA, NCB);
    c = array2d(NRA, NCB);

    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    if (numtasks < 2 ) {
        printf("x> at least two MPI tasks are required, %d available\n",
                numtasks);
        rc = 1;
        MPI_Abort(MPI_COMM_WORLD, rc);
        exit(1);
    }
    numworkers = numtasks-1;

    /*******************************************************************
     * work for the master task
     ******************************************************************/
    if (taskid == MASTER) {
        printf("-----------------------------------------\n");
        printf("matrix multiply test (parallel %d threads with MPI)\n", numtasks);
        printf("--------------------------------------------------\n");
        printf("matrix dimensions: a[%d][%d] b[%d][%d] c[%d][%d]\n",
                NRA, NCA, NCA, NCB, NRA, NCB);
        printf("-> initializing matrices\n");

        /* initialize the input arrays in serial (unlike OpenMP) */
        for (i=0; i<NRA; i++) {
            for (j=0; j<NCA; j++) {
                a[i][j] = i + j;
            }
        }
        for (i=0; i<NCA; i++) {
            for (j=0; j<NCB; j++) {
                b[i][j] = i * j;
            }
        }

        /* send matrix data to worker tasks */
        printf("-> performing matrix multiply (sending tasks to workers)...");
        averow = NRA / numworkers;
        extra = NRA % numworkers;
        offset = 0;
        mtype = FROM_MASTER;
        for (dest=1; dest<=numworkers; dest++) {
            /* add <extra> extra rows to make sure all rows are assigned */
            rows = (dest <= extra) ? averow+1 : averow;

            MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
            MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,
                    MPI_COMM_WORLD);
            MPI_Send(&b[0][0], NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
            offset += rows;
        }

        /* recieve results from worker tasks */
        mtype = FROM_WORKER;
        for (i=1; i<=numworkers; i++) {
            source = i;
            MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD,
                    &status);
            MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD,
                    &status);
            MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, mtype, 
                    MPI_COMM_WORLD, &status);
        }
        printf(" received results from workers\n");

        if (PRINT) {
            print_double_array(NRA, NCB, c);
        }
    
        printf("-> complete\n");
    }

    /*******************************************************************
     * work for the worker tasks
     ******************************************************************/
    if (taskid > MASTER) {
        mtype = FROM_MASTER;
        MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&a[0][0], rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD,
                &status);
        MPI_Recv(&b[0][0], NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD,
                &status);

        for (k=0; k<NCB; k++) {
            for (i=0; i<rows; i++) {
                c[i][k] = 0.0;
                for (j=0; j<NCA; j++) {
                    c[i][k] = c[i][k] + a[i][j] * b[j][k];
                }
            }
        }

        mtype = FROM_WORKER;
        MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
        MPI_Send(&c[0][0], rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
    }   
 
    /* deallocate memory */
    dealloc_array2d(a);
    dealloc_array2d(b);
    dealloc_array2d(c);

    MPI_Finalize();
    return (0);
}

