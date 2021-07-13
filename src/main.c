#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/history.h>
#include <readline/readline.h>

#define LOAD_STMT_LENGTH (10)

void repl(void)
{
    while (1)
    {
        char *input = readline("\033[1m\033[31m>\033[33m>\033[32m>\033[0m ");

        if (input == NULL)
        {
            clear_history();
            free(input);

            return;
        }
        else if(*input)
        {
            add_history(input);
            scm_eval(input, true);
        }

        free(input);
    }
}

int main(int argc, char const **argv)
{
    if (argc == 1)
    {
        repl();
        return 0;
    }
    else 
    {
        char *stmt = malloc(strlen(argv[1]) + LOAD_STMT_LENGTH);
        sprintf(stmt, "(load \"%s\")", argv[1]);

        scm_eval(stmt, false);

        free(stmt);
    }

    return 0;
}