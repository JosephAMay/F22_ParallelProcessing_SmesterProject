#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/time.h>
#define MAXTHREADS 200

using namespace std;

int N, M, num_threads;       // matrix dimensions NxM, number of iterations
float **matrix;              // matrix
float *b;                    // b
float *xOld;                 // old solution
float *x;                    // new solution
float epsilon, max_error;    // acceptable error, maximum error
pthread_mutex_t lock_m;      // mutex
float max_eps[MAXTHREADS];   // stores max errors

bool checkSolution();
void * updateSolution(void * argu);
double time1();
double time2();

int main(int argc, char *argv[]) {
    
    string filename;     // file name
    double t1, t2;       // time variables
    int i, j;            // loop variables
    
    // Get filename and number of threads from user
    if (argc != 3) {
        printf("useage: jacobi-pthread filename num_threads\n");
        return 0;
    } else {
        filename = argv[1];
        num_threads = atoi(argv[2]);
    }
        
    // Open the file and read in the matrix dimensions
    ifstream file;
    file.open(filename);
    file >> M;
    file >> N;
    
    // Dynamic array allocation for arrays
    matrix = (float **)malloc(M * sizeof(float *));
    b = (float *)malloc(N * sizeof(float));
    x = (float *)malloc(N * sizeof(float));
    xOld = (float *)malloc(N * sizeof(float));
    for (i = 0; i < M; i++) {
        matrix[i] = (float*)malloc(N* sizeof(float));
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
        x[i] = 0;
        if ( rand() % 2 *-1 == -1)
        {
            b[i] *= -1;
        }
    }
 
    epsilon = 0.01;      // acceptable error 
    int iterations = 0;  // iteration number
    
    // loop until acceptable error is reached
    pthread_t pids[num_threads-1];
    t1 = time1();
    while (1) {
        iterations++;
        max_error = 0;
        
        for (i=0; i < num_threads; i++) {
            pthread_create(&pids[i], NULL, updateSolution, (void *) (long int)i );
        }
        
        for (i=1; i < num_threads; i++) {
            pthread_join(pids[i], NULL);
        }
        
        if (max_error <= epsilon) {
            break;
        }
    }
    t2 = time1();
    printf("\n\nTOTAL TIME %f\n", t2-t1);  
    cout << "# Iterations: " << iterations << "\n";
    
    cout << "\nx:\n";
    for (i=0; i < N; i++) {
        cout << x[i] << " ";
    }
    cout << "\n\n";
    
    // Check for wrong solution
    if (checkSolution()) {
        cout << "good solution!\n";
    } else {
        cout << "bad solution\n";
    }

    // Free allocated memory
    for (i=0; i < M; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(b);
    free(x);
    free(xOld);
            
    return 0;
            
}

void * updateSolution(void * argu) {   
    long int lcount = ((long int)argu);
    float lmax = 0, error, sum;
    int thread_num1, i, j, rowStep, begin, end;
    
    // decide which rows will be handled
    thread_num1 = num_threads;
    rowStep = (int) (M/thread_num1);
    begin = (lcount * rowStep);
    end = ((lcount + 1) * rowStep) - 1;
    
    // find solutions
    for (i=begin; i <= end; i++) {
        sum = 0;
        for (j=0; j < N; j++) {
            if (i != j)
                sum += matrix[i][j] * xOld[j];
        }
        x[i] = (b[i] - (sum)) / matrix[i][i];
    }

    for (int i=begin; i <= end ; i++) {
        xOld[i] = x[i];
    }
    
    // calculate the maximum error for any of the equations to compare to epsilon
    for (int i=begin; i <= end; i++) {
        error = b[i];
        for (j=0; j<N; j++) {
            error -= matrix[i][j] * x[j];
        }
        if (abs(error) > lmax) {
            lmax = error;
        }
    }
    
    max_eps[lcount] = lmax;
    pthread_mutex_lock(&lock_m);
    if (max_eps[lcount] >= max_error) {
        max_error = max_eps[lcount];
    }
    pthread_mutex_unlock(&lock_m);
    
    return NULL;
}

bool checkSolution() {
    int i,j;
    float sum;
    for (i=0; i < N; i++) {
        sum = 0;
        for (j=0; j<N; j++) {
            sum += x[j] * matrix[i][j];
        }
        if (abs(b[i] - sum) > epsilon) {
            return false;
        }
    }
    return true;
}

double time1() {
    struct timeval tv;
    gettimeofday( &tv, (struct timezone *) 0);
    return ( (double) (tv.tv_sec + (tv.tv_usec / 1000000.0)) );
}

double time2() {
    return ( (double) time(NULL) );
}