/******************************************************************************
 * File: mm_serial.c
 * Description:
 *   Matrix multiplication (serial version).
 * Author: Andrew Dawson
 * Credit: Based on code by Blaise Barney
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#define NRA 5000    /* number of rows in matrix A */
#define NCA 1000    /* number of columns in matrix A */
#define NCB 1000    /* number of columns in matrix B */

/* whether or not to print the result to the screen, should be off for large
 * matrix dimensions, can set using -DPRINT=1 at compile time */
#ifndef PRINT
#define PRINT 0
#endif

int main(int argc, char *argv[])
{
    int i, j, k;    /* loop indices */
    
    /* matrices to be multiplied and the resut */
    double **a, **b, **c;

    /* allocate memory */
    a = array2d(NRA, NCA);
    b = array2d(NCA, NCB);
    c = array2d(NRA, NCB);

    printf("-----------------------------\n");
    printf("matrix multiply test (serial)\n");
    printf("-----------------------------\n");
    printf("matrix dimensions: a[%d][%d] b[%d][%d] c[%d][%d]\n",
            NRA, NCA, NCA, NCB, NRA, NCB);

    printf("-> initializing matrices\n");
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
    for (i=0; i<NRA; i++) {
        for (j=0; j<NCB; j++) {
            c[i][j] = 0;
        }
    }

    printf("-> performing matrix multiply\n");
    for (i=0; i<NRA; i++) {
        for (j=0; j<NCB; j++) {
            for (k=0; k<NCA; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    if (PRINT) {
        print_double_array(NRA, NCB, c);
    }

    /* deallocate memory */
    free(a);
    free(b);
    free(c);

    printf("-> complete\n");
    return (0);
}

