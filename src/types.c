#include <stdio.h>
#include <types.h>

#define auto __auto_type

void scm_print_var(scm_var_t var, bool is_repl)
{
    if (is_repl)
    {
        printf("=> ");
    }
    switch (var.type)
    {
        case SCM_BOOL:
        {
            if (var._bool)
            {
                printf("#t");
            }
            else
            {
                printf("#f");
            }

            break;
        }

        case SCM_NIL:
        {
            printf("nil");
            break;
        }

        case SCM_NUMBER_FLOAT:
        {
            printf("%lf", var._float);
            break;
        }

        case SCM_NUMBER_INT:
        {
            printf("%ld", var._nbr);
            break;
        }

        case SCM_CHAR:
        {
            printf("\'%c\'", var._char);
            break;
        }

        case SCM_STR:
        {
            printf("\"%s\"", var._str);
            break;
        }

        default:
        {
            break;
        }
    }

    if (is_repl)
    {
        printf("\n");
    }
}

char *scm_error_type_str(enum scm_error_type type)
{
    char *ret;
    switch (type)
    {
        case SCM_ARGUMENT_ERROR:
        {
            ret = "ArgumentError";
            break;
        }
        case SCM_TYPE_ERROR:
        {
            ret = "TypeError";
            break;
        }
        case SCM_SCHEME_ERROR:
        {
            ret = "SchemeError";
            break;
        }
        case SCM_FILE_NOT_FOUND:
        {
            ret = "FileNotFound";
            break;
        }
        case SCM_ASSERT_ERROR:
        {
            ret = "AssertionFailed";
            break;
        }
    }

    return ret;
}