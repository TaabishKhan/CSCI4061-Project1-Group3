// Taabish Khan - Khan0882@umn.edu
// CSCI 4061 (001) - Jon Weissman
// P1 Group 3 : Taabish Khan, Michael Sharp, Omar Yasin

#include "include/utility.h"
#include <stdio.h>      // for printf, fprintf
#include <stdlib.h>     // for exit, atoi, qsort
#include <unistd.h>     // for fork, exec
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>   // for waitpid, WIFEXITED, WEXITSTATUS, WIFSIGNALED
#include <errno.h>      // for perror
#include <string.h>     // for strcspn

// Define constants
#define MAX_FILES 100  // Max number of executables we expect
#define MAX_FILENAME_LEN 100  // Max length of each file name
#define OUT_FILE "autograder.out"

// Comparator function for sorting solution names in alphabetical order
int compare_filenames(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int main(int argc, char *argv[]) {

    // Initialize variables
    FILE *file; // Input file for solutions
    FILE *out_file; // Output file for generated solutions
    char *filenames[MAX_FILES];  // Array of strings to hold executable file names
    char buffer[MAX_FILENAME_LEN];  // Temporary buffer to hold each line (file name)
    pid_t pid; // Child process
    int status; // Current exit status of child
    int total_executables = 0;
    int finished = 0; // Number of executed processes with a given parameter

    // Open the output file and error check
    out_file = fopen(OUT_FILE, "w");
    if (out_file == NULL) {
        perror("Error opening autograder.out");
        return 1;
    }

    // Improper input check and syntax printing for execution
    if (argc < 2) {
        printf("Usage: %s <batch> <p1> <p2> ... <pn>\n", argv[0]);
        fclose(out_file);
        return 1;
    }

    // Convert the first argument to an integer to get the batch size
    int batch_size = atoi(argv[1]);
    if (batch_size <= 0) {
        printf("Invalid batch size\n");
        fclose(out_file);
        return 1;
    }

    // Collect all input parameters (e.g., p1, p2)
    int number_of_parameters = argc - 2;
    char *parameters[number_of_parameters];
    for (int i = 0; i < number_of_parameters; i++) {
        parameters[i] = argv[i + 2];  // parameter i corresponds to argv[i+2]
    }

    // Write the file paths from the "solutions" directory into the submissions.txt file
    write_filepath_to_submissions("solutions", "submissions.txt");

    // Open submissions.txt in read mode and error check
    file = fopen("submissions.txt", "r");
    if (file == NULL) {
        perror("Error: Could not open submissions.txt");
        fclose(out_file);
        return 1;
    }

    // Read each line from the file and store it in the filenames array and removing the newline
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = '\0';

        // Allocate memory and store it in the array
        filenames[total_executables] = malloc(strlen(buffer) + 1);
        strcpy(filenames[total_executables], buffer);

        // Increment the total count and error check
        total_executables += 1;
        if (total_executables >= MAX_FILES) {
            printf("Too many files, increase MAX_FILES\n");
            break;
        }
    }
    fclose(file);  // Close the submissions.txt file after reading

    // Sort the filenames array alphabetically
    qsort(filenames, total_executables, sizeof(char *), compare_filenames);

    // For each parameter, run all executables in batch size chunks
    for (int i = 0; i < number_of_parameters; i++) {

        // Reset processed_executables for each parameter
        int processed_executables = 0;

        // Run while there are still solutions to be executed on the current parameter
        while (processed_executables < total_executables) {

            // Loop for B times to batch the executions
            for (int j = 0; j < batch_size && processed_executables < total_executables; j++) {

                // Get the filename from the sorted array
                char *filename = filenames[processed_executables];

                // Begin creating child processes and call them on the filename generated
                pid = fork();

                // Child process
                if (pid == 0) {
                    char *exec_args[] = {filename, parameters[i], NULL};
                    if (execv(filename, exec_args) == -1) {
                        perror("Error during exec");
                        exit(1);
                    }
                } else if (pid < 0) {
                    perror("Fork failed");
                }
                processed_executables++;
            }

            // Post batch parent processing
            finished = 0;
            while (finished < batch_size && finished < total_executables) {

                // Wait on child process to return
                pid_t wpid = waitpid(-1, &status, 0);
                if (WIFEXITED(status)) {
                    if (WEXITSTATUS(status) == 0) {
                        // Correct answer
                        fprintf(out_file, "%s: %d (correct)\n", filenames[processed_executables - batch_size + finished], atoi(parameters[i]));
                    } else {
                        // Incorrect answer
                        fprintf(out_file, "%s: %d (incorrect)\n", filenames[processed_executables - batch_size + finished], atoi(parameters[i]));
                    }
                } else if (WIFSIGNALED(status)) {
                    // Crashed
                    fprintf(out_file, "%s: %d (crash)\n", filenames[processed_executables - batch_size + finished], atoi(parameters[i]));
                }
                finished += 1;
            }
        }
    }

    // Free allocated memory for file names
    for (int i = 0; i < total_executables; i++) {
        free(filenames[i]);
    }

    fclose(out_file);
    return 0;
}