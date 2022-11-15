#include "mpi.h"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>

using namespace std;

float updateSolution(int** matrix, int *b, float *x, float* xOld, int rowCount, int myrank, int numranks, int N);
double time1();
double time2();

int main(int argc, char *argv[]) {
    
    int N, M;
    int numranks, myrank, rowCount;
    float maxerror, lerror, error;
    //MPI_Status status;
    float *temp;
    
    float epsilon = 0.0001;
    int root = 0;
    int step = 1;
        
    string filename;     // file name
    double t1, t2;       // time variables
    int i, j;            // loop variables
    
    // Get filename and number of threads from user
    if (argc != 2) {
        printf("useage: jacobi-mpi filename\n");
        return 0;
    } else {
        filename = argv[1];
    }
        
    // Open the file and read in the matrix dimensions
    ifstream file;
    file.open(filename);
    file >> M;
    file >> N;
    
    // Dynamic array allocation for arrays
    int **matrix = (int **)malloc(M * sizeof(float *));
    int *b = (int *)malloc(N * sizeof(float));
    for (i = 0; i < M; i++) {
        matrix[i] = (int*)malloc(N* sizeof(float));
    }
  
    // read in matrix
    for(i=0; i<N; i++) {
        for (j=0; j<M; j++) {
            file >> matrix[i][j];
        }
    }

    file.close();
    
    // generate b and initial guess, x
    for(i=0; i<N; i++) {
        b[i] = (rand() % 8);
        if ( rand() % 2 *-1 == -1)
        {
            b[i] *= -1;
        }
    }
    
    t1 = time1();
    
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numranks);
    MPI_Comm_rank(MPI_COMM_WORLD,&myrank);
    //printf ("MPI task %d has started...\n", myrank);
    
    if (myrank != 0) {
        rowCount = M / (numranks - 1);
        //if (myrank == numranks - 1) {
        //    rowCount = M - (numranks-2)*rowCount;
        //}

        float *x = (float *)malloc(N * sizeof(float));
        float *xOld = (float *)malloc(N * sizeof(float));
        for(i=0; i<N; i++) {
            x[i] = 0;
            xOld[i] = 0;
        }
        while (1) {
            error = updateSolution(matrix, b, x, xOld, rowCount, myrank, numranks, N);
            
            temp = x;
            x = xOld;
            xOld = temp;
            
            MPI_Reduce(&error, NULL, 1, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);
            MPI_Bcast(&error, 1, MPI_FLOAT, root, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);

            if (error < epsilon) {
                break;
            }
        }
        free(x);
        free(xOld);
    
    } else {
        while(1) {
            MPI_Reduce(&lerror, &maxerror, 1, MPI_FLOAT, MPI_MAX, root, MPI_COMM_WORLD);
            MPI_Bcast(&maxerror, 1, MPI_FLOAT, root, MPI_COMM_WORLD);
            MPI_Barrier(MPI_COMM_WORLD);

            if (maxerror < epsilon) {
                printf("# Iterations: %d\n", step);
                t2 = time1();
                printf("\n\nTOTAL TIME %f\n", t2-t1); 
                break;
            }
            step++;
        }
    }
    
    MPI_Finalize();
    //t2 = time1();
    //printf("\n\nTOTAL TIME %f\n", t2-t1); 
    for (int i=0; i < M; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(b);
    return 0;
}

float updateSolution(int** matrix, int *b, float *x, float* xOld, int rowCount, int myrank, int numranks, int N) {   
    float lmax = 0, error, sum;
    int i, j;
    
    // decide which rows will be handled
    int begin = (myrank - 1) * rowCount;
    
    if (myrank == numranks - 1) {
        rowCount = N - (numranks-2)*rowCount;
    }
    
    int end = begin + rowCount - 1;
    
    //cout << "begin: " << begin << endl;
    //cout << "  end: " << end << endl;
    
    // find solutions
    for (i=begin; i <= end; i++) {
        sum = 0;
        for (j=0; j < N; j++) {
            if (i != j)
                sum += matrix[i][j] * xOld[j];
        }
        x[i] = (b[i] - (sum)) / matrix[i][i];
    }
    
    // calculate the maximum error for any of the equations to compare to epsilon
    for (int i=begin; i <= end; i++) {
        error = b[i];
        for (j=0; j<N; j++) {
            error -= matrix[i][j] * x[j];
        }
        if (abs(error) > lmax) {
            lmax = abs(error);
        }
    }
    
    return lmax;
}

double time1() {
    struct timeval tv;
    gettimeofday( &tv, (struct timezone *) 0);
    return ( (double) (tv.tv_sec + (tv.tv_usec / 1000000.0)) );
}

double time2() {
    return ( (double) time(NULL) );
}