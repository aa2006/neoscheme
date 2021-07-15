#pragma once

#include <stdbool.h>
#include <types.h>

int scm_eval(char const *stmt, bool repl);
void scm_c_define(char *key, scm_var_t value);