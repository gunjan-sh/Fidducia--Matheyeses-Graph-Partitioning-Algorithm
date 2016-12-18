# Fidducia--Matheyeses-Graph-Partitioning-Algorithm
Implementation of Fidducia--Matheyeses Algorithm

The first step of the physical design process in VLSI chip fabrication is partitioning. It is done to solve the complexity of the design.
Fidducia--Matheyeses is an algorithm for automizing the partitioning process.  

Algorithm 

1. Compute the balance criterion
2. Compute the cell gain of each cell
3. Choose the base cell ci that has maximum gain and satify the balance criteria and move the cell
4. Lock the base cell and update the gains of all the cells connected to the nets of this base cell
5. If all the cell are locked then go to step 6, otherwise go to step 3
6. Determine the best move sequence c1,c2,……cm(1<=m<=i), so that Gm = ΣΔ𝑔𝑖𝑚𝑖=1 is maximized. If Gm > 0 go to step 7
7. Make all the moves permanent and end.

Implementation details:

The Algorithm was implemented in C++ using STL libraries. 
The code is written in a very modular way where different functions have been defined for calculating different aspects of the FM-algorithm.

Steps to run the code:

1. First download the ibmXX.net and ibm.are file in a directory (XX = benchmark id)
2. Run the perl script: $ ibm.perl ibmXX.net ibmXX.are > ibmXX.hgr
3. Compile partitioning.cpp $ g++ partitioning.cpp
4. Run the FM algorithm: $ ./a.out ibmXX.hgr
