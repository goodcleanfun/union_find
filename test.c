#include <stdint.h>
#include "greatest/greatest.h"

#include "union_find.h"

TEST test_union_find(void) {
    union_find_t *uf = union_find_new();

    union_find_node_t *a = union_find_insert(uf, "a");
    union_find_node_t *b = union_find_insert(uf, "b");
    union_find_node_t *c = union_find_insert(uf, "c");
    union_find_node_t *d = union_find_insert(uf, "d");
    union_find_node_t *e = union_find_insert(uf, "e");
    union_find_node_t *f = union_find_insert(uf, "f");
    union_find_node_t *g = union_find_insert(uf, "g");
    union_find_node_t *h = union_find_insert(uf, "h");
    union_find_node_t *i = union_find_insert(uf, "i");
    union_find_node_t *j = union_find_insert(uf, "j");
    union_find_node_t *k = union_find_insert(uf, "k");
    union_find_node_t *l = union_find_insert(uf, "l");
    union_find_node_t *m = union_find_insert(uf, "m");
    union_find_node_t *n = union_find_insert(uf, "n");
    union_find_node_t *o = union_find_insert(uf, "o");
    union_find_node_t *r = union_find_insert(uf, "r");
    union_find_node_t *s = union_find_insert(uf, "s");
    union_find_node_t *t = union_find_insert(uf, "t");
    union_find_node_t *u = union_find_insert(uf, "u");

    union_find_node_t *nodes[] = {a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, r, s, t, u};

    union_find_join(r, a);
    union_find_join(b, c);
    union_find_join(c, a);

    union_find_join(d, e);
    union_find_join(f, g);
    union_find_join(g, d);

    union_find_join(h, i);
    union_find_join(j, k);
    union_find_join(k, h);

    union_find_join(l, m);
    union_find_join(n, o);
    union_find_join(o, l);

    union_find_join(r, d);
    union_find_join(h, l);
    union_find_join(l, r);
    union_find_join(r, s);
    union_find_join(r, t);
    union_find_join(r, u);
    union_find_join(u, t);

    for (size_t i = 0; i < sizeof(nodes) / sizeof(nodes[0]); i++) {
        for (size_t j = 0; j < sizeof(nodes) / sizeof(nodes[0]); j++) {
            ASSERT(union_find_same_class(nodes[i], nodes[j]));
        }
    }

    union_find_destroy(uf);
    PASS();
}

/* Add definitions that need to be in the test runner's main file. */
GREATEST_MAIN_DEFS();

int32_t main(int32_t argc, char **argv) {
    GREATEST_MAIN_BEGIN();      /* command-line options, initialization. */

    RUN_TEST(test_union_find);

    GREATEST_MAIN_END();        /* display results */
}
