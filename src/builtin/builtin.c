#include "builtin.h"

map_fn_t scm_builtin_init(void)
{
    map_fn_t map;
    map_init(&map);

    map_set(&map, "load", scm_load);
    map_set(&map, "+", scm_add);
    map_set(&map, "-", scm_subs);

    return map;
}