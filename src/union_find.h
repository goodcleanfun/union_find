#pragma once

typedef struct union_find_node {
    int height;
    int in_degree;
    void *item;
    struct union_find_node *parent;
    struct union_find_node *list;
} union_find_node_t;

#define MEMORY_POOL_NAME union_find_node_memory_pool
#define MEMORY_POOL_TYPE union_find_node_t
#include "memory_pool/memory_pool.h"
#undef MEMORY_POOL_NAME
#undef MEMORY_POOL_TYPE

typedef struct union_find {
    union_find_node_memory_pool *node_pool;
} union_find_t;


static union_find_t *union_find_new_pool(union_find_node_memory_pool *pool) {
    if (pool == NULL) return NULL;
    union_find_t *uf = calloc(1, sizeof(union_find_t));
    if (uf == NULL) return NULL;
    uf->node_pool = pool;
    return uf;
}

static union_find_t *union_find_new(void) {
    union_find_node_memory_pool *pool = union_find_node_memory_pool_new();
    if (pool == NULL) return NULL;
    union_find_t *uf = union_find_new_pool(pool);
    if (uf == NULL) {
        union_find_node_memory_pool_destroy(pool);
        return NULL;
    }
    return uf;
}

static union_find_node_t *union_find_insert(union_find_t *uf, void *item) {
    if (uf == NULL || uf->node_pool == NULL) return NULL;
    union_find_node_t *node = union_find_node_memory_pool_get(uf->node_pool);
    node->item = item;
    node->height = 0;
    node->in_degree = 0;
    node->parent = NULL;
    node->list = NULL;
    return node;
}

static union_find_node_t *union_find_node_root(union_find_node_t *node) {
    if (node == NULL) return NULL;
    union_find_node_t *root;
    for (root = node; root->parent != NULL; root = root->parent);
    return root;
}

static bool union_find_same_class(union_find_node_t *node1, union_find_node_t *node2) {
    if (node1 == NULL || node2 == NULL) return false;
    union_find_node_t *root1 = union_find_node_root(node1);
    union_find_node_t *root2 = union_find_node_root(node2);

    return root1 == root2;
}

static void union_find_join(union_find_node_t *node1, union_find_node_t *node2) {
    if (node1 == NULL || node2 == NULL) return;

    union_find_node_t *root1 = union_find_node_root(node1);
    union_find_node_t *root2 = union_find_node_root(node2);

    union_find_node_t *tmp;
    // Make sure root1 is the taller of the two
    if (root1->height < root2->height) {
        tmp = root1;
        root1 = root2;
        root2 = tmp;
    }

    if (root1->height >= 2) {
        /*
        Assuming heights of at least 2 for both roots,

              r                   s
           / /  \ \      +      / | \
          a b    c d           x  y  z
          ^ ^    ^ ^           ^  ^  ^
        */

        if (root2->height < root1->height) {
            /*
            Inserting two levels below root 1. Point all lower neighbors of root2,
            and root2 itself, to root1's list (lower neighbor).

                  r -------
              /  / \ \    |
             a<-b<--c<-d<--
                      //\\
                    x y z s
            */

            tmp = root2->list;
            // go through list below root2
            while (tmp != NULL) {
                // point to node on root1 list
                tmp->parent = root1->list;
                tmp = tmp->list;
            }
            // also point root2 at the list node
            root2->parent = root1->list;
            // root2->height == root1->height
        } else {
            /*
            Join lower neighbor lists and point all lower neighbors of root2 to root1

                      r ----------
             /  /  / / \ \ \     |
            a<-b<-c<-d<-x<-y<-z<--

            Note in this implementation we only need to maintain the list and in_degree
            for the root node of a tree, so we can just leave that metadata
            */

            tmp = root2->list;
            tmp->parent = root1;
            while (tmp->list != NULL) {
                tmp = tmp->list;
                // move to end of root 2 list
                tmp->parent = root1;
            }
            // join linked lists (tmp is now the end of root2's list)
            tmp->list = root1->list;
            root1->list = root2->list;
            // now lists joined together below root 1, increase its in_degree
            root1->in_degree += root2->in_degree;
            if (root1->in_degree <= root1->height) {
                /*
                point root2 to node on root1 list
                         r -----------
                /  /  / /  \  \  \   |
                a<-b<-c<-d<-x<-y<-z<--
                                   \
                                    s
                */
                root2->parent = root1->list;
            } else {
                /*
                root2 becomes the new root, root 1 goes below
                         s----
                             |
                         r<---
                /  /  / /  \  \  \
                a<-b<-c<-d<-x<-y<-z
                */
                root1->parent = root2;
                root1->list = NULL;
                root2->height++;
                root2->in_degree = 1;
                root2->list = root1;
            }
        }
    // root1->height <= 1
    } else {
        if (root1->height == 0) {
            root1->height = 1;
            root1->in_degree = 1;
            root1->list = root2;
            // root1 is the new root
            root2->parent = root1;
        } else { // root1->height == 1
            // any root at height 1 has exactly one lower neighbor
            if (root2->height == 1) {
                // both height 1
                root2->list->parent = root1;
            }
            // make root1 lower neighbor of root2
            root2->height = 2;
            root2->in_degree = 1;
            root2->list = root1;
            root1->list = NULL;
            // root2 is the new root
            root1->parent = root2;
        }
    }
}

static void union_find_destroy(union_find_t *uf) {
    if (uf == NULL) return;
    if (uf->node_pool != NULL) {
        union_find_node_memory_pool_destroy(uf->node_pool);
    }
    free(uf);
}