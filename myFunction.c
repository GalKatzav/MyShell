#include "myFunction.h"
#include <unistd.h> // for gethostname, getcwd
#include <stdio.h>

#define BUFF_SIZE 255

char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    // Allocate memory for a single character
    char *str = (char *)malloc(size * sizeof(char));
    // Loop until newline character is encountered
    while ((ch = getchar()) != '\n')
    {
        // Store the character in the strin
        *(str + index) = ch;
        // Increase the size of the string
        size++;
        index++;
        // Reallocate memory for the string
        str = (char *)realloc(str, size * sizeof(char));
    }
    // Terminate the string with null character
    *(str + index) = '\0';

    return str;
}

char *strwok(char *str, const char *delim)
{
    static char *next_token = NULL;
    if (str)
        next_token = str; // Initialize on first call
    if (!next_token || *next_token == '\0')
        return NULL; // End of string

    char *token_start = next_token; // Start of the next token
    char *current;                  // Current character being checked

    // Skip leading delimiters
    for (current = next_token; *current != '\0'; current++)
    {
        const char *d;
        for (d = delim; *d != '\0'; d++)
        {
            if (*current == *d)
            {

                token_start++;
                break;
            }
        }
        if (*d == '\0')
            break; // Found the start of a token
    }
    if (*token_start == '\0')
    {
        next_token = token_start; // Prepare for the next call to return NULL
        return NULL;
    }
    next_token = token_start; // Begin searching for the end of the token
    for (current = token_start; *current != '\0'; current++)
    {
        const char *d;
        for (d = delim; *d != '\0'; d++)
        {
            if (*current == *d)
            {
                // Found the end of the token
                *current = '\0';          // Terminate the token
                next_token = current + 1; // Set up for the next token
                return token_start;       // Return the current token
            }
        }
    }

    // Reached the end of the string
    next_token = current; // No more tokens
    return token_start;   // Return the last token
}

char **splitArgument(char *str)
{
    // Handling paths with spaces within double quotes
    //[cp, "file path with spaces", "destination path with spaces", NULL]

    char **arguments = (char **)malloc(100 * sizeof(char *));
    int argc = 0;
    char *token = strwok(str, " ");
    while (token != NULL)
    {
        if (token[0] == '"')
        {
            char temp[1000] = "";
            strcat(temp, token + 1);
            while (token[strlen(token) - 1] != '"' && (token = strwok(NULL, " ")) != NULL)
            {
                strcat(temp, " ");
                strcat(temp, token);
            }
            // Remove the closing double quote
            temp[strlen(temp) - 1] = '\0';
            arguments[argc] = strdup(temp);
        }
        else
        {
            arguments[argc] = strdup(token);
        }
        argc++;
        token = strwok(NULL, " ");
    }
    arguments[argc] = NULL;

    return arguments;
}

void getLocation()
{
    char location[BUFF_SIZE]; // Chars array
    char hostname[BUFF_SIZE]; // Chars array
    char *userName;

    // This function gets the current working directory and stores it in the buffer `location`.
    //  The function returns `NULL` if an error occurs.
    // This condition checks if `getcwd()` returns `NULL`, indicating an error while retrieving the current working directory.
    if (getcwd(location, BUFF_SIZE) == NULL)
    {
        puts("Error");
        return;
    }
    // This function gets the hostname of the computer and stores it in the buffer `hostname`. It returns `-1` if an error occurs.
    // This checks if `gethostname()` returns `-1`, indicating an error while retrieving the hostname.
    if (gethostname(hostname, BUFF_SIZE) == -1)
    {
        puts("Error getting hostname");
        return;
    }

    // This function retrieves the username of the currently logged-in user. It returns `NULL` if it fails to determine the username.
    userName = getlogin();
    // This condition checks if `getlogin()` failed to retrieve the username.
    // If so, it attempts to retrieve the username using the `getenv("USER")` function,
    // which gets the value of the environment variable "USER" (typically set to the username).
    // If both methods fail, it prints "Error getting userName" and returns from the function.
    if (userName == NULL)
    {
        userName = getenv("USER");
    }
    else
    {
        puts("Error getting userName");
        return;
    }

    // Print the username and hostname in green
    green();
    printf("%s@%s", userName, hostname);
    reset();

    // Print the path in blue
    blue();
    printf(":%s", location);
    reset();

    // Print the dollar sign and allow user to enter a string
    bold();
    printf("$ ");
    reset();

    // Reset colors and formatting
    reset();
}

void logout(char *input)
{
    free(input);
    puts("log out");
    exit(EXIT_SUCCESS); // EXIT_SUCCESS = 0
}

void echo(char **arg)
{
    while (*(++arg))
        printf("%s ", *arg);
    puts("");
}

void cd(char **arg)
{
    char path[1024] = {0}; // Adjust size as needed

    if (arg[1] == NULL)
    {
        printf("-myShell: cd: missing operand\n");
        return;
    }

    // Direct handling for "cd .."
    if (strcmp(arg[1], "..") == 0 && arg[2] == NULL)
    {
        if (chdir("..") != 0)
        {
            perror("-myShell: cd: error changing to parent directory");
        }
        return;
    }

    // For quoted paths or paths without spaces
    if (arg[1][0] == '\"')
    {
        // Quoted path
        strncpy(path, arg[1] + 1, sizeof(path) - 1); // Copy without the leading quote
        size_t path_len = strlen(path);
        if (path[path_len - 1] == '\"')
        {
            path[path_len - 1] = '\0'; // Remove trailing quote
        }
        else if (arg[2] != NULL)
        { // Handle case where path is split across arguments
            for (int i = 2; arg[i] != NULL; i++)
            {
                strcat(path, " ");
                strncat(path, arg[i], sizeof(path) - strlen(path) - 1);
                if (arg[i][strlen(arg[i]) - 1] == '\"')
                {
                    path[strlen(path) - 1] = '\0'; // Remove trailing quote and break
                    break;
                }
            }
        }
    }
    else
    {
        // Non-quoted but possibly space-containing path
        strncpy(path, arg[1], sizeof(path) - 1);
        for (int i = 2; arg[i] != NULL; i++)
        {
            if (strlen(path) + strlen(arg[i]) + 2 > sizeof(path))
            {
                printf("-myShell: cd: path is too long\n");
                return;
            }
            strcat(path, " ");
            strcat(path, arg[i]);
        }
    }

    // Attempt to change directory
    if (chdir(path) != 0)
    {
        printf("-myShell: cd: %s: No such file or directory\n", path);
    }
}

void cp(char **arguments)
{
    char ch;
    FILE *src, *des;
    // Open the source file for reading
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        // Print an error message if the source file cannot be opened
        puts("Erorr");
        return;
    }
    // Open the destination file for writing
    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        // Print an error message if the destination file cannot be opened
        puts("Erorr");
        fclose(src);
        return;
    }
    // Copy contents from source file to destination file character by character
    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }
    // Close both files after copying is done
    fclose(src);
    fclose(des);
}

void delete(char **path)
{
    // Concatenate path arguments into a single string
    int len = 0;
    for (int i = 1; path[i] != NULL; i++)
    {
        len += strlen(path[i]);
        if (path[i + 1] != NULL)
            len++; // Space
    }

    char *full_path = (char *)malloc(len + 1); // +1 for null terminator
    full_path[0] = '\0';                       // Start with an empty string

    for (int i = 1; path[i] != NULL; i++)
    {
        strcat(full_path, path[i]);
        if (path[i + 1] != NULL)
        {
            strcat(full_path, " "); // Space between path components
        }
    }

    if (unlink(full_path) != 0)
    {
        printf("-myShell: path: %s: No such file or directory\n", full_path);
    }
    else
    {
        printf("File deleted successfully: %s\n", full_path);
    }

    free(full_path);
}

void systemCall(char **arg)
{
    // Create a child process
    pid_t pid = fork();
    // Check for fork error
    if (pid == -1)
    {
        printf("fork err\n");
        return;
    }
    // Child process
    // Execute the command with the given arguments
    // If execvp returns, it means an error occurred
    if (pid == 0 && execvp(arg[0], arg) == -1)
        exit(1);
}

char **splitInput(char *input, int *pipeIndex)
{
    // Allocate memory for the array of strings
    char **arguments = malloc(strlen(input) * sizeof(char *));
    if (arguments == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize variables
    int argCount = 0;
    int index = 0;
    *pipeIndex = -1;

    // Iterate through the input string
    for (int i = 0; input[i] != '\0'; i++)
    {
        // If '|' is found, set pipeIndex and break
        if (input[i] == '|')
        {
            *pipeIndex = i;
            break;
        }
        // Otherwise, add characters to the current argument
        if (input[i] != ' ')
        {
            int start = i;
            // Find the end of the current argument
            while (input[i] != ' ' && input[i] != '|' && input[i] != '\0')
                i++;
            int end = i;
            // Allocate memory for the current argument
            arguments[argCount] = malloc((end - start + 1) * sizeof(char));
            if (arguments[argCount] == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            // Copy characters to the current argument
            strncpy(arguments[argCount], input + start, end - start);
            arguments[argCount][end - start] = '\0';
            // Increment argument count
            argCount++;
        }
    }

    // Null-terminate the array of strings
    arguments[argCount] = NULL;

    return arguments;
}

char **splitAfterPipe(char *input, int pipeIndex)
{
    // Allocate memory for the array of strings
    char **arguments = malloc(strlen(input) * sizeof(char *));
    if (arguments == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize variables
    int argCount = 0;

    // Iterate through the input string starting from pipeIndex + 1
    for (int i = pipeIndex + 1; input[i] != '\0'; i++)
    {
        // Skip leading spaces after '|'
        while (input[i] == ' ')
            i++;
        // Add characters to the current argument
        if (input[i] != '\0')
        {
            int start = i;
            // Find the end of the current argument
            while (input[i] != ' ' && input[i] != '\0')
                i++;
            int end = i;
            // Allocate memory for the current argument
            arguments[argCount] = malloc((end - start + 1) * sizeof(char));
            if (arguments[argCount] == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            // Copy characters to the current argument
            strncpy(arguments[argCount], input + start, end - start);
            arguments[argCount][end - start] = '\0';
            // Increment argument count
            argCount++;
        }
    }

    // Null-terminate the array of strings
    arguments[argCount] = NULL;

    return arguments;
}

void mypipe(char **argv1, char **argv2)
{
    int fildes[2];
    if (fork() == 0)
    {
        pipe(fildes);
        if (fork() == 0)
        {
            /* first component of command line */
            close(STDOUT_FILENO);
            dup(fildes[1]);
            close(fildes[1]);
            close(fildes[0]);
            /* stdout now goes to pipe */
            /* child process does command */
            execvp(argv1[0], argv1);
        }
        /* 2nd command component of command line */
        close(STDIN_FILENO);
        dup(fildes[0]);
        close(fildes[0]);
        close(fildes[1]);
        /* standard input now comes from pipe */
        execvp(argv2[0], argv2);
    }
}

void move(char **args)
{
    // Initialize source and destination paths
    char srcPath[2048] = {0};
    char destPath[2048] = {0};

    // Construct source path
    strcat(srcPath, args[1]);
    if (args[2] != NULL)
    {
        strcat(srcPath, " ");
        strcat(srcPath, args[2]);
    }
    // Construct destination path
    if (args[3] != NULL)
    {
        strcat(destPath, args[3]);
        if (args[4] != NULL)
        {
            strcat(destPath, " ");
            strcat(destPath, args[4]);
        }
    }

    // Print the attempted move operation
    printf("Attempting to move from '%s' to '%s'\n", srcPath, destPath);

    // Attempt to move/rename the file
    if (rename(srcPath, destPath) == 0)
    {
        printf("File successfully moved.\n");
    }
    else
    {
        // Print error message if the move operation fails
        perror("Error moving file");
    }
}

void echoppend(char **args)
{
    // Variables to hold the concatenated text and file path
    char textToAppend[2048] = {0};
    char filePath[2048] = {0};

    int redirectIndex = -1; // Index of ">>" in args
    for (int i = 0; args[i] != NULL; ++i)
    {
        if (strcmp(args[i], ">>") == 0)
        {
            redirectIndex = i;
            break;
        }
    }

    if (redirectIndex == -1)
    {
        fprintf(stderr, "Error: Command format incorrect. Expected '>>' for redirection.\n");
        return;
    }

    // Concatenate everything before ">>" as the text to append
    for (int i = 1; i < redirectIndex; ++i)
    {
        strcat(textToAppend, args[i]);
        if (i < redirectIndex - 1)
            strcat(textToAppend, " "); // Add space except for the last part
    }

    // Concatenate everything after ">>" as the file path
    for (int i = redirectIndex + 1; args[i] != NULL; ++i)
    {
        strcat(filePath, args[i]);
        if (args[i + 1] != NULL)
            strcat(filePath, " "); // Add space except for the last part
    }

    // Attempt to open the file for appending
    FILE *file = fopen(filePath, "a");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    // Append the text and a newline, then close the file
    fprintf(file, "%s\n", textToAppend);
    fclose(file);

    printf("Appended '%s' to '%s'.\n", textToAppend, filePath);
}

void echorite(char **args)
{
    char textToWrite[2048] = {0};
    char filePath[2048] = {0};

    // Finding the ">"
    int foundRedirection = 0;
    for (int i = 1; args[i] != NULL; i++)
    {
        if (strcmp(args[i], ">") == 0)
        {
            foundRedirection = 1;
            if (args[i + 1] != NULL)
            {
                // Construct file path from command arguments
                for (int j = i + 1; args[j] != NULL; j++)
                {
                    if (j > i + 1)
                        strcat(filePath, " ");
                    strcat(filePath, args[j]);
                }
            }
            else
            {
                fprintf(stderr, "File path not provided.\n");
                return;
            }
            break;
        }
        else
        {
            // Construct text to write from command arguments
            if (i > 1)
                strcat(textToWrite, " ");
            strcat(textToWrite, args[i]);
        }
    }
    // Check if ">" redirection operator is found
    if (!foundRedirection)
    {
        fprintf(stderr, "Redirection operator '>' not found.\n");
        return;
    }
    // Open file for writing
    FILE *file = fopen(filePath, "w");
    if (!file)
    {
        perror("Error opening file");
        return;
    }
    // Write text to file
    fprintf(file, "%s\n", textToWrite);
    fclose(file);

    printf("Content written to '%s'.\n", filePath);
}

void readd(char **args)
{
    // Check if file path is provided
    if (args[1] == NULL)
    {
        fprintf(stderr, "Usage: read <path>\n");
        return;
    }
    char filePath[2048] = {0};
    // Construct file path from command arguments
    for (int i = 1; args[i] != NULL; ++i)
    {
        if (i > 1)
            strcat(filePath, " ");
        strcat(filePath, args[i]);
    }
    // Open file for reading
    FILE *file = fopen(filePath, "r");
    if (!file)
    {
        perror("Error opening file");
        return;
    }
    // Read and print file contents line by line
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(file);
}

void wordCount(char **args)
{
    // Check if at least an option and part of a file path are provided
    if (args[1] == NULL || args[2] == NULL)
    {
        fprintf(stderr, "Usage: wc <-l/-w> <file path>\n");
        return;
    }

    // Extract option
    char *option = args[1];

    // Concatenate all remaining arguments to form the file path, handling spaces
    char file_path[2048] = {0};
    for (int i = 2; args[i] != NULL; ++i)
    {
        if (i > 2)
            strcat(file_path, " "); // Add spaces back between parts of the path
        strcat(file_path, args[i]);
    }

    // Open file for reading
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("File '%s' does not exist or cannot be opened.\n", file_path);
        return;
    }

    int count = 0; // Counter for lines or words

    // Count lines or words based on the selected option
    if (strcmp(option, "-l") == 0)
    {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            count++;
        }
        printf("Number of lines in the file: %d\n", count);
    }
    else if (strcmp(option, "-w") == 0)
    {
        int prev_char = ' ';
        int current_char;
        while ((current_char = fgetc(file)) != EOF)
        {
            if (current_char == ' ' || current_char == '\n' || current_char == '\t')
            {
                if (prev_char != ' ' && prev_char != '\n' && prev_char != '\t')
                {
                    count++;
                }
            }
            prev_char = current_char;
        }
        // Check for the last word if not ending in space/newline
        if (prev_char != ' ' && prev_char != '\n' && prev_char != '\t')
        {
            count++;
        }
        printf("Number of words in the file: %d\n", count);
    }
    else
    {
        fprintf(stderr, "Invalid option. Usage: wc <-l/-w> <file path>\n");
    }

    // Close file
    fclose(file);
}