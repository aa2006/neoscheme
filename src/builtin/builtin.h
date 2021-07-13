#pragma once 

#include <vec/vec.h>
#include <map/map.h>

#include <types.h>

typedef scm_var_t (*fn)(vec_scm_var_t);
typedef map_t(fn) map_fn_t;

scm_var_t scm_load(vec_scm_var_t args);
scm_var_t scm_add(vec_scm_var_t args);
scm_var_t scm_subs(vec_scm_var_t args);

map_fn_t scm_builtin_init(void);