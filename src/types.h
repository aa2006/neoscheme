#pragma once 

#include <stdbool.h>
#include <stddef.h>
#include <vec/vec.h>

enum scm_types
{
    SCM_TOKENS,
    SCM_SYMBOLS,
    SCM_BOOL,
    SCM_NUMBER_INT,
    SCM_NUMBER_FLOAT,
    SCM_CHAR,
    SCM_STR,
    SCM_NIL
};

typedef struct scm_var
{
    enum scm_types type;

    union 
    {
        bool _bool;
        ssize_t _nbr;
        char _char;
        char *_str;
        double _float;
        vec_t(struct scm_var) _toks;
    };
} scm_var_t;

#define scm_token_nil ((scm_var_t) {.type = SCM_NIL, ._nbr = 0})

#define scm_token(value)_Generic(\
    (value),\
        int: (scm_var_t) {.type = SCM_NUMBER_INT, ._nbr = (ssize_t) (value)},\
        ssize_t: (scm_var_t) {.type = SCM_NUMBER_INT, ._nbr = (ssize_t) (value)},\
        double: (scm_var_t) {.type = SCM_NUMBER_FLOAT, ._float = (double) (value)})

void scm_print_var(scm_var_t var);
typedef vec_t(scm_var_t) vec_scm_var_t;