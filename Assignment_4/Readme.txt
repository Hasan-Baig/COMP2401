Name: Hasan Baig
Student Number: 101032292

Source Files Included:
display.c
display.h
obstacles.h
pathPlanner.c
plannerTester.c
bigEnvironment.c
makefile

INSTRUCTIONS:

Compiling:
1) make plannerTester
2) make bigEnvironment
3) make all

Running:
./plannerTester
./bigEnvironment

Check for Memory Allocation:
valgrind --leak-check=yes ./plannerTester
valgrind --leak-check=yes ./bigEnvironment

Cleaning .o files:
make clean
