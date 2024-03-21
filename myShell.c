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
        "                     aGPY//YAGa\n"
        "              apyyyyCY//////////YCa       |\n"
        "             GY//////YGpcg  gcpCY//Pg     | Welcome to myShell\n"
        "  ayp ayyyyyyyCGP//Pg           gyY//C    | Version 2.4.3\n"
        "  AYAgAYYYYYYYY///Pg              cY//G   |\n"
        "          pCCCCY//p          cGGpg y//Y   | https://github.com/GalKatzav/MyShell.git\n"
        "          GPPPP///a          pP///AC//Y   |\n"
        "               A//A            cyP////C   | Have fun!\n"
        "               p///Ac            sC///a   |\n"
        "               P////YCpc           A//A   | Craft packets like it is your last\n"
        "        ggggggp///pGP///p          p//Y   | day on earth.\n"
        "       gY/////////y  caa           G//P   |                      -- Lao-Tze\n"
        "        cayCyayP//Ya              pY/Ya   |\n"
        "         gY/PgY////YGCg          aC//Yp\n"
        "          gc  gccgAY//PGgpaapyaCP//YGg\n"
        "                   gpGPY//////YGPgp\n"
        "                        ggaagg\n"
        "                                                                           using c\n");
}