#include "myShell.h"
#include "myFunction.h"

int main()
{

    welcome();
    while (1)
    {
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
        free(arguments);
        free(input);
    }
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