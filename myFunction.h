#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

#define BUFF_SIZE 256
#define blue() printf("\033[0;34m")
#define reset() printf("\033[0m")
#define green() printf("\033[0;32m")
#define bold() printf("\e[1m")
#define boldOff() printf("\e[m")

/**
 * Reads input from the user until a newline character is encountered.
 *
 * This function reads input from the standard input until a newline character
 * is encountered. It dynamically allocates memory to store the input string
 * and reallocates memory as needed while reading characters. The caller is
 * responsible for freeing the memory allocated for the string when it's no
 * longer needed.
 *
 * @return A pointer to the dynamically allocated string containing the input
 *         from the user, terminated by a null character. Returns NULL if memory
 *         allocation fails or if no input is provided.
 */
char *getInputFromUser();
// cp file file
// [cp,file,file]

/**
 * Splits a string into individual tokens based on whitespace.
 *
 * This function takes a string as input and tokenizes it based on whitespace
 * characters (space, tab, newline). It dynamically allocates memory for an
 * array of strings to store the tokens. The caller is responsible for freeing
 * the memory allocated for the array and its elements when it's no longer needed.
 *
 * @param input A pointer to the string to be split.
 *
 * @return A dynamically allocated array of strings (char**) containing the tokens.
 *         The last element of the array is NULL. Returns NULL if input is NULL
 *         or if memory allocation fails.
 *
 * @note The input string will be modified (tokenized) by this function.
 *       Make sure to use a copy of the original string if preservation
 *       of the original string is required.
 *
 * @warning The caller is responsible for freeing the memory allocated for
 *          the returned array and its elements using free().
 */
char **splitArgument(char *);

/**
 * Retrieves the current user's location, username, and hostname.
 *
 * This function retrieves the current working directory, username, and hostname
 * of the user and prints them to the standard output. It also provides a prompt
 * for the user to enter commands. The colors and formatting of the printed
 * elements are configured using ANSI escape codes.
 *
 * @note This function relies on system functions like getcwd(), getlogin(),
 *       and gethostname() to retrieve information about the user's environment.
 *
 * @warning This function assumes that BUFF_SIZE is defined and that it is large
 *          enough to hold the necessary information. Ensure BUFF_SIZE is appropriately
 *          defined before using this function.
 */
void getLocation();

/**
 * Logs out the current user and exits the program.
 *
 * This function frees the memory allocated for the input string and then
 * prints a message indicating that the user has logged out. It exits the
 * program with a success status code.
 *
 * @param input A pointer to the input string to be freed.
 *
 * @warning The caller must ensure that the provided input string was allocated
 *          using dynamic memory allocation functions like malloc(), calloc(), or realloc().
 *          Behavior is undefined if the input string is not allocated dynamically
 *          or if it has already been freed before calling this function.
 *          The function relies on system functions like free() and puts(). Ensure
 *          that these functions behave as expected on the target system.
 *          Behavior is undefined if the input string is modified or freed while
 *          in use by this function.
 */
void logout(char *);

/**
 * Prints the elements of an array of strings.
 *
 * This function iterates through an array of strings and prints each string
 * followed by a space. After printing all strings, it prints a newline character.
 *
 * @param arg A pointer to an array of strings (char**) to be printed.
 *
 * @warning The caller must ensure that the provided array of strings is properly
 *          terminated with a NULL pointer. Behavior is undefined if the array is
 *          NULL or if any of its elements are NULL.
 *          The function relies on system functions like printf() and puts(). Ensure
 *          that these functions behave as expected on the target system.
 *          Behavior is undefined if the input strings are modified or freed while
 *          in use by this function.
 */
void echo(char **);

/**
 * Changes the current working directory.
 *
 * This function changes the current working directory based on the provided
 * path. It supports both quoted paths and paths without spaces. If the provided
 * path contains spaces, it concatenates them into a single path. If the path is
 * "..", it changes to the parent directory. It prints error messages if the
 * directory change fails or if the path is invalid.
 *
 * @param arg A pointer to an array of strings (char**) containing the command
 *            and the path to change to.
 *
 * @warning The caller must ensure that the input array is properly formatted
 *          and contains valid strings. Behavior is undefined if the input array
 *          is NULL or if any of its elements are NULL.
 *          The caller is responsible for ensuring that the provided path is
 *          within the bounds of the system's maximum path length.
 *          The function relies on system functions like chdir(), strcmp(),
 *          strncpy(), strcat(), and strncat(). Ensure that these functions behave
 *          as expected on the target system.
 *          Behavior is undefined if the input path exceeds the maximum allowable
 *          length for a file path.
 *          Behavior is undefined if the input path is modified while in use by
 *          this function.
 */
void cd(char **);

/**
 * Copies the contents of one file to another.
 *
 * This function opens the source file for reading and the destination file
 * for writing. It then copies the contents of the source file to the destination
 * file character by character until the end of file is reached. It prints error
 * messages if either file cannot be opened or if an error occurs during the copying
 * process.
 *
 * @param arguments A pointer to an array of strings (char**) containing the command
 *                  and the paths of the source and destination files.
 *
 * @note Ensure that the source file exists and is accessible for reading,
 *       and that the destination file can be created or overwritten for writing.
 *       The function does not perform any checks on the validity of the file paths
 *       or the content of the files being copied.
 *       It is the responsibility of the caller to ensure proper error handling
 *       and validation of file paths and contents.
 *
 * @warning The caller must ensure that the provided array of strings is properly
 *          formatted and contains valid paths for the source and destination files.
 *          Behavior is undefined if the array is NULL or if any of its elements are NULL.
 *          The function relies on system functions like fopen(), fclose(), fgetc(), and
 *          fputc(). Ensure that these functions behave as expected on the target system.
 *          Behavior is undefined if the input file paths are modified or freed while
 *          in use by this function.
 */
void cp(char **);

/**
 * Deletes a file specified by the provided path.
 *
 * This function concatenates the path arguments into a single string and
 * attempts to delete the file specified by that path. It prints error messages
 * if the file does not exist or if an error occurs during the deletion process.
 * Upon successful deletion, it prints a success message.
 *
 * @param path A pointer to an array of strings (char**) containing the command
 *             and the path of the file to be deleted.
 *
 * @note Ensure that the file path is valid and that the file exists before
 *       attempting to delete it. The function does not perform any checks on
 *       the validity of the file path or whether the file is a directory.
 *       It is the responsibility of the caller to ensure proper error handling
 *       and validation of the file path.
 *
 * @warning The caller must ensure that the provided array of strings is properly
 *          formatted and contains a valid path for the file to be deleted.
 *          Behavior is undefined if the array is NULL or if any of its elements are NULL.
 *          The function relies on system functions like unlink() and printf().
 *          Ensure that these functions behave as expected on the target system.
 *          Behavior is undefined if the input file path is modified or freed while
 *          in use by this function.
 */
void delete(char **);

/**
 * Executes a system command specified by the provided arguments.
 *
 * This function creates a child process using fork() and attempts to execute
 * a system command specified by the provided arguments using execvp(). If fork()
 * or execvp() encounters an error, it prints an error message. The child process
 * exits with a status of 1 if execvp() fails.
 *
 * @param arg A pointer to an array of strings (char**) containing the command
 *            and its arguments.
 *
 * @note Ensure that the provided command and arguments are valid. The function
 *       does not perform any validation on the command or arguments.
 *       It is the responsibility of the caller to ensure proper error handling
 *       and validation of the command and arguments.
 *
 * @warning The caller must ensure that the provided array of strings is properly
 *          formatted and contains a valid command followed by its arguments.
 *          Behavior is undefined if the array is NULL or if any of its elements are NULL.
 *          The function relies on system functions like fork(), execvp(), and exit().
 *          Ensure that these functions behave as expected on the target system.
 *          Behavior is undefined if the input command or arguments are modified or freed
 *          while in use by this function.
 */
void systemCall(char **);

/**
 * Splits the input string up to the first '|' character.
 *
 * This function splits the input string into individual arguments up to the first
 * occurrence of the '|' character. It allocates memory for the array of strings
 * to store the arguments. If memory allocation fails, it prints an error message
 * and exits the program. It returns the array of strings containing the arguments
 * and updates the pipeIndex parameter with the index of the '|' character.
 *
 * @param input A pointer to the input string to be split.
 * @param pipeIndex A pointer to an integer to store the index of the first '|' character.
 *
 * @return A dynamically allocated array of strings (char**) containing the arguments
 *         up to the first '|' character. The last element of the array is NULL.
 *
 * @note The input string will be modified by this function as it tokenizes the string.
 *       Make sure to use a copy of the original string if preservation of the original
 *       string is required.
 *
 * @warning The caller is responsible for freeing the memory allocated for the returned
 *          array and its elements using free(). The function relies on system functions
 *          like malloc(), fprintf(), and exit(). Ensure that these functions behave
 *          as expected on the target system. Behavior is undefined if the input string
 *          is modified or freed while in use by this function.
 */
char **splitInput(char *, int *);

/**
 * Splits the input string after the first '|' character.
 *
 * This function splits the input string into individual arguments after the first
 * occurrence of the '|' character. It allocates memory for the array of strings
 * to store the arguments. If memory allocation fails, it prints an error message
 * and exits the program. It returns the array of strings containing the arguments
 * after the first '|' character.
 *
 * @param input A pointer to the input string to be split.
 * @param pipeIndex The index of the first '|' character in the input string.
 *
 * @return A dynamically allocated array of strings (char**) containing the arguments
 *         after the first '|' character. The last element of the array is NULL.
 *
 * @note The input string will not be modified by this function. It starts tokenizing
 *       the string after the first '|' character.
 *
 * @warning The caller is responsible for freeing the memory allocated for the returned
 *          array and its elements using free(). The function relies on system functions
 *          like malloc(), fprintf(), and exit(). Ensure that these functions behave
 *          as expected on the target system. Behavior is undefined if the input string
 *          is modified or freed while in use by this function.
 */
char **splitAfterPipe(char *, int);

/**
 * Creates a pipe and connects the output of one command to the input of another.
 *
 * This function creates a pipe using the pipe() system call and forks two child processes.
 * The first child process executes the command specified by the argv1 argument and sends
 * its output to the write end of the pipe. The second child process executes the command
 * specified by the argv2 argument and reads its input from the read end of the pipe.
 * The parent process waits for both child processes to complete. If any system calls fail,
 * it prints an error message.
 *
 * @param argv1 A pointer to an array of strings (char**) containing the command and its arguments
 *              for the first component of the command line.
 * @param argv2 A pointer to an array of strings (char**) containing the command and its arguments
 *              for the second component of the command line.
 *
 * @note Ensure that both argv1 and argv2 are properly terminated with a NULL pointer.
 *       The function does not perform any checks on the validity of the command line components.
 *       It is the responsibility of the caller to ensure proper error handling and validation
 *       of the command line components.
 *
 * @warning The caller is responsible for ensuring that the pipe is properly closed and that
 *          both child processes are properly terminated. Behavior is undefined if the provided
 *          arrays of strings are NULL or if any of their elements are NULL.
 *          The function relies on system functions like fork(), pipe(), close(), dup(), and execvp().
 *          Ensure that these functions behave as expected on the target system. Behavior is undefined
 *          if the input arrays of strings are modified or freed while in use by this function.
 */
void mypipe(char **, char **);

/**
 * Moves or renames a file specified by the source path to the destination path.
 *
 * This function constructs the source and destination paths using the arguments
 * provided in the args array. It then attempts to move or rename the file specified
 * by the source path to the destination path using the rename() system call. It prints
 * error messages if the move operation fails.
 *
 * @param args A pointer to an array of strings (char**) containing the command and its arguments.
 *             args[1] and args[2] specify the source path, and args[3] and args[4] specify the
 *             destination path. If args[2] or args[4] is NULL, it is ignored.
 *
 * @note Ensure that the provided source and destination paths are valid and that the file
 *       specified by the source path exists. The function does not perform any checks on
 *       the validity of the paths or whether the source file exists.
 *       It is the responsibility of the caller to ensure proper error handling and validation
 *       of the paths.
 *
 * @warning The caller is responsible for ensuring that both source and destination paths
 *          are properly formatted and terminated with a NULL pointer. Behavior is undefined
 *          if the provided arrays of strings are NULL or if any of their elements are NULL.
 *          The function relies on system functions like strcat(), printf(), perror(), and rename().
 *          Ensure that these functions behave as expected on the target system. Behavior is undefined
 *          if the input arrays of strings are modified or freed while in use by this function.
 */
void move(char **);

/**
 * Appends text to a file specified by the file path.
 *
 * This function extracts the text to append and the file path from the provided
 * arguments. It searches for the ">>" symbol in the args array to determine the
 * separation between the text to append and the file path. If the ">>" symbol
 * is not found or if the command format is incorrect, it prints an error message
 * and returns. Otherwise, it opens the file specified by the file path in append mode
 * using fopen(). If the file opening fails, it prints an error message and returns.
 * It appends the text followed by a newline character to the file using fprintf(),
 * then closes the file. It prints a success message if the operation is successful.
 *
 * @param args A pointer to an array of strings (char**) containing the command and its arguments.
 *             The text to append should appear before ">>", and the file path should appear after ">>".
 *
 * @note Ensure that the provided file path is valid and that the file can be opened for appending.
 *       The function does not perform any checks on the validity of the file path or whether
 *       the file exists. It is the responsibility of the caller to ensure proper error handling
 *       and validation of the file path.
 *
 * @warning The caller is responsible for ensuring that the args array is properly formatted and
 *          terminated with a NULL pointer. Behavior is undefined if the args array is NULL
 *          or if any of its elements are NULL. The function relies on system functions like
 *          strcat(), fopen(), fprintf(), fclose(), and perror(). Ensure that these functions
 *          behave as expected on the target system. Behavior is undefined if the input array
 *          of strings is modified or freed while in use by this function.
 */
void echoppend(char **);

/**
 * Writes text to a file specified by the file path, overwriting existing content.
 *
 * This function extracts the text to write and the file path from the provided
 * arguments. It searches for the ">" symbol in the args array to determine the
 * separation between the text to write and the file path. If the ">" symbol
 * is not found or if the command format is incorrect, it prints an error message
 * and returns. Otherwise, it opens the file specified by the file path in write mode
 * using fopen(). If the file opening fails, it prints an error message and returns.
 * It writes the text followed by a newline character to the file using fprintf(),
 * then closes the file. It prints a success message if the operation is successful.
 *
 * @param args A pointer to an array of strings (char**) containing the command and its arguments.
 *             The text to write should appear before ">", and the file path should appear after ">".
 *
 * @note Ensure that the provided file path is valid and that the file can be opened for writing.
 *       The function does not perform any checks on the validity of the file path.
 *       It is the responsibility of the caller to ensure proper error handling
 *       and validation of the file path.
 *
 * @warning The caller is responsible for ensuring that the args array is properly formatted and
 *          terminated with a NULL pointer. Behavior is undefined if the args array is NULL
 *          or if any of its elements are NULL. The function relies on system functions like
 *          strcat(), fopen(), fprintf(), fclose(), and perror(). Ensure that these functions
 *          behave as expected on the target system. Behavior is undefined if the input array
 *          of strings is modified or freed while in use by this function.
 */
void echorite(char **);

/**
 * Reads and prints the contents of a file specified by the file path.
 *
 * This function extracts the file path from the provided arguments and attempts
 * to open the file specified by the path in read mode using fopen(). If the file
 * opening fails or if no file path is provided, it prints an error message and returns.
 * It reads the contents of the file line by line using fgets() and prints each line
 * to the standard output using printf(). It then closes the file using fclose().
 *
 * @param args A pointer to an array of strings (char**) containing the command and its arguments.
 *             The file path should be provided as the second argument (args[1]).
 *
 * @note Ensure that the provided file path is valid and that the file exists and can be opened for reading.
 *       The function does not perform any checks on the validity of the file path or whether the file exists.
 *       It is the responsibility of the caller to ensure proper error handling and validation of the file path.
 *
 * @warning The caller is responsible for ensuring that the args array is properly formatted and
 *          terminated with a NULL pointer. Behavior is undefined if the args array is NULL
 *          or if any of its elements are NULL. The function relies on system functions like
 *          strcat(), fopen(), fgets(), printf(), and fclose(). Ensure that these functions
 *          behave as expected on the target system. Behavior is undefined if the input array
 *          of strings is modified or freed while in use by this function.
 */
void readd(char **);

/**
 * Counts the number of lines or words in a file based on the provided option.
 *
 * This function takes an array of strings containing the command and its arguments.
 * It expects at least an option (-l for lines or -w for words) and part of a file path.
 * If the required arguments are not provided, it prints a usage message to stderr
 * and returns. It extracts the option and concatenates the remaining arguments to
 * form the file path, handling spaces. It then attempts to open the file specified
 * by the file path in read mode using fopen(). If the file opening fails, it prints
 * an error message and returns. Depending on the selected option (-l or -w), it
 * counts the number of lines or words in the file. It prints the result to stdout
 * and closes the file using fclose().
 *
 * @param args A pointer to an array of strings (char**) containing the command and its arguments.
 *             The option should be provided as the second argument (args[1]), and the file path
 *             should follow (args[2] onwards).
 *
 * @note Ensure that the provided file path is valid and that the file exists and can be opened for reading.
 *       The function does not perform any checks on the validity of the file path or whether the file exists.
 *       It is the responsibility of the caller to ensure proper error handling and validation of the file path.
 *
 * @warning The caller is responsible for ensuring that the args array is properly formatted and
 *          terminated with a NULL pointer. Behavior is undefined if the args array is NULL
 *          or if any of its elements are NULL. The function relies on system functions like
 *          strcat(), fopen(), fgets(), fgetc(), printf(), and fclose(). Ensure that these functions
 *          behave as expected on the target system. Behavior is undefined if the input array
 *          of strings is modified or freed while in use by this function.
 */
void wordCount(char **);