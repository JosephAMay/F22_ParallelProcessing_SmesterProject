#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main(int argc, char *argv[]) {
    
    // TODO: have values be entered as params
    float epsilon, error, max_error, sum;
    int N, M, i, j;     
    
    ifstream file;
    file.open( "300Matrix17.txt");
    file >> N;
    file >> M;
    
    int matrix[N][M];
    int b[N];
    float xOld[N];
    float x[N];
    
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
 
    epsilon = 0.1;    // acceptable error    
    error = 1;        // starting error rate
    max_error = 1;    // starting max error rate
    
    // loop until acceptable error is reached
    while (abs(max_error) > epsilon) {
        
        // find solutions
        for (int i=0; i < N; i++) {
            sum = 0;
            for (int j=0; j < N; j++) {
                if (i != j)
                    sum += matrix[i][j] * xOld[j];
            }
            x[i] = (b[i] - (sum)) / matrix[i][i];
        }

        for (int i=0; i < N; i++) {
            xOld[i] = x[i];
        }
        
        // calculate the maximum error for any of the equations to compare to epsilon
        max_error = 0;
        for (int i=0; i < N; i++) {
            error = b[i];
            for (j=0; j<N; j++) {
                error -= matrix[i][j] * x[j];
            }
            if (abs(error) > max_error)
                max_error = error;
        }
    }
    
    // Check for wrong solution
    for (i=0; i < N; i++) {
        sum = 0;
        for (j=0; j<N; j++) {
            sum += x[j] * matrix[i][j];
        }
        if (abs(b[i] - sum) > epsilon) {
            cout << "false";
        }
    }
            
            
    return 0;
            
}