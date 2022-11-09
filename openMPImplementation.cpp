// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
using namespace std;
#include <fstream>
#include <string>
#include <omp.h>
#define MAXTHREADS 200
//Global variables
int numrows;
int numcols;
float epsilon = .01;
float maxerror = 1;
int threadID;

//Will contain matrices for jacobi iteration
float **matrix;      	// whole matrix
float *b;              	// right hand side array
float *xOld;         	// old solution
float *x;            	// new solution

float maxeps[MAXTHREADS];

//function prrototypes
void OMPjacobi(float **matrix, float *xOld, float *x,float * b, int numThreads);


int main(int argc, char* argv[])
{

    //declare variables
    int i;
    int numThreads;
    string fileName;
    fstream myfile;    
      
    // Get filename & numBer of threads from from user
    if (argc != 3) {
        printf("useage: Number of threads, filename\n");
        return 0;
    } 
    else {
    	//get command line argument for filename 
    	numThreads = atoi(argv[1]);
    	fileName = string(argv[2]);
	
	/*//uncomment to verify correct input	
    	cout << numThreads << endl;
	cout << fileName << endl;
	*/
    }
    
       
    //Open matrix file
    myfile.open(fileName);

    if (!myfile)
    {
	cout << "File doesn't exist / there is a typo. \n";
	cout << "Please try again and check spelling.\n";
    }
    else 
    {
    //first two entries will be number of rows, then number of columns
    //all data after that will be rows with entries seperated by a space
    myfile >> numrows;
    myfile >> numcols;

    // Dynamiically allocate arrays, now that we know their size
    matrix = (float **)malloc(numrows * sizeof(float *));
    b = (float *)malloc(numcols * sizeof(float));
    x = (float *)malloc(numcols * sizeof(float));
    xOld = (float *)malloc(numcols * sizeof(float));
    for (i = 0; i < numrows; i++) {
        matrix[i] = (float*)malloc(numcols* sizeof(float));
    }   

    //Read In from file, store into matrix array
    for( i=0; i<numrows;i++)
    {
	for(int j=0; j<numcols; j++)
	{
		myfile >> matrix[i][j];		
	}
	
    }

    //Close the file now that we are done with the data
    myfile.close();


    /*//Uncomment to veriffy file has been read in correctly
    for( i=0; i<numrows;i++)
    {
	for(int j=0; j<numcols; j++)
	{
		cout << matrix[i][j] << " ";		
	}
	cout << endl;
    }
*/

    // generate b and initial guess, x=0
    for(i=0; i<numrows; i++) {
        b[i] = (rand() % 8);
        x[i] = 0;
        xOld[i] = 0;
        if ( rand() % 2 *-1 == -1)
        {
            b[i] *= -1;
        }
    }


    //Call jacobi function, iterate until a solution is found    
    OMPjacobi(matrix, xOld, x, b, numThreads);
    
    
    }

    //release dynamic memory
    free(matrix);
    free(b);
    free(x);
    free(xOld);
        
    return 0;
}

void OMPjacobi(float **matrix, float *xOld, float *x,float * b, int numThreads)
{

     //Declare variables
     int threadID;
     int epsilon = .01;	
     int iterations = 0; 
     float lmax = 0, error, sum;
     int thread_num1, i, j, rowStep, begin, end;
     // decide which rows will be handled
     thread_num1 = numThreads;
     long int lcount;

          //begin parallel work
     #pragma omp parallel num_threads(numThreads) private(threadID, i, j,sum, error, begin, end, rowStep) shared(maxerror)
     {
    	threadID = omp_get_thread_num();
	
	rowStep = (int) (numrows/thread_num1); 
     	begin = (threadID * rowStep);
     	end = ((threadID + 1) * rowStep) - 1;


	//uncomment to confirm the correct number of threads are made
	//cout << threadID << endl;
	
	// loop until acceptable error is reached
    	while (abs(maxerror) > epsilon) {
        
		//Keep track of total iterations
		#pragma omp critical 	
		{
			iterations++;
			maxerror = 0;
		}

        	// find solutions
        	for (i=begin; i <= end; i++) {
            		sum = 0;
			for(j = 0; j<numcols;j++)
			{
                		if (i != j)
				{
                    		sum += matrix[threadID][i] * xOld[i];
				}
            		x[i] = (b[i] - (sum)) / matrix[i][i];
	    		xOld[i] = x[i];
			}
		}

        
	// calculate the maximum error for any of the equations to compare to epsilon
        for (int i=begin; i <=end; i++) {
            	error = b[i];
            	for (j=0; j < numcols; j++) {
                	error -= matrix[i][j] * x[j];
            	}
            	if (abs(error) > maxerror){
                	lmax = error;
	    	}

		}
	}	

	maxeps[threadID] = lmax;
	#pragma omp critical 
	{
		if(maxeps[threadID] >= maxerror)
		{
			maxerror = maxeps[threadID];
		}
	}
	//threads wait until eveyone is synced to do the next pass.	
	#pragma omp barrier
	
     }
	//end parallel section
    
} //end function
	

