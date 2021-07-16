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
    map_set(map, "error", scm_error);
    // map_set(map, "assert-eq?", scm_assert_eq);
    map_set(map, "assert-true?", scm_assert_true);
    map_set(map, "eq?", scm_equal);
}