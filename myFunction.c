#include "myFunction.h"

char *getInputFromUser()
{
    // second
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
void blue()
{

    printf("\033[0;34m");
}
void reset()
{

    printf("\033[0m");
}
char **splitArgument(char *str)
{
    char *subStr;
    int size = 2;
    int index = 0;
    subStr = strtok(str, " ");

    char **arguments = (char **)malloc(size * sizeof(char *));

    *(arguments + index) = subStr;

    while ((subStr = strtok(NULL, " ")) != NULL)
    {
        size++;
        index++;
        *(arguments + index) = subStr;
        arguments = (char **)realloc(arguments, size * sizeof(char *));
    }
    *(arguments + (index + 1)) = NULL;
    return arguments;
}

void getLocation()
{
    char location[BUFF_SIZE];

    if (getcwd(location, BUFF_SIZE) == NULL)
    {
        puts("Error");
    }
    else
    {
        blue();
        puts(location);
        reset();
    }
}