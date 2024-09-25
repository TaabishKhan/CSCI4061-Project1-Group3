# CSCI4061-Project1-Group3
Hello -Mike

Finish Make ME
Finish Read ME
Refine Code and delete extra comments / print statements


AUTOGRADER.C
Verify that the output is correct given the input (1 3 5 7) 
Clean up the output formatting
Print function to add to the output file in the given format
We may neeed to implemet a kill function to stop segementaion fails.

// Taabish Khan - Khan0882@umn.edu - 5819790
// CSCI 4061 (001) - Jon Weissman
// P1 Group 3 : Taabish Khan, Michael Sharp, Omar Yasin



You are assumed to be using some IDE and have GCC and GNU installed to operate.
Ensure that your environment has access to all the necessary libraries for system calls like fork(), exec(), and waitpid().

How to Compile autograder.c
---
First compile autograder.c using this command:
```console
$ gcc -o autograder autograder.c 
```

or 

``` console 
$ make 
``` 

This will create a out put file named autograder.
To run the auto grader simply 

``` console 
$ ./autograder <batch_size> <parameter> <parameter> <parameter>
```

NOTE: n parameters means n executions of the autograder.

Example:

``` console
$ ./autograder 1 3 7 9
```




You can open a terminal or command prompt and navigate to the directory containing autograder.c. Then, type gcc -o autograder autograder.c to compile the code. This will generate an executable file called autograder.
To execute, type ./autograder (x) (y) (z) and replace x y and z with any integer to run the executables with.

The autograder will execute the provided student programs, assess their correctness, and report their performance based on the input parameters.
