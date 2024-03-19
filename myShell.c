#include "myShell.h"
#include "myFunction.h"

int main()
{

    welcome();
    while (1)
    {
        int piping = 1;
        getLocation();
        char *input = getInputFromUser();
        if (strcmp(input, "exit") == 0 || strncmp(input, "exit ", 5) == 0 || strncmp(input, " exit", 5) == 0 || strncmp(input, " exit ", 6) == 0)
            logout(input);
        char **arguments = splitArgument(input);
        if (strcmp(input, "echo") == 0)
            echo(arguments);
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

            // Define pointers for argv1 and argv2
            char **argv1, **argv2;

            // Call splitInput to split the input and get argv1 and argv2
            argv1 = splitInput(input, &argv2);

            // If both argv1 and argv2 are not NULL, call mypipe
            if (argv1 != NULL && argv2 != NULL)
            {
                mypipe(argv1, argv2);
                wait(NULL);
            }

            // Free allocated memory for argv1
            if (argv1 != NULL)
            {
                for (int i = 0; argv1[i] != NULL; i++)
                {
                    free(argv1[i]);
                }
                free(argv1);
            }

            // Free allocated memory for argv2
            if (argv2 != NULL)
            {
                for (int i = 0; argv2[i] != NULL; i++)
                {
                    free(argv2[i]);
                }
                free(argv2);
            }
        }
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