/***********************************************************************
 * File: util.c
 * Description:
 *   Utilities for parallel test cases.
 * Author: Andrew Dawson
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*
 * Dynamic allocation of contiguous 2D arrays.
 */
double **array2d(int n, int m)
{
    int i;
    double **array;
    array = (double **) calloc(n, sizeof(double *));
    array[0] = (double *) calloc(n * m, sizeof(double));
    for (i=0; i<n; i++) {
        array[i] = array[0] + i * m;
    }
    return (array);
}

/*
 * Print the values of a 2D array to the screen.
 */
void print_double_array(int nrows, int ncols, double **array)
{
    int i, j;
    for (i=0; i<nrows; i++) {
        for (j=0; j<ncols; j++) {
            printf("%4.1f ", array[i][j]);
        }
        printf("\n");
    }
    return;
}

