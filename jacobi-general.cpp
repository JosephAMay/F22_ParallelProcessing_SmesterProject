#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sys/time.h>

using namespace std;

double time1();
double time2();

int main(int argc, char *argv[]) {
    
    float epsilon, error, max_error, sum;    // acceptable error, iteration error, maximum error, sum
    int N, M, i, j;                          // matrix dimensions NxM, loop variables
    double t1, t2;                           // time variables
    string filename;                         // matrix file
    
    // Get filename from user
    if (argc != 2) {
        printf("useage: jacobi-pthread filename\n");
        return 0;
    } else {
        filename = argv[1];
    }
    
    // Open the file and read in the matrix dimensions
    ifstream file;
    file.open(filename);
    file >> N;
    file >> M;
    
    int matrix[N][M];      // matrix
    int b[N];              // b
    float xOld[N];         // old solution
    float x[N];            // new solution
    
    // read in matrix data
    for(i=0; i<N; i++) {
        for (j=0; j<M; j++) {
            file >> matrix[i][j];
        }
    }

    file.close();
    
    // generate b and initial guess, x=0
    for(i=0; i<N; i++) {
        b[i] = (rand() % 8);
        x[i] = 0;
        xOld[i] = 0;
        if ( rand() % 2 *-1 == -1)
        {
            b[i] *= -1;
        }
    }
 
    epsilon = 0.01;      // acceptable error    
    error = 1;           // starting error rate
    max_error = 1;       // starting max error rate
    int iterations = 0;  // # iterations 
    
    t1 = time1();
    // loop until acceptable error is reached
    while (abs(max_error) > epsilon) {
        iterations++;
        // find solutions
        for (int i=0; i < N; i++) {
            sum = 0;
            for (int j=0; j < M; j++) {
                if (i != j)
                    sum += matrix[i][j] * xOld[j];
            }
            x[i] = (b[i] - (sum)) / matrix[i][i];
        }

        // update xOld
        for (int i=0; i < N; i++) {
            xOld[i] = x[i];
        }
        
        // calculate the maximum error for any of the equations to compare to epsilon
        max_error = 0;
        for (int i=0; i < N; i++) {
            error = b[i];
            for (j=0; j < M; j++) {
                error -= matrix[i][j] * x[j];
            }
            if (abs(error) > max_error)
                max_error = error;
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
    bool bad = false;
    for (i=0; i < N; i++) {
        sum = 0;
        for (j=0; j < M; j++) {
            sum += x[j] * matrix[i][j];
        }
        if (abs(b[i] - sum) > epsilon) {
            bad = true;
        }
    }
    if (bad) {
        cout << "bad solution!\n";
    } else {
        cout << "good solution!\n";
    }
    
    return 0;            
}

double time1() {
    struct timeval tv;
    gettimeofday( &tv, (struct timezone *) 0);
    return ( (double) (tv.tv_sec + (tv.tv_usec / 1000000.0)) );
}

double time2() {
    return ( (double) time(NULL) );
}