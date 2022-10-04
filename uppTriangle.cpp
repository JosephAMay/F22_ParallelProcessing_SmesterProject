// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
using namespace std;
#include <fstream>
#include <string>
//number of rows and columns in the array

string filename; //= "500Matrix"; // to name files
int const NUMROW = 1000;
int const  NUMCOL  = 1000;
//function prototype
void createRandArray(int row, int col, int arr[NUMROW][NUMCOL], int* sumArr);
bool checkDiagDominance(int arr[NUMROW][NUMCOL], int row, int col);
void createSysArray(string filename, int **matrixArray, int* rhsArray, int* guessValue, int* answerArray );
void arrayWriteToFile ( int arr[NUMROW][NUMCOL], int row, int col);
void printMatrix ( int * matrixArray);
void print2dMatrix (int ** matrixArray);

int main(int argc, char* argv[])
{
    //accept command line arguments
    filename = string(argv[1]);

    //declare variables
    int arr[NUMROW][NUMCOL];
    int * sumArr;
    bool diagDominant;
    int ** matrixArray;
    int * rhsArray;
    int * guessValue;
    int* answerArray;
    string infile;
    srand(time(NULL)); //set random seed
    
    //create a random array of specified size
    createRandArray(NUMROW, NUMCOL, arr, sumArr);


    //check array is diagonally dominant
    diagDominant = checkDiagDominance(arr, NUMROW, NUMCOL);


    if(diagDominant == 1) //it is diagonally dominant
    {
    	//write the random array to a file to be stored for later usage
    	arrayWriteToFile(arr, NUMROW, NUMCOL);
    }


    return 0;
}

//create a random array from scratch
//this will be a lightly sparse array.
void createRandArray(int numRow, int numCol, int arr[NUMROW][NUMCOL], int* sumArr)
{
    //declare variables
    int i, j;
    int sum = 0;
    int iszero;
    //make new arrays
    sumArr = new int [numCol]; // will hold sum of row values
    //Loop through array to be created
    for (i = 0; i < numRow; i++)
    {
        sum = 0; //reset row sum for each row
        for (j = 0; j < numCol; j++)
        {
	if(j >= i)
	{
	    //give chance to be 0s
	    //for sparse matrix
	    iszero = rand() %2;
	    if(iszero == 0)
	    {
		arr[i][j] = 0;
	    }
	    else //assign random value
	    {
	    	//assign  a random number between 0 and 7
            	arr[i][j] = (rand() % 8);
	   

	  	//randomly assign some values to be negative 
	    	////must check equal to -1, otherwise could be 0 and you'd null out matrix values
	    	if ( rand() % 2 *-1 == -1)
	   	{
			arr[i][j] *= -1;
	    	}
	    }
	    if (i != j) //not a main diagonal value
            {
                sum += abs(arr[i][j]);
            }

        }
	else if ( j <= i)
    	{
		arr[i][j] = 0;
    	}
        
	//add in sum of values for the row to array
	sumArr[i] = sum;
    }
    }
     
    //loop through main diagonals to check for diagonal dominance
    for (int x = 0; x < numRow; x++)
    {
        for (int y = x; y <= x; y++)
        {
            //while main diagonal values are less than the sum of the rest of their row
            //increase the value to ensure diagonal dominance
            while (arr[x][y] <= sumArr[x])
            {
                arr[x][y] += 1;
            }
	    //randomly assign some values to be negative 
	    ////must check equal to -1, otherwise could be 0 and you'd null out matrix values
	    if ( rand() % 2 *-1 == -1)
	    {
		arr[i][j] *= -1;
	    }
        }
    }

    return;
}

//write the array to a file for later use
void arrayWriteToFile ( int arr[NUMROW][NUMCOL], int numrow, int numcol)
{

    //Declare variables
    int i,j;
    ofstream myfile;
    //add random appendix to filename so this can be rerun and generate a new file
    string numAppendix = to_string(rand() % 100);
    myfile.open(filename + numAppendix + ".txt");
    
    //have num rows and cols be first items in the file
    myfile << numrow << " " << numcol << endl;

    //write array to a file with spaces in between values
    for (i = 0; i < numrow; i++)
    {
        for (j = 0; j < numcol; j++)
        {
            myfile << arr[i][j] << " ";
        }
        myfile << endl;
    }
 
    myfile.close();

    return;
}

//check the diagonal dominance of a function
bool checkDiagDominance(int arr[NUMROW][NUMCOL], int row, int col)
{
    //declare variables
    bool flag = true;
    int firstVal;
    int sum = 0;
    int i, j;
    
    //loop through array
    for (i = 0; i < row; i++)
    {
	sum = 0;	//reset sum of row for each row
        for (j = 0; j < col; j++)
        {
            if (i == j) //diagonal value
            {
                firstVal = abs(arr[i][j]);
            }
            else //rest of matrix values
            {
                sum += abs(arr[i][j]);
            }
        }

        if (firstVal < sum) // not diagonally dominant return false
        {
            flag = false;
            break;
        }
    }
    return flag;
}

//Create the arrays to be used for the linear system of equations
//matrix array = whole system
//rhs array = rhs values of system
//guess value = guess values of variables
//answer Array = empty array that will hold final answers to problem,
//alternatively could scrap answer array and use guessvalue when it is finished iterating
void createSysArray(string  infile, int** matrixArray, int* rhsArray, int* guessValue, int* answerArray)
{
    int numRows;
    int numCols;
    int rhs = numCols - 1;
    int cur_val;
    ifstream myFile;

    //open input file
    myFile.open(infile);

    myFile >> numRows;
    myFile >> numCols;

    //setup array to be right number of rows and columns
    matrixArray = new int* [numRows];
    for (int i = 0; i < numRows; i++)
    {
        matrixArray[i] = new int[numCols];
    }
    //setup guess values to be right size and all 0s initially
    guessValue = new int [numRows];
    for (int i = 0; i < numRows; i++)
    {
        guessValue[i] = 0;
    }

    //setup rhs to be right size
    rhsArray = new int [numRows];

    //setup answerArray to be right size
    answerArray = new int [numRows];


    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            myFile >> cur_val;
                if (j == numCols-1) // this is the end value of the matrix and needs to be stored in the rhs matrix
                {
                    rhsArray[i] = cur_val;
                }
                else //LHS side of array
                {
                    matrixArray[i][j] = cur_val;
                }
        }
    }

    //print2dMatrix(matrixArray); 
    //printMatrix(guessValue);
    //printMatrix(rhsArray);
    
    
    //close input file
    myFile.close();


    return;
}


//print out 1d matrix, plus number of entries in the matrix to verify right number added
void printMatrix(int *matrix)
{
	int i,j;
	int count = 0;

	for(i = 0; i<NUMROW; i++)
	{
		count ++;
		cout << matrix[i] << endl;
	}
	cout << count;

	return;
}




//prints out the last 3 values in the new big matrix, just to verify
//that all values got included in it
void print2dMatrix(int **matrix)
{
	int i,j;
	for( i =NUMROW-1; i> NUMROW-4; i--)
	{

		for(j=NUMCOL-1; j>NUMCOL-4; j--)
		{
			cout << matrix[i][j] <<" ";
		}
		cout << " i= " << i << " j = " << j << endl;
	}


	return;
}
