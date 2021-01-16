# Hashmap
## By Adrian Currington

### Description
This hashmap stores a key-value pair, where both the key and value are void* elements. This allows a key and value of any type to be stored by storing a pointer to it. Upon initialisation, the initial capacity of the map is determined, and function pointers to a hash function and a comparison function are set. This hashmap is resizeable and expands its capacity when the number of elements is 66% that of the current capacity. Collisions are also dealt with using separate chaining, with each node keeping a pointer to the next node in the separate chain.

### Files Includes
- main.h: Header file for my hashmap.
- main.c: Implementation code for my hashmap.
- tester.c: Tester code to test the functionality of each method in main.c.
- main.c.gcov: Shows the coverage of the main.c code when the tester.c file is run.

### Run On Your Machine
I ran the tester code with the following inputs to the command line:
1. gcc tester.c main.c
2. ./a.out
