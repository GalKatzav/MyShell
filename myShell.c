#include "myShell.h"
#include "myFunction.h"

int main()
{

    welcome();
    while (1)
    {
        int piping = 0;
        int pipeIndex;
        getLocation();
        char *input = getInputFromUser();
        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0 || strncmp(input, " exit", 5) == 0 || strncmp(input, " exit ", 6) == 0)
            logout(input);
        char **arguments = splitArgument(input);
        int foundTwoRedirect = 0;
        int foundOneRedirect = 0;
        for (int i = 0; arguments[i] != NULL; i++)
        {
            if (strcmp(arguments[i], ">>") == 0)
            {
                foundTwoRedirect = 1;
                break;
            }
            else if (strcmp(arguments[i], ">") == 0)
            {
                foundOneRedirect = 1;
                break;
            }
        }

        if (foundTwoRedirect)
        {
            echoppend(arguments);
        }
        else if (foundOneRedirect)
            echorite(arguments);
        else if (strcmp(arguments[0], "echo") == 0)
        {
            echo(arguments);
        }
        else if (strcmp(input, "cd") == 0)
        {
            cd(arguments);
            // getLocation();
        }
        else if (strcmp(input, "cp") == 0)
            cp(arguments);
        else if (strcmp(input, "delete") == 0)
            delete (arguments);
        else if (piping)
        {
            char **beforePipe = splitInput(input, &pipeIndex);
            char **afterPipe = splitAfterPipe(input, pipeIndex);

            mypipe(beforePipe, afterPipe);
            wait(NULL);
        }
        else if (strcmp(input, "mv") == 0)
            move(arguments);
        else if (strcmp(input, "read") == 0)
            readd(arguments);
        else if (strcmp(input, "wc") == 0)
            wordCount(arguments);
        else
        {
            systemCall(arguments);
            wait(NULL);
        }
        free(arguments);
        free(input);
    }
    return 1;
}

void welcome()
{
    printf(
        "                              GGGGGG   AAA   LL          KK  KK\n"
        "                             GG    GG AAAAA  LL          KK KK\n"
        "                             GG       AA  AA LL          KKK\n"
        "                             GG   GGG AAAAAA LL          KKK\n"
        "                             GG    GG AA  AA LL          KK KK\n"
        "                              GGGGGG  AA  AA LLLLLLL     KK  KK\n"
        "              |\n"
        "              | Welcome to myShell\n"
        "              | Version 2.4.3\n"
        "              |\n"
        "              | https://github.com/GalKatzav/MyShell.git\n"
        "              |\n"
        "              | Have fun!\n"
        "              |\n"
        "              | Craft packets like it is your last\n"
        "              | day on earth.\n"
        "              |                      -- Lao-Tze\n"
        "              |\n"
        "              |\n"
        "              |\n"
        "              |\n"
        "              |\n"
        "              |\n"
        "              |\n"
        "                                                                           using c\n");
}