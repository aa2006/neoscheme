#include <assert.h>
#include "builtin/builtin.h"
#include <ctype.h>
#include "eval.h"
#include <stdio.h>
#include <string.h>
#include "types.h"

#include <vec/vec.h>
#include <reader/reader.h>

static size_t line = 0;
static bool is_repl;
static scm_var_t read_list(reader_str_t *reader);

static void token_smart_push(vec_str_t *this, vec_char_t *token)
{
    if (token->length > 0)
    {
        assert(vec_push(token, 0) == 0);
        assert(vec_reserve(this, this->capacity + 1) == 0);

        this->data[this->length] = (char *) malloc(token->length);
        assert(this->data[this->length] != NULL);

        strcpy(this->data[this->length], token->data);
        this->length++;

        vec_clear(token);
    }
}

static vec_str_t tokenize(char const *stmt)
{
    vec_char_t buffer;
    vec_str_t tokens;

    vec_init(&tokens);
    vec_init(&buffer);

    for (size_t i = 0; i < strlen(stmt); i++)
    {
        switch(stmt[i])
        {
            case '\r':
			case '\t':
			case '\f':
			case '\v':
			case ' ':
            {
                token_smart_push(&tokens, &buffer);
                break;
            }

            case '#':
            {
                token_smart_push(&tokens, &buffer);

                if (stmt[i+1] == 'f' || stmt[i+1] == 't' || stmt[i+1] == 'f')
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

			case '\n': 
            case '\'':
            case ')':
            case '(':
            {

                token_smart_push(&tokens, &buffer);
                assert(vec_push(&buffer, stmt[i]) == 0);
                token_smart_push(&tokens, &buffer);
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

    return ret;
}

bool isfloat(char const *s)
{
    bool found_dot = false;
    bool ret = true;
    
    for (size_t i = (s[0] == '-' ? 1 : 0); i < strlen(s); i++)
    {
        if (found_dot && s[i] == '.' && !isdigit(s[i]))
        {
            ret = false;
        }

        if (s[i] == '.')
        {
            found_dot = true;
        }
    }

    return ret && found_dot;
}

static bool isstr(char const *s)
{
    if (s[0] != '\"')
    {
        return 0;
    }

    size_t i;

    for (i = 1; i <strlen(s); i++)
    {
        if (s[i] == '\\')
        {
            i++;
        }

        if (s[i] == '\"' && s[i+1] != 0)
        {
            return false;
        }
    }

    if (s[i-1] != '\"')
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

        raw_token[strlen(raw_token)-1] = '\0';
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
            return scm_token(SCM_NIL, NULL);
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

    return scm_token(SCM_NIL, NULL);
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
                return scm_token(SCM_NIL, NULL);
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

static void scm_run(scm_var_t tokens)
{
    if (tokens.type != SCM_TOKENS)
    {
        if (is_repl)
        {
            scm_print_var(tokens);
        }
        else 
        {
            return;
        }
    }
}

int scm_eval(char const *stmt, bool repl)
{
    vec_str_t tokens = tokenize(stmt);
    reader_str_t reader;

    is_repl = repl;

    reader_init(&reader, tokens.data, tokens.length);

    if (tokens.length == 0)
    {
        return 0;
    }

    scm_run(read_form(&reader));

    vec_deinit(&tokens);
    return 0;
}