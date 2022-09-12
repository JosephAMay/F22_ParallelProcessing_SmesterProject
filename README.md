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

