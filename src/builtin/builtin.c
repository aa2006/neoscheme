#include "builtin.h"

#include <assert.h>
#include <stdlib.h>

void scm_builtin_init(map_scm_fn_t *map)
{
    map_init(map);

    map_set(map, "+", scm_add);
    map_set(map, "-", scm_subs);
    map_set(map, "*", scm_mult);
    map_set(map, "/", scm_div);
    map_set(map, "display", scm_display);
    map_set(map, "load", scm_load);
    map_set(map, "define", scm_define);
    map_set(map, "exit", scm_exit);
}