#include <stdio.h>
#include <types.h>
#include "eval.h"

static char *unescape(char const *str)
{
    char *ret = calloc(strlen(str), sizeof(char));

    for (size_t i = 0; i < strlen(str); i++)
    {
        if (str[i] == '\\')
        {
            switch(str[++i])
            {
                case 'n':
                {
                    ret[strlen(ret)] = '\n';
                    break;
                }
                case 't':
                {
                    ret[strlen(ret)] = '\t';
                    break;
                }
                default:
                {
                    ret[strlen(ret)] = str[i];
                }
            }
        } 
        else 
        {
            ret[strlen(ret)] = str[i];
        }
    }

    return ret;
}

scm_var_t scm_display(scm_var_t args)
{
    scm_var_t ret = scm_token_nil;

    scm_var_t arg;
    int i;

    vec_foreach(&args._toks, arg, i)
    {
        switch (arg.type)
        {
            case SCM_NUMBER_INT:
            {
                printf("%ld", arg._nbr);
                break;
            }

            case SCM_NUMBER_FLOAT:
            {
                printf("%f", arg._float);
                break;
            }

            case SCM_STR:
            {
                char *s = unescape(arg._str);
                printf("%s",s);

                free(s);
                break;
            }

            case SCM_BOOL:
            {
                printf(arg._bool ? "#t" : "#f");
                break;
            }

            default:
            {
                printf("Non-printable");
                break;
            }
        }
    }

    return ret;
}