#pragma once

#include <map/map.h>
#include <types.h>
#include <vec/vec.h>

typedef scm_var_t (*scm_fn_t)(scm_var_t);
typedef map_t(scm_fn_t) map_scm_fn_t;

scm_var_t scm_display(scm_var_t args);
scm_var_t scm_load(scm_var_t args);
scm_var_t scm_add(scm_var_t args);
scm_var_t scm_subs(scm_var_t args);
scm_var_t scm_mult(scm_var_t args);
scm_var_t scm_div(scm_var_t args);
scm_var_t scm_define(scm_var_t args);
scm_var_t scm_exit(scm_var_t args);
scm_var_t scm_error(scm_var_t error);

void scm_builtin_init(map_scm_fn_t *map);