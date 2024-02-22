#include "myFunction.h"
#include <unistd.h> // for gethostname, getcwd
#include <stdio.h>

#define BUFF_SIZE 255

char *getInputFromUser()
{
    char ch;
    int size = 1;
    int index = 0;
    char *str = (char *)malloc(size * sizeof(char));
    while ((ch = getchar()) != '\n')
    {
        *(str + index) = ch;
        size++;
        index++;
        str = (char *)realloc(str, size * sizeof(char));
    }
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
    // str = cp file file file
    //[cp,file,file,file,NULL]
    char *subStr;
    int size = 2;
    int index = 0;
    subStr = strwok(str, " ");
    char **argumnts = (char **)malloc(size * sizeof(char *));
    *(argumnts + index) = subStr;
    while ((subStr = strwok(NULL, " ")) != NULL)
    {
        size++;
        index++;
        *(argumnts + index) = subStr;
        argumnts = (char **)realloc(argumnts, size * sizeof(char *));
    }
    *(argumnts + (index + 1)) = NULL;

    return argumnts;
}

// This function retrieves the current working directory, hostname,
// and username of the currently logged-in user, and prints them in the format
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
    if ((src = fopen(arguments[1], "r")) == NULL)
    {
        puts("Erorr");
        return;
    }
    if ((des = fopen(arguments[2], "w")) == NULL)
    {
        puts("Erorr");
        fclose(src);
        return;
    }

    while ((ch = fgetc(src)) != EOF)
    {
        fputc(ch, des);
    }
    fclose(src);
    fclose(des);
}
