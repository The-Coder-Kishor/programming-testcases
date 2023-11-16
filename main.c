#include <stdio.h>
#include <string.h>
#include "matrix.h"

#define MAX_FILEPATH_LENGTH 2048
#define MAX_COMMAND_LENGTH 18 // This is the max length of a command/operation
#define MAX_LOG_LINE_LENGTH (5 + MAX_COMMAND_LENGTH + 2) // Includes "LOG::" & op_mode

// By setting this macro to:
// 0 - the program exits due to keyboard interrupt or after specified 
// 1 - the command 'exit' exits the program as well. This command is not logged.
#define ALTERNATE_EXIT 0

// Set this macro to:
// 0 - to output only "ERROR: INVALID ARGUMENT" for all errors caused
// 1 - to output verbose error messages, along with the above, describing the issue
#define VERBOSE_ERROR_MESSAGES 0

typedef enum {InputError = 1, InternalError = 2} Error;
char ErrorTitle[3][17] = {"", "INVALID ARGUMENT", "INTERNAL ERROR"};

void raise_error(Error error_code, const char* msg) {
    if (error_code == InputError ||
        (error_code == InternalError && VERBOSE_ERROR_MESSAGES)) {
        printf("ERROR: %s\n", ErrorTitle[error_code]);
    }
    
    if (VERBOSE_ERROR_MESSAGES) printf("%s\n", msg);
}

void log_command(const char *command, const char *io_suffix) {
    // Opens/creates log file, and print "LOG::" + command + io_suffix

    char line[MAX_LOG_LINE_LENGTH + 1] = "LOG::";
    strcat(line, command);
    strcat(line, io_suffix);
    
    // Open/create log file

    FILE *logfile = fopen("mx_history", "a");
    if (!logfile) logfile = fopen("mx_history", "w");

    if (!logfile) {
        // The file could still not be created
        raise_error(InternalError, "The log file 'mx_history' could not be created. Will reattempt next time.");
        return;
    }

    // Log the command onto the file
    fprintf(logfile, "%s\n", line);

    fclose(logfile);
}

void print_history() {
    // mx_history is ensured to exist when this function is called
    
    FILE *logfile = fopen("mx_history", "r");

    // Sanity check to ensure logfile exists
    if (!logfile) {
        raise_error(InternalError, "The log file 'mx_history' does not exist.\n"
                                   "Possible reason: the program was unable to create the file earlier.");
        return;
    }

    // Print the lines from the log file
    fseek(logfile, 0L, SEEK_END);
    long long int filesize = ftell(logfile);

    fseek(logfile, 0L, SEEK_SET);

    char contents[filesize + 1];
    fread(contents, 1, filesize, logfile);
    printf(contents);
    
    fclose(logfile);
}

Matrix *read_matrix_from_file(char in_file_path[MAX_FILEPATH_LENGTH + 1]) {
    FILE *in_file;

    if (!strcmp(in_file_path, "<stdin>")) {
        in_file = stdin;
    }
    else {
        in_file = fopen(in_file_path, "r");
        if (!in_file) {
            char* msg;
            sprintf(msg,
                "Could not access file. The file at '%s' may not exist, or the file could be "
                "unavailable/busy, or the program may not have permission to access the file."
                , in_file_path);
            raise_error(InputError, msg);
            return NULL;
        }
    }

    // Read matrix
    // Assuming that if file exists, its contents exist in the desired format.
    // Incorrect format may lead to undefined behaviour.
    // Input via stdin is waited upon till it receives input in the desired format.

    int num_rows = 0, num_cols = 0; // '= 0' to be a little extra cautious to avoid issues
    scanf("%d %d", &num_rows, &num_cols);

    Matrix *M = create_matrix(num_rows, num_cols);

    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            fscanf(in_file, "%lld", &(M->data[r][c]));
        }
    }

    if (in_file != stdin) fclose(in_file);

    return M;
}

void write_matrix_to_file(char out_file_path[MAX_FILEPATH_LENGTH + 1], Matrix *M) {
    FILE *out_file;

    if (!strcmp(out_file_path, "<stdout>")) {
        out_file = stdout;
    }
    else {
        // Overwrites the file in case it already exists
        
        out_file = fopen(out_file_path, "w");
        if (!out_file) {
            char* msg;
            sprintf(msg,
                "Could not create file. There may already exist a file at '%s' which is unavailable/busy, "
                "or the program may not have permission to modify the file.",
                out_file_path);
            raise_error(InputError, msg);
            return;
        }
        
        // Enhancing the functionality of print_matrix by using a file pointer
        
        int num_rows = M->num_rows, num_cols = M->num_cols;
    
        fprintf(out_file, "%d %d\n", num_rows, num_cols);
        for (int i = 0; i < num_rows; i++) {
            for (int j = 0; j < num_cols; j++) {
                fprintf(out_file, "%lld ", M->data[i][j]);
            }
            fprintf(out_file, "\n");
        }

        fclose(out_file);
    }
}

int main() {
    #define cmd_exit "exit"
    #define cmd_history "history"
    #define cmd_add_matrix "add_matrix"
    #define cmd_mult_matrix "mult_matrix"
    #define cmd_scalar_mult_matrix "scalar_mult_matrix"
    #define cmd_transpose_matrix "transpose_matrix"
    #define cmd_determinant "determi"

    int q = 0;
    scanf("%d", &q);

    char command[MAX_COMMAND_LENGTH + 1];
    int io_mode; // Can be 0 or 1
    
    while (q--) {
        scanf(" %s", command);

        if (ALTERNATE_EXIT && !strcmp(command, cmd_exit)) {
            return 0;
        }

        if (!strcmp(command, cmd_history)) {
            log_command(cmd_history, "");
            print_history();
        }

        else if (!strcmp(command, cmd_add_matrix) ||
                 !strcmp(command, cmd_mult_matrix) ||
                 !strcmp(command, cmd_scalar_mult_matrix) ||
                 !strcmp(command, cmd_transpose_matrix) ||
                 !strcmp(command, cmd_determinant)) {
            
            // First get the io_mode and log the command
            scanf("%d", io_mode);
            if (io_mode == 0) log_command(command, " 0");
            else if (io_mode == 1) log_command(command, " 1");
            else raise_error(InputError, "Incorrect I/O mode entered. Should be either 0 or 1.");

            // Now running the commands for real
            if (!strcmp(command, cmd_add_matrix)) {
                // -- Operation chosen: Add matrices --
                
                char in_file1_path[MAX_FILEPATH_LENGTH + 1];
                char in_file2_path[MAX_FILEPATH_LENGTH + 1];
                char out_file_path[MAX_FILEPATH_LENGTH + 1];

                if (io_mode == 0) {
                    strcpy(in_file1_path, "<stdin>");
                    strcpy(in_file2_path, "<stdin>");
                    strcpy(out_file_path, "<stdout>");
                }
                else {
                    // It is given that all filepaths are alphanumeric (includes '_')
                    scanf(" %s %s %s", in_file1_path, in_file2_path, out_file_path);
                }

                Matrix *A = read_matrix_from_file(in_file1_path);
                Matrix *B = read_matrix_from_file(in_file2_path);
                if (!A || !B) continue;

                Matrix *Y = add_matrix(A, B);
                if (!Y) raise_error(InputError, "Cannot add the matrices. Orders do not match for matrix addition.");
                else write_matrix_to_file(out_file_path, Y);

                destroy_matrix(A);
                destroy_matrix(B);
                destroy_matrix(Y);
            }

            else if (!strcmp(command, cmd_mult_matrix)) {
                // -- Operation chosen: Multiply matrices --
                
                char in_file1_path[MAX_FILEPATH_LENGTH + 1];
                char in_file2_path[MAX_FILEPATH_LENGTH + 1];
                char out_file_path[MAX_FILEPATH_LENGTH + 1];

                if (io_mode == 0) {
                    strcpy(in_file1_path, "<stdin>");
                    strcpy(in_file2_path, "<stdin>");
                    strcpy(out_file_path, "<stdout>");
                }
                else {
                    // It is given that all filepaths are alphanumeric (includes '_')
                    scanf(" %s %s %s", in_file1_path, in_file2_path, out_file_path);
                }

                Matrix *A = read_matrix_from_file(in_file1_path);
                Matrix *B = read_matrix_from_file(in_file2_path);
                if (!A || !B) continue;

                Matrix *Y = mult_matrix(A, B);
                if (!Y) raise_error(InputError, "Cannot multiply the matrices. Orders do not match for matrix multiplication.");
                else write_matrix_to_file(out_file_path, Y);

                destroy_matrix(A);
                destroy_matrix(B);
                destroy_matrix(Y);
            }

            else if (!strcmp(command, cmd_scalar_mult_matrix)) {
                // -- Operation chosen: Multiply matrix by a scalar --

                char in_file_path[MAX_FILEPATH_LENGTH + 1];
                char out_file_path[MAX_FILEPATH_LENGTH + 1];
                
                long long int scalar;
                scanf("%lld", &scalar);

                if (io_mode == 0) {
                    strcpy(in_file_path, "<stdin>");
                    strcpy(out_file_path, "<stdout>");
                }
                else {
                    // It is given that all filepaths are alphanumeric (includes '_')
                    scanf(" %s %s", in_file_path, out_file_path);
                }

                Matrix *A = read_matrix_from_file(in_file_path);
                if (!A) continue;

                Matrix *Y = scalar_mult_matrix(scalar, A);
                write_matrix_to_file(out_file_path, Y);

                destroy_matrix(A);
                destroy_matrix(Y);
            }

            else if (!strcmp(command, cmd_transpose_matrix)) {
                // -- Operation chosen: Transpose matrix --

                char in_file_path[MAX_FILEPATH_LENGTH + 1];
                char out_file_path[MAX_FILEPATH_LENGTH + 1];
                
                if (io_mode == 0) {
                    strcpy(in_file_path, "<stdin>");
                    strcpy(out_file_path, "<stdout>");
                }
                else {
                    // It is given that all filepaths are alphanumeric (includes '_')
                    scanf(" %s %s", in_file_path, out_file_path);
                }

                Matrix *A = read_matrix_from_file(in_file_path);
                if (!A) continue;

                Matrix *Y = transpose_matrix(A);
                write_matrix_to_file(out_file_path, Y);

                destroy_matrix(A);
                destroy_matrix(Y);
            }

            else if (!strcmp(command, cmd_determinant)) {
                // -- Operation chosen: Transpose matrix --

                char in_file_path[MAX_FILEPATH_LENGTH + 1];
            
                if (io_mode == 0) {
                    strcpy(in_file_path, "<stdin>");
                }
                else {
                    // It is given that all filepaths are alphanumeric (includes '_')
                    scanf(" %s", in_file_path);
                }

                Matrix *A = read_matrix_from_file(in_file_path);
                if (!A) continue;

                // Check if the matrix is a square matrix
                if (A->num_rows != A->num_cols) raise_error(InputError, "Cannot find determinant. The input matrix "
                                                                  "is not a square matrix.");
                
                long long int det = determinant(A);
                printf("%lld\n", det);

                destroy_matrix(A);
            }
        }

        else {
            raise_error(InputError, "Invalid operation chosen. Enter operation from among 'add_matrix', 'mult_matrix', "
                                     "'scalar_mult_matrix', 'transpose_matrix', 'determinant', and 'history'.");
        }
    }
    
    return 0;
}