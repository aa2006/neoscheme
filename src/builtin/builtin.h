#pragma once 

#include <vec/vec.h>
#include <map/map.h>

#include <types.h>

typedef scm_var_t (*fn)(vec_str_t);
typedef map_t(fn) map_fn_t;

scm_var_t scm_load(vec_str_t args);
scm_var_t scm_add(vec_str_t args);
scm_var_t scm_subs(vec_str_t args);

map_fn_t scm_builtin_init(void);