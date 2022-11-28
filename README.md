# F22_ParallelProcessing_SmesterProject
Semester long project for the 6330 Parallel Processing Class at mtsu


This project has two main aims:
Firstly: to implement an algorithm to solve linear systems of equations
Secondly: to parallelize that program to achieve signigant speedups.


# Algorithm:
Due to the need to parallaelize the project, certain algorithms are ill-suited or not applicable for this application.

We chose to implement the jacobi iteration algorthim.
This algorthim works only for nonsingular matrices that are diagonally dominant.
It is an iterative algorthim, and so it does not typically arrive at an exact solution. Rather it solves the system with
values that are close to the exact. The program will continue iterating until the error, difference between the expected value and the actual value, is below a threshold, commonly assumed to be .1.

# Jacobi Iteration:
Jacobi iteration is executed in the following steps:

Given a linear system such as:
3x +1y + 2z = 9

2x + 4y +1z = 8

4x + 4y + 12z = 20


Importantly for out program, such a system may also be represented by three matrices:

[3 1 2]  [x] [9] 

[2 4 1]  [y] [8] 

[4 4 12] [z] [20] 
That is one matrix to hold variable coefficients, one matrix to hold the variables, one matrix to hold the answers


Solve for one variable per row

x = (9 - y -2z)/3

y = (8-2x-z)/4

z = (20-4x-4y)/12

Assume a value for each variable. The better the assumption, the less iterations are required to arrive at the answer, the faster the process. It is easiest to assume 0 when starting out.

Assumed values (x=0, y=0, z=0)

Plug in the assumed values of each variable to the equation, and compare the answer to the expected answer for error

x = (9 - 0 -2(0))/3

y = (8-2(0)-0)/4

z = (20-4(0)-4(0))/12


x = 9/3 = 3

y = 8/4 = 2

z = 20/12 = 1.66


Use these values to as input to the original system

3x +1y + 2z = 9

becomes

3(3) + 2 + 2(1.66) = 9

14.32 = 9

2x + 4y +1z = 8

becomes

2(3) + 4(2) + 1.66 = 8

15.66 = 8


4x + 4y + 12z = 20

becomes 

4(3) + 4(2) + 12(1.66) = 20

39.92 = 20



You can see after the first iteration, no values matched or were close to the target. 
The iteration would proceed in the same steps
assume a value
use that value to solve for a variable
check to see if the assumed value results in correct answer
if not, keep iterating using the newest assumed values as inputs to the next loop.



# Parallelization
We chose this method because it is easy to parallelize.
Our main data structure is an array
Our main processing technique is a loop.

Both arrays and loops are exceellent targets for parallel operations.

Each row of the linear system can be operated on independently from other rows. There is little communication overhead. Each thread just needs to enter in its new assumed variable value at the end of each opearation and continue iterating until the error is below the .1 threshold. 

# Steps
First we used the array functions.cpp file to generate arrays of variable sizes. The minimum was 500x500, the maximum was 1000x1000.
Currently, the arrays are guarenteed to be diagonally dominant, and can positive or negative values in the range [-7,7], except for the diagonal values
which must be larger than the magnitude of their rows to maintain diagonal dominance.


Some sample matrices are contained in the arrayData.zip file. The file names follow this pattern: size of the matrix is the first section, followed by the word Matrix followed by some random number, so an example filename of a 550x550 Matrix would be "550x550Matrix32"


From there we made the acobi general.cpp program to solve these random matrices sequentially.

After that we made the jacobi pthread.cpp which is a parallel version of the original sequential jacobi program.

Aditionally we also implemented the jacobi alrogithm using openMP and MPI

#Testing 
To evaluate our programs, we recorded and charted the run times of each implementation.
Run time started after each program read in the matrix file and initialized the arrays,
and stopped once the epsilon point was reached. The run time of each program is printed to standard output. 

In order to maintain standards, all parallel implementations were tested using the matrix 1000Matrix20.txt. 
This is a non-sparse array of size 1000x1000.

Each implementation was tested with 4 thread values: 1,2,4,8. 

Each implementation was run 100 times for each possible thread number.
For example, the pthread implementation ran 100 times with 1 thread, 100 times with 2 threads, 
100 times with 3 threads, 100 times with 4 threads. The results of each test were sent into a file.
This process was repeated for both openMP and MPI implementations.

On screen I have an example shell command to do a test like this colored in black. 
there is an outer loop which represents the possible thread numbers 1 2 4 and 8. Then an inner loop
which loops 100 times. Each thread number goes 100 times and the output of that thread number is appended to a file

Once all the individual files are made, they are combined into a complete file with all thread data 
using the the command of the bottom bullet point. Listing the files with the -v flag so they appear in numerical order
then the files are pasted together seperated by a tab.

These complete files were exported into microsoft excel where I graphed the average, minimum, and maximum times of each
implementation which we will discuss below. 


# Future Work 
From here, we want to generate different types of matrices (sparse, upper triangular, non diagonally dominant) and test to see how the program performs, implement jacobi with different parallel methods like open mp and compare results amongst parallel implementations, as well as try to find a specific problem application of the jacobi algorithm that we can use
