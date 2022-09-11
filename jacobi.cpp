#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

int main(int argc, char *argv[]) {
    
    // TODO: have values be entered as params
    float epsilon, error, max_error;
    int N = 3;                                               // size of square matrix
    
    // TODO: need to check if it is diagonally dominant
    int matrix[3][3] = {{9, 2, 3}, {1, 12, 9}, {4, 6, 14}};  // square matrix
    int b[3] = {7, 2, 1};                                    // values
    float xOld[3] = {0, 0, 0};                               // initial guess
    float x[3];                                              // will hold solution at end of iteration
    
    epsilon = 0.1;    // acceptable error
    error = 1;        // starting error rate
    max_error = 1;    // starting max error rate
    
    // loop until acceptable error is reached
    while (std::abs(max_error) > epsilon) {
        
        // find solutions
        for (int i=0; i < N; i++) {
            float sum = 0;
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
            error = b[i] - (matrix[i][0] * x[0]) - (matrix[i][1] * x[1]) - (matrix[i][2] * x[2]);
            if (std::abs(error) > max_error)
                max_error = error;
        }
    }
    
    // final solution
    for (int i=0; i < N; i++) {
        std::cout << x[i] << "\n";
    }
      
    return 0;
}