#pragma once

#include <map/map.h>
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
    SCM_NIL,
    SCM_ERROR
};

static char *const scm_typename[] = {"token",  "symbol", "boolean",
                                     "number", "number", "char",
                                     "string", "nil",    "error"};

enum scm_error_type
{
    SCM_ARGUMENT_ERROR,
    SCM_TYPE_ERROR,
    SCM_SCHEME_ERROR,
    SCM_FILE_NOT_FOUND,
    SCM_ASSERT_ERROR,
};

typedef struct
{
    enum scm_error_type type;
    char *msg;
} scm_error_t;

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
        scm_error_t _error;
        vec_t(struct scm_var) _toks;
    };
} scm_var_t;

#define scm_token_nil ((scm_var_t){.type = SCM_NIL, ._nbr = 0})
#define scm_token_error(t, m)                                                  \
    ((scm_var_t){.type = SCM_ERROR,                                            \
                 ._error = (scm_error_t){.type = (t), .msg = (m)}})

#define scm_token(value)                                                       \
    _Generic(                                                                  \
        (value), bool                                                          \
        : (scm_var_t){.type = SCM_BOOL, ._bool = (value)}, int                 \
        : (scm_var_t){.type = SCM_NUMBER_INT, ._nbr = (ssize_t) (value)},      \
          ssize_t                                                              \
        : (scm_var_t){.type = SCM_NUMBER_INT, ._nbr = (ssize_t) (value)},      \
          double                                                               \
        : (scm_var_t){.type = SCM_NUMBER_FLOAT, ._float = (double) (value)})

void scm_print_var(scm_var_t var);
char *scm_error_type_str(enum scm_error_type type);

typedef vec_t(scm_var_t) vec_scm_var_t;
typedef map_t(scm_var_t) map_scm_var_t;