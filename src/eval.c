#include "eval.h"

#include "builtin/builtin.h"
#include "types.h"

#include <assert.h>
#include <ctype.h>
#include <reader/reader.h>
#include <stdio.h>
#include <string.h>
#include <vec/vec.h>

static size_t line = 0;
static bool is_repl;
static scm_var_t read_list(reader_str_t *reader);
static map_scm_fn_t builtin;
static map_scm_var_t user_symbols;
static bool is_symbol_init = false;

static void token_smart_push(vec_str_t *this, vec_char_t *token)
{
    if (token->length > 0)
    {
        assert(vec_push(token, 0) == 0);
        assert(vec_reserve(this, this->capacity + 1) == 0);

        this->data[this->length] = strdup(token->data);
        this->length++;

        vec_clear(token);
    }
}

static vec_str_t tokenize(char const *stmt)
{
    vec_char_t buffer;
    vec_str_t tokens;

    bool is_str = false;

    vec_init(&tokens);
    vec_init(&buffer);

    for (size_t i = 0; i < strlen(stmt); i++)
    {
        switch (stmt[i])
        {
            case '\n':
            case '\r':
            case '\f':
            case '\v':
            {
                token_smart_push(&tokens, &buffer);
                break;
            }

            case '\t':
            case ' ':
            {
                if (!is_str)
                {
                    token_smart_push(&tokens, &buffer);
                }
                else
                {
                    assert(vec_push(&buffer, stmt[i]) == 0);
                }

                break;
            }

            case '#':
            {
                token_smart_push(&tokens, &buffer);

                if (stmt[i + 1] == 'f' || stmt[i + 1] == 't' ||
                    stmt[i + 1] == 'f')
                {
                    assert(vec_push(&buffer, '#') == 0);
                    assert(vec_push(&buffer, stmt[++i]) == 0);
                    token_smart_push(&tokens, &buffer);
                }
                else
                {
                    token_smart_push(&tokens, &buffer);
                    assert(vec_push(&buffer, stmt[i]) == 0);
                    token_smart_push(&tokens, &buffer);
                }

                break;
            }

            case '\'':
            case ')':
            case '(':
            {

                if (!is_str)
                {
                    token_smart_push(&tokens, &buffer);
                    assert(vec_push(&buffer, stmt[i]) == 0);
                    token_smart_push(&tokens, &buffer);
                }
                else
                {
                    assert(vec_push(&buffer, stmt[i]) == 0);
                }

                break;
            }

            case '\"':
            {
                assert(vec_push(&buffer, stmt[i]) == 0);
                is_str = !is_str;
                break;
            }

            case ';':
            {
                while (stmt[i] != '\0' && stmt[i] != '\n' && stmt[i] != EOF)
                {
                    continue;
                }

                break;
            }

            default:
            {
                assert(vec_push(&buffer, stmt[i]) == 0);
                break;
            }
        }
    }

    token_smart_push(&tokens, &buffer);
    vec_deinit(&buffer);
    return tokens;
}

bool isnumber(char const *s)
{
    bool ret = true;

    for (size_t i = (s[0] == '-' ? 1 : 0); i < strlen(s); i++)
    {
        if (isdigit(s[i]) == 0)
        {
            ret = false;
        }
    }

    return (s[0] == '-' && strlen(s) == 1) ? false : ret;
}

bool isfloat(char const *s)
{
    bool has_digit = false;
    bool found_dot = false;
    bool ret = true;

    for (size_t i = (s[0] == '-' ? 1 : 0); i < strlen(s); i++)
    {
        if (found_dot && s[i] == '.' && !isdigit(s[i]))
        {
            ret = false;
        }

        if (isdigit(s[i]))
        {
            has_digit = true;
        }

        if (s[i] == '.')
        {
            found_dot = true;
        }
    }

    return found_dot && has_digit && ret;
}

static bool isstr(char const *s)
{
    if (s[0] != '\"')
    {
        return 0;
    }

    size_t i;

    for (i = 1; i < strlen(s); i++)
    {
        if (s[i] == '\\')
        {
            i++;
        }

        if (s[i] == '\"' && s[i + 1] != 0)
        {
            return false;
        }
    }

    if (s[i - 1] != '\"')
    {
        return false;
    }

    return true;
}

static scm_var_t read_atom(reader_str_t *reader)
{
    char *raw_token = reader_peek(reader);
    scm_var_t token;

    if (isnumber(raw_token))
    {
        token.type = SCM_NUMBER_INT;
        token._nbr = atoll(raw_token);
    }
    else if (isfloat(raw_token))
    {
        token.type = SCM_NUMBER_FLOAT;
        token._float = strtod(raw_token, NULL);
    }
    else if (isstr(raw_token))
    {
        token.type = SCM_STR;

        raw_token[strlen(raw_token) - 1] = '\0';
        raw_token++;

        token._str = raw_token;
    }
    else if (strcmp(raw_token, "#f") == 0)
    {
        token.type = SCM_BOOL;
        token._bool = false;
    }
    else if (strcmp(raw_token, "#t") == 0)
    {
        token.type = SCM_BOOL;
        token._bool = true;
    }
    else if (strcmp(raw_token, "nil") == 0)
    {
        token.type = SCM_NIL;
        token._bool = false;
    }
    else
    {
        token.type = SCM_SYMBOLS;
        token._str = raw_token;
    }

    (void) reader_next(reader);
    return token;
}

static scm_var_t read_form(reader_str_t *reader)
{
    char *token = reader_peek(reader);

    if (strcmp(token, "(") == 0)
    {
        return read_list(reader);
    }
    else if (strcmp(token, ")") == 0)
    {
        fprintf(stderr, "SyntaxError: Unexpected ) at line %ld\n", line);

        if (!is_repl)
        {
            exit(1);
        }
        else
        {
            return scm_token_nil;
        }
    }
    else if (strcmp(token, "\n") == 0)
    {
        line++;
    }
    else
    {
        return read_atom(reader);
    }

    return scm_token_nil;
}

static scm_var_t read_list(reader_str_t *reader)
{
    scm_var_t tokens;
    tokens.type = SCM_TOKENS;
    vec_init(&tokens._toks);

    (void) reader_next(reader);

    while (strcmp(reader_peek(reader), ")"))
    {
        if (reader_eof(reader))
        {
            fprintf(stderr, "SyntaxError: Expected )\n");

            if (is_repl)
            {
                return scm_token_nil;
            }
            else
            {
                exit(1);
            }
        }

        char *token = reader_peek(reader);

        if (strcmp(token, "\n"))
        {
            line++;
        }

        assert(vec_push(&tokens._toks, read_form(reader)) == 0);
        token = reader_peek(reader);
    }

    (void) reader_next(reader);
    return tokens;
}

scm_var_t scm_run(scm_var_t tokens)
{
    scm_fn_t *call = NULL;
    scm_var_t lst;

    if (tokens.type == SCM_TOKENS)
    {
        vec_init(&lst._toks);
        scm_var_t tok;
        int i;
        vec_foreach(&tokens._toks, tok, i)
        {
            if (tok.type == SCM_TOKENS)
            {
                assert(vec_push(&lst._toks, scm_run(tok)) == 0);
            }
            else if (tok.type == SCM_SYMBOLS)
            {
                scm_fn_t *function = map_get(&builtin, tok._str);
                scm_var_t *symbol = map_get(&user_symbols, tok._str);

                if (symbol != NULL)
                {
                    assert(vec_push(&lst._toks, *symbol) == 0);
                }
                else if (call != NULL && *call == scm_define)
                {
                    assert(vec_push(&lst._toks, tok) == 0);
                }
                else if (function == NULL)
                {
                    fprintf(stderr, "NameError: %s is undefined\n", tok._str);
                    return scm_token_nil;
                }
                else if (call == NULL)
                {
                    call = function;
                }
            }
            else
            {
                if (call == NULL && tokens._toks.length > 1)
                {
                    fprintf(stderr, "Invalid application\n");
                    return scm_token_nil;
                }
                else
                {
                    assert(vec_push(&lst._toks, tok) == 0);
                }
            }
        }

        if (call != NULL)
        {
            return (*call)(lst);
        }
    }
    else if (tokens.type != SCM_TOKENS)
    {
        return tokens;
    }

    return lst;
}

int scm_eval(char const *stmt, bool repl)
{
    vec_str_t tokens = tokenize(stmt);
    reader_str_t reader;

    scm_builtin_init(&builtin);

    is_repl = repl;

    reader_init(&reader, tokens.data, tokens.length);

    if (!is_symbol_init)
    {
        map_init(&user_symbols);
        is_symbol_init = true;
    }

    if (tokens.length == 0)
    {
        return 0;
    }

    if (is_repl)
    {
        while (!reader_eof(&reader))
        {
            scm_print_var(scm_run(read_form(&reader)));
        }
    }
    else
    {
        while (!reader_eof(&reader))
        {
            scm_run(read_form(&reader));
        }
    }

    vec_deinit(&tokens);
    return 0;
}

void scm_c_define(char *key, scm_var_t value)
{
    scm_var_t last_var = {
        .type = SCM_STR,
        ._str = key,
    };

    map_set(&user_symbols, key, value);
    map_set(&user_symbols, "__scm_last_var", last_var);
    map_set(&user_symbols, "__scm_last_value", value);
}
