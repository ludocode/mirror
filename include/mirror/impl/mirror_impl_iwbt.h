/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2023 Fraser Heavy Software
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef MIRROR_IMPL_IWBT_H
#define MIRROR_IMPL_IWBT_H

/* TODO move to mirror_impl_ghost.h */
#include "ghost/debug/ghost_assert.h"
#include "ghost/ghost_core.h"
#include "ghost/impl/ghost_impl_function.h"
#include "ghost/impl/ghost_impl_inline.h"
#include "ghost/language/ghost_discard.h"
#include "ghost/language/ghost_null.h"
#include "ghost/language/ghost_unreachable.h"
#include "ghost/type/bool/ghost_bool.h"
#include "ghost/type/size_t/ghost_size_t.h"
#include "ghost/format/ghost_format_z.h"

/*
 * This is an implementation of a type-erased intrusive weight-balanced
 * order-statistic binary tree. It was originally written for Ghost but it has
 * been pulled into Mirror temporarily while I sort out what to do with it.
 */

/*
 * The implementation of the tree is described in "Balancing weight-balanced
 * trees" by Yoichi Hirai, 2011:
 *
 *     https://yoichihirai.com/bst.pdf
 *
 * We use the original balance method (where weight == size + 1), not the
 * variant balance method. We use parameters (3,2) which are the only integer
 * parameters that correctly preserve balance constraints in the original
 * balance method according to the paper.
 */

/* TODO these constants need to be MIRROR_IMPL */

/* The tree is unbalanced at a if `a > b*delta` for any siblings a and b. */
#define MIRROR_IWBT_DELTA 3

/* We do a double rotation if `aa > ab * delta` for the children of any
 * unbalanced node a (where aa is the interior node, i.e. the opposite side of
 * a in its parent), otherwise we do a single rotation. */
#define MIRROR_IWBT_GAMMA 2

#ifndef MIRROR_IWBT_DEBUG
    #define MIRROR_IWBT_DEBUG 0
#endif

#if MIRROR_IWBT_DEBUG
    #include <stdio.h>
    #define mirror_iwbt_log(x) printf x
#else
    #define mirror_iwbt_log(x) /*nothing*/
#endif

/**
 * Metadata for a node in an intrusive weight-balanced tree.
 *
 * Add a field of this type to a struct that you want to store in an iwbt data
 * structure.
 */
typedef struct mirror_iwbt_node_t {
    struct mirror_iwbt_node_t* mirror_impl_v_parent;
    struct mirror_iwbt_node_t* mirror_impl_v_first_child;
    struct mirror_iwbt_node_t* mirror_impl_v_last_child;
    ghost_size_t mirror_impl_v_weight;
} mirror_iwbt_node_t;

/**
 * A type-erased weight-balanced tree.
 *
 * You can initialize the tree to empty by zeroing, e.g. with GHOST_ZERO_INIT
 * or ghost_bzero(). You can also initialize it by clearing it with
 * mirror_iwbt_clear().
 *
 * The tree does not need to be de-initialized (but your nodes might.)
 *
 * @see GHOST_ZERO_INIT
 * @see ghost_bzero()
 * @see mirror_iwbt_clear()
 */
typedef struct mirror_iwbt_t {
    mirror_iwbt_node_t* mirror_impl_v_root;
} mirror_iwbt_t;

/* TODO for now we still lean on Ghost's internals. */
GHOST_IMPL_FUNCTION_OPEN

/*
 * Returns the weight of the given node or 1 if null.
 */
ghost_impl_inline
ghost_size_t mirror_impl_iwbt_weight(mirror_iwbt_node_t* /*nullable*/ node) {
    return (node == ghost_null) ? 1 : node->mirror_impl_v_weight;
}

/**
 * Returns the number of elements in the subtree rooted at the given node, or 0
 * if the given node is null.
 */
ghost_impl_inline
ghost_size_t mirror_iwbt_node_count(mirror_iwbt_node_t* mirror_impl_v_node) {
    if (mirror_impl_v_node == ghost_null)
        return 0;
    ghost_assert(mirror_impl_v_node->mirror_impl_v_weight > 1, "");
    return mirror_impl_v_node->mirror_impl_v_weight - 1;
}

#if MIRROR_IWBT_DEBUG
ghost_impl_function
void mirror_impl_iwbt_sanity_check_node(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    ghost_size_t left_weight = mirror_impl_iwbt_weight(node->mirror_impl_v_first_child);
    ghost_size_t right_weight = mirror_impl_iwbt_weight(node->mirror_impl_v_last_child);

    /* Make sure the weight is correct and the tree is balanced */
    ghost_assert(node->mirror_impl_v_weight > 1, "");
    ghost_assert(node->mirror_impl_v_weight == left_weight + right_weight, "");
    ghost_assert(left_weight * MIRROR_IWBT_DELTA >= right_weight, "");
    ghost_assert(right_weight * MIRROR_IWBT_DELTA >= left_weight, "");

    /* Check left child */
    if (node->mirror_impl_v_first_child != ghost_null) {
        ghost_assert(node->mirror_impl_v_first_child->mirror_impl_v_parent == node, "");
        mirror_impl_iwbt_sanity_check_node(tree, node->mirror_impl_v_first_child);
    }

    /* Check right child */
    if (node->mirror_impl_v_last_child != ghost_null) {
        ghost_assert(node->mirror_impl_v_last_child->mirror_impl_v_parent == node, "");
        mirror_impl_iwbt_sanity_check_node(tree, node->mirror_impl_v_last_child);
    }
})
#endif

/*
 * Sanity check recurses over the entire tree, checking every node to make
 * sure its weight is correct, its nodes are consistent and it is properly
 * balanced.
 */
ghost_impl_inline
void mirror_impl_iwbt_sanity_check(mirror_iwbt_t* tree) {
    ghost_discard(tree);
    #if MIRROR_IWBT_DEBUG
    ghost_assert(tree != ghost_null, "");
    mirror_iwbt_node_t* node = tree->mirror_impl_v_root;
    if (node == ghost_null)
        return;
    ghost_assert(node->mirror_impl_v_parent == ghost_null, "");
    mirror_impl_iwbt_sanity_check_node(tree, node);
    /*mirror_iwbt_log(("sanity check PASS\n")); */
    #endif
}

/**
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_root(mirror_iwbt_t* mirror_impl_v_tree) {
    mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);
    return mirror_impl_v_tree->mirror_impl_v_root;
}

/**
 * Returns the first child of the given node, or null if it has no first child.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_node_first_child(mirror_iwbt_node_t* mirror_impl_v_node) {
    ghost_assert(mirror_impl_v_node != ghost_null, "");
    return mirror_impl_v_node->mirror_impl_v_first_child;
}

ghost_impl_inline
void mirror_impl_iwbt_print_node(mirror_iwbt_t* tree, mirror_iwbt_node_t* node, ghost_size_t depth) {
    /* print right before left, the tree is rotated 90 degrees counter-clockwise */
    ghost_size_t i;
    if (node->mirror_impl_v_last_child)
        mirror_impl_iwbt_print_node(tree, node->mirror_impl_v_last_child, depth + 1);
    for (i = 0; i < depth; ++i)
        printf("    ");
    printf("%p  weight:%" GHOST_PRIiZ "\n", ghost_static_cast(void*, node), node->mirror_impl_v_weight);
    if (node->mirror_impl_v_first_child)
        mirror_impl_iwbt_print_node(tree, node->mirror_impl_v_first_child, depth + 1);
}

ghost_impl_inline
void mirror_impl_iwbt_print(mirror_iwbt_t* tree) {
    if (tree->mirror_impl_v_root == ghost_null) {
        printf("tree is empty.\n");
    } else {
        printf("=====\n");
        mirror_impl_iwbt_print_node(tree, tree->mirror_impl_v_root, 0);
        printf("=====\n");
    }
}

/*
 * Decrement sizes from this node to the root of the tree.
 */
ghost_impl_inline
void mirror_impl_iwbt_decrement_to_root(mirror_iwbt_node_t* node) {
    while (node != ghost_null) {
        --node->mirror_impl_v_weight;
        ghost_assert(node->mirror_impl_v_weight > 1, "");
        node = node->mirror_impl_v_parent;
    }
}

/*
 * Increment sizes from this node to the root of the tree.
 */
ghost_impl_inline
void mirror_impl_iwbt_increment_to_root(mirror_iwbt_node_t* node) {
    while (node != ghost_null) {
        ++node->mirror_impl_v_weight;
        node = node->mirror_impl_v_parent;
    }
}

ghost_impl_inline
void mirror_impl_iwbt_recalculate_size(mirror_iwbt_node_t* node) {
    ghost_assert(node != ghost_null, "");
    node->mirror_impl_v_weight =
        mirror_impl_iwbt_weight(node->mirror_impl_v_first_child) +
        mirror_impl_iwbt_weight(node->mirror_impl_v_last_child);
    ghost_assert(node->mirror_impl_v_weight > 1, "");
}

/**
 * Returns the first node among the subtree rooted at this node.
 *
 * In other words, it follows first children recursively starting at the given
 * node until it finds a node with no first child.
 *
 * This cannot be called with null. It never returns null.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_node_first_in_subtree(mirror_iwbt_node_t* mirror_impl_v_node) {
    ghost_assert(mirror_impl_v_node != ghost_null, "");
    mirror_iwbt_log(("finding firstmost %p\n", ghost_static_cast(void*, mirror_impl_v_node)));
    for (;;) {
        mirror_iwbt_node_t* next = mirror_impl_v_node->mirror_impl_v_first_child;
        if (next == ghost_null)
            break;
        mirror_iwbt_log(("found first %p\n", ghost_static_cast(void*, next)));
        mirror_impl_v_node = next;
    }
    mirror_iwbt_log(("no more firsts, firstmost is %p\n", ghost_static_cast(void*, mirror_impl_v_node)));
    return mirror_impl_v_node;
}

/**
 * Returns the last node among the subtree rooted at this node.
 *
 * In other words, it follows last children recursively starting at the given
 * node until it finds a node with no last child.
 *
 * This cannot be called with null. It never returns null.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_node_last_in_subtree(mirror_iwbt_node_t* mirror_impl_v_node) {
    ghost_assert(mirror_impl_v_node != ghost_null, "");
    for (;;) {
        mirror_iwbt_node_t* next = mirror_impl_v_node->mirror_impl_v_last_child;
        if (next == ghost_null)
            break;
        mirror_impl_v_node = next;
    }
    return mirror_impl_v_node;
}

/*
 * Perform a left rotation on the given node.
 *
 * When run on node x, it performs the following transformation:
 *
 *           |                 |
 *           x                 y
 *          / \               / \
 *         1   y    ---->    x   3
 *            / \           / \
 *           2   3         1   2
 */
ghost_impl_function
void mirror_impl_iwbt_rotate_left(mirror_iwbt_t* tree, mirror_iwbt_node_t* x) GHOST_IMPL_DEF({
    mirror_iwbt_node_t* y = x->mirror_impl_v_last_child;
    mirror_iwbt_log(("rotating left %p\n", ghost_static_cast(void*, x)));
    ghost_assert(x != ghost_null, "");
    ghost_assert(y != ghost_null, "");

    /* swap child pointers */
    x->mirror_impl_v_last_child = y->mirror_impl_v_first_child;
    y->mirror_impl_v_first_child = x;
    if (x->mirror_impl_v_last_child != ghost_null)
        x->mirror_impl_v_last_child->mirror_impl_v_parent = x;

    /* fix parent references */
    if (x->mirror_impl_v_parent == ghost_null) {
        ghost_assert(tree->mirror_impl_v_root == x, "");
        tree->mirror_impl_v_root = y;
    } else {
        mirror_iwbt_node_t* parent = x->mirror_impl_v_parent;
        if (parent->mirror_impl_v_first_child == x) {
            parent->mirror_impl_v_first_child = y;
        } else {
            ghost_assert(parent->mirror_impl_v_last_child == x, "");
            parent->mirror_impl_v_last_child = y;
        }
    }
    y->mirror_impl_v_parent = x->mirror_impl_v_parent;
    x->mirror_impl_v_parent = y;

    /* fixup sizes. make sure we fix the original x first since it's now
     * the child of its original y child!
     * TODO there's probably a quicker way to do this */
    mirror_impl_iwbt_recalculate_size(x);
    mirror_impl_iwbt_recalculate_size(y);
})

/*
 * Perform a right rotation on the given node.
 *
 * When run on node x, it performs the following transformation:
 *
 *           |                 |
 *           x                 y
 *          / \               / \
 *         y   3   ---->     1   x
 *        / \                   / \
 *       1   2                 2   3
 */
ghost_impl_function
void mirror_impl_iwbt_rotate_right(mirror_iwbt_t* tree, mirror_iwbt_node_t* x) GHOST_IMPL_DEF({
    mirror_iwbt_node_t* y = x->mirror_impl_v_first_child;
    mirror_iwbt_log(("rotating right %p\n", ghost_static_cast(void*, x)));
    ghost_assert(x != ghost_null, "");
    ghost_assert(y != ghost_null, "");

    /* swap child pointers */
    x->mirror_impl_v_first_child = y->mirror_impl_v_last_child;
    y->mirror_impl_v_last_child = x;
    if (x->mirror_impl_v_first_child != ghost_null)
        x->mirror_impl_v_first_child->mirror_impl_v_parent = x;

    /* fix parent reference */
    if (x->mirror_impl_v_parent == ghost_null) {
        ghost_assert(tree->mirror_impl_v_root == x, "");
        tree->mirror_impl_v_root = y;
    } else {
        mirror_iwbt_node_t* parent = x->mirror_impl_v_parent;
        if (parent->mirror_impl_v_first_child == x) {
            parent->mirror_impl_v_first_child = y;
        } else {
            ghost_assert(parent->mirror_impl_v_last_child == x, "");
            parent->mirror_impl_v_last_child = y;
        }
    }
    y->mirror_impl_v_parent = x->mirror_impl_v_parent;
    x->mirror_impl_v_parent = y;

    /* fixup sizes. make sure we fix x first since it's now the child of y! */
    mirror_impl_iwbt_recalculate_size(x);
    mirror_impl_iwbt_recalculate_size(y);
})

/*
 * Rebalance the given node.
 */
ghost_impl_function
void mirror_impl_iwbt_rebalance(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    mirror_iwbt_log(("pre-rebalance tree:\n"));
    /*mirror_impl_iwbt_print(tree); */
    mirror_iwbt_log(("rebalancing starting at %p\n", ghost_static_cast(void*, node)));
    while (node != ghost_null) {
        mirror_iwbt_node_t* left = node->mirror_impl_v_first_child;
        mirror_iwbt_node_t* right = node->mirror_impl_v_last_child;

        ghost_size_t left_weight = mirror_impl_iwbt_weight(left);
        ghost_size_t right_weight = mirror_impl_iwbt_weight(right);
        ghost_assert(node->mirror_impl_v_weight == left_weight + right_weight, "");

        if (left_weight * MIRROR_IWBT_DELTA < right_weight) {
            ghost_assert(right != ghost_null, ""); /* right_weight is non-zero so it can't be null */
            {
                ghost_size_t right_left_weight = mirror_impl_iwbt_weight(right->mirror_impl_v_first_child);
                ghost_size_t right_right_weight = mirror_impl_iwbt_weight(right->mirror_impl_v_last_child);
                ghost_assert(right->mirror_impl_v_weight == right_left_weight + right_right_weight, "");

                if (right_left_weight >= MIRROR_IWBT_GAMMA * right_right_weight) {
                    /* double rotation */
                    mirror_iwbt_log(("DOUBLE right then left\n"));
                    mirror_impl_iwbt_rotate_right(tree, right);
                }
                mirror_impl_iwbt_rotate_left(tree, node);
            }
            node = node->mirror_impl_v_parent->mirror_impl_v_parent;

        } else if (right_weight * MIRROR_IWBT_DELTA < left_weight) {
            ghost_assert(left != ghost_null, ""); /* left_weight is non-zero so it can't be null */
            {
                ghost_size_t left_left_weight = mirror_impl_iwbt_weight(left->mirror_impl_v_first_child);
                ghost_size_t left_right_weight = mirror_impl_iwbt_weight(left->mirror_impl_v_last_child);
                ghost_assert(left->mirror_impl_v_weight == left_left_weight + left_right_weight, "");

                if (left_right_weight >= MIRROR_IWBT_GAMMA * left_left_weight) {
                    /* double rotation */
                    mirror_iwbt_log(("DOUBLE left then right\n"));
                    mirror_impl_iwbt_rotate_left(tree, left);
                }
                mirror_impl_iwbt_rotate_right(tree, node);
            }

            node = node->mirror_impl_v_parent->mirror_impl_v_parent;

        } else {
            node = node->mirror_impl_v_parent;
        }
    }

    mirror_iwbt_log(("post-rebalance tree:\n"));
    /*mirror_impl_iwbt_print(tree); */
    mirror_impl_iwbt_sanity_check(tree);
})

/**
 * Replace an existing node with a new one.
 *
 * The new_node must not already be in the tree.
 */
ghost_impl_function
void mirror_iwbt_replace(mirror_iwbt_t* mirror_impl_v_tree,
        mirror_iwbt_node_t* mirror_impl_v_old_node,
        mirror_iwbt_node_t* mirror_impl_v_new_node) GHOST_IMPL_DEF(
{
    mirror_iwbt_node_t* parent;

    mirror_iwbt_log(("replacing %p with %p\n",
            ghost_static_cast(void*, mirror_impl_v_old_node),
            ghost_static_cast(void*, mirror_impl_v_new_node)));
    mirror_impl_v_new_node->mirror_impl_v_parent = mirror_impl_v_old_node->mirror_impl_v_parent;
    mirror_impl_v_new_node->mirror_impl_v_first_child = mirror_impl_v_old_node->mirror_impl_v_first_child;
    mirror_impl_v_new_node->mirror_impl_v_last_child = mirror_impl_v_old_node->mirror_impl_v_last_child;
    mirror_impl_v_new_node->mirror_impl_v_weight = mirror_impl_v_old_node->mirror_impl_v_weight;

    parent = mirror_impl_v_new_node->mirror_impl_v_parent;
    if (parent == ghost_null) {
        ghost_assert(mirror_impl_v_tree->mirror_impl_v_root == mirror_impl_v_old_node, "");
        mirror_impl_v_tree->mirror_impl_v_root = mirror_impl_v_new_node;
    } else if (parent->mirror_impl_v_first_child == mirror_impl_v_old_node) {
        parent->mirror_impl_v_first_child = mirror_impl_v_new_node;
    } else {
        ghost_assert(parent->mirror_impl_v_last_child == mirror_impl_v_old_node, "");
        parent->mirror_impl_v_last_child = mirror_impl_v_new_node;
    }

    if (mirror_impl_v_new_node->mirror_impl_v_first_child != ghost_null)
        mirror_impl_v_new_node->mirror_impl_v_first_child->mirror_impl_v_parent = mirror_impl_v_new_node;
    if (mirror_impl_v_new_node->mirror_impl_v_last_child != ghost_null)
        mirror_impl_v_new_node->mirror_impl_v_last_child->mirror_impl_v_parent = mirror_impl_v_new_node;
})

/*
 * A helper function to insert a node as the first child of a node that doesn't
 * have one.
 */
ghost_impl_function
void mirror_impl_iwbt_insert_as_first_child(mirror_iwbt_t* tree,
        mirror_iwbt_node_t* parent,
        mirror_iwbt_node_t* new_node) GHOST_IMPL_DEF(
{
    mirror_iwbt_log(("inserting %p as first child of %p\n",
            ghost_static_cast(void*, new_node),
            ghost_static_cast(void*, parent)));

    ghost_assert(parent != ghost_null, "");
    ghost_assert(parent->mirror_impl_v_first_child == ghost_null, "");
    parent->mirror_impl_v_first_child = new_node;

    new_node->mirror_impl_v_parent = parent;
    new_node->mirror_impl_v_first_child = ghost_null;
    new_node->mirror_impl_v_last_child = ghost_null;
    new_node->mirror_impl_v_weight = 2;

    mirror_impl_iwbt_increment_to_root(parent);
            mirror_iwbt_log(("%i\n",__LINE__));
    mirror_impl_iwbt_rebalance(tree, parent);
})

/*
 * A helper function to insert a node as the last child of a node that doesn't
 * have one.
 */
ghost_impl_function
void mirror_impl_iwbt_insert_as_last_child(mirror_iwbt_t* tree,
        mirror_iwbt_node_t* parent,
        mirror_iwbt_node_t* new_node) GHOST_IMPL_DEF(
{
    mirror_iwbt_log(("inserting %p as last child of %p\n",
            ghost_static_cast(void*, new_node),
            ghost_static_cast(void*, parent)));

    ghost_assert(parent != ghost_null, "");
    ghost_assert(parent->mirror_impl_v_last_child == ghost_null, "");
    parent->mirror_impl_v_last_child = new_node;

    new_node->mirror_impl_v_parent = parent;
    new_node->mirror_impl_v_first_child = ghost_null;
    new_node->mirror_impl_v_last_child = ghost_null;
    new_node->mirror_impl_v_weight = 2;

    mirror_impl_iwbt_increment_to_root(parent);
            mirror_iwbt_log(("%i\n",__LINE__));
    mirror_impl_iwbt_rebalance(tree, parent);
})

/*
 * A helper function to insert a node into an empty tree.
 */
ghost_impl_function
void mirror_impl_iwbt_insert_into_empty_tree(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    mirror_iwbt_log(("inserting %p into empty tree\n", ghost_static_cast(void*, node)));
    ghost_assert(tree->mirror_impl_v_root == ghost_null, "");
    tree->mirror_impl_v_root = node;
    node->mirror_impl_v_parent = ghost_null;
    node->mirror_impl_v_first_child = ghost_null;
    node->mirror_impl_v_last_child = ghost_null;
    node->mirror_impl_v_weight = 2;
})

/**
 * Returns the first node or null if the map is empty.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_first(mirror_iwbt_t* tree) {
    mirror_impl_iwbt_sanity_check(tree);
    if (tree->mirror_impl_v_root == ghost_null)
        return ghost_null;
    return mirror_iwbt_node_first_in_subtree(tree->mirror_impl_v_root);
}


/**
 * Returns the number of elements in the tree.
 */
ghost_impl_inline
ghost_size_t mirror_iwbt_count(mirror_iwbt_t* mirror_impl_v_tree) {
    ghost_assert(mirror_impl_v_tree != ghost_null, "");
    mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);
    return mirror_iwbt_node_count(mirror_impl_v_tree->mirror_impl_v_root);
}


/**
 * Gets the node at the given index.
 */
ghost_impl_function
mirror_iwbt_node_t* mirror_iwbt_at(mirror_iwbt_t* mirror_impl_v_tree, ghost_size_t mirror_impl_v_index) GHOST_IMPL_DEF({
    mirror_iwbt_node_t* mirror_impl_v_node;
    ghost_assert(mirror_impl_v_tree->mirror_impl_v_root != ghost_null, "");
    ghost_assert(mirror_impl_v_index < mirror_iwbt_count(mirror_impl_v_tree), "");
    mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);

    mirror_impl_v_node = mirror_impl_v_tree->mirror_impl_v_root;
    while (mirror_impl_v_node) {
        ghost_size_t left_size = mirror_impl_iwbt_weight(mirror_impl_v_node->mirror_impl_v_first_child) - 1;
        if (mirror_impl_v_index == left_size)
            return mirror_impl_v_node;
        if (mirror_impl_v_index < left_size) {
            mirror_impl_v_node = mirror_impl_v_node->mirror_impl_v_first_child;
        } else {
            mirror_impl_v_index -= left_size + 1;
            mirror_impl_v_node = mirror_impl_v_node->mirror_impl_v_last_child;
        }
    }

    ghost_unreachable(ghost_null);
})

/**
 * Clears the tree.
 *
 * This is equivalent to zeroing the tree. It can be used to initialize an
 * uninitialized tree.
 */
ghost_impl_inline
void mirror_iwbt_clear(mirror_iwbt_t* mirror_impl_v_tree) {
    ghost_assert(mirror_impl_v_tree != ghost_null, "");
    mirror_impl_v_tree->mirror_impl_v_root = ghost_null;
}

/**
 * Gets the index of the given node.
 *
 * TODO should this accept null? probably not, maybe a separate rank function
 * should accept null? or that's way too confusing, probably it should be
 * index_opt() or something
 */
ghost_impl_function
ghost_size_t mirror_iwbt_index(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    ghost_size_t i;
    mirror_iwbt_node_t* parent;

    ghost_assert(tree != ghost_null, "");
    ghost_assert(node != ghost_null, "");
    ghost_discard(tree);

    i = 0;
    if (node->mirror_impl_v_first_child)
        i += node->mirror_impl_v_first_child->mirror_impl_v_weight - 1;

    parent = node->mirror_impl_v_parent;
    while (parent) {
        if (node == parent->mirror_impl_v_last_child) {
            i += 1;
            if (parent->mirror_impl_v_first_child != ghost_null)
                i += parent->mirror_impl_v_first_child->mirror_impl_v_weight - 1;
        }
        node = parent;
        parent = node->mirror_impl_v_parent;
    }

    return i;
})

/**
 * Insert a node at the start of the tree.
 */
ghost_impl_function
void mirror_iwbt_insert_first(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    mirror_iwbt_log(("inserting %p first\n", ghost_static_cast(void*, node)));
    if (tree->mirror_impl_v_root == ghost_null) {
        mirror_impl_iwbt_insert_into_empty_tree(tree, node);
    } else {
        mirror_iwbt_node_t* reference = mirror_iwbt_node_first_in_subtree(tree->mirror_impl_v_root);
        mirror_impl_iwbt_insert_as_first_child(tree, reference, node);
            mirror_iwbt_log(("%i\n",__LINE__));
        mirror_impl_iwbt_rebalance(tree, reference);
    }
})

/**
 * Inserts a node after the given reference node.
 *
 * If reference_node is null, the node is inserted at the start of the list.
 *
 * TODO probably should have insert_after_opt() to allow reference node to be null
 */
ghost_impl_function
void mirror_iwbt_insert_after(mirror_iwbt_t* tree,
        mirror_iwbt_node_t* reference_node,
        mirror_iwbt_node_t* node_to_insert) GHOST_IMPL_DEF(
{
    mirror_iwbt_log(("inserting %p after %p\n",
            ghost_static_cast(void*, node_to_insert),
            ghost_static_cast(void*, reference_node)));

    if (reference_node == ghost_null) {
        mirror_iwbt_insert_first(tree, node_to_insert);
        return;
    }

    if (reference_node->mirror_impl_v_last_child == ghost_null) {
        mirror_impl_iwbt_insert_as_last_child(tree, reference_node, node_to_insert);
    } else {
        reference_node = mirror_iwbt_node_first_in_subtree(reference_node->mirror_impl_v_last_child);
        mirror_impl_iwbt_insert_as_first_child(tree, reference_node, node_to_insert);
    }
})

/**
 */
ghost_impl_function
void mirror_iwbt_insert_last(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    mirror_iwbt_log(("inserting %p last\n", ghost_static_cast(void*, node)));
    if (tree->mirror_impl_v_root == ghost_null) {
        mirror_impl_iwbt_insert_into_empty_tree(tree, node);
    } else {
        mirror_iwbt_node_t* reference = mirror_iwbt_node_last_in_subtree(tree->mirror_impl_v_root);
        mirror_impl_iwbt_insert_as_last_child(tree, reference, node);
            mirror_iwbt_log(("%i\n",__LINE__));
        mirror_impl_iwbt_rebalance(tree, reference);
    }
})

/**
 * Inserts a node before the given reference node.
 *
 * If current_node is null, the node is inserted at the end of the list.
 */
ghost_impl_function
void mirror_iwbt_insert_before(mirror_iwbt_t* tree,
        mirror_iwbt_node_t* reference_node,
        mirror_iwbt_node_t* node_to_insert) GHOST_IMPL_DEF(
{
    mirror_iwbt_log(("inserting %p before %p\n",
            ghost_static_cast(void*, node_to_insert),
            ghost_static_cast(void*, reference_node)));

    if (reference_node == ghost_null) {
        mirror_iwbt_insert_last(tree, node_to_insert);
        return;
    }

    if (reference_node->mirror_impl_v_first_child == ghost_null) {
        mirror_impl_iwbt_insert_as_first_child(tree, reference_node, node_to_insert);
    } else {
        reference_node = mirror_iwbt_node_last_in_subtree(reference_node->mirror_impl_v_first_child);
        mirror_impl_iwbt_insert_as_last_child(tree, reference_node, node_to_insert);
    }
})

/**
 * Inserts a node at the given index.
 */
ghost_impl_function
void mirror_iwbt_insert_at(mirror_iwbt_t* mirror_impl_v_tree,
        ghost_size_t mirror_impl_v_index,
        mirror_iwbt_node_t* mirror_impl_v_node) GHOST_IMPL_DEF(
{
    mirror_iwbt_log(("inserting %p at %zi\n", ghost_static_cast(void*, mirror_impl_v_node), mirror_impl_v_index));
    if (mirror_impl_v_tree->mirror_impl_v_root == ghost_null) {
        mirror_impl_iwbt_insert_into_empty_tree(mirror_impl_v_tree, mirror_impl_v_node);
    } else if (mirror_impl_v_index == mirror_iwbt_count(mirror_impl_v_tree)) {
        mirror_iwbt_insert_last(mirror_impl_v_tree, mirror_impl_v_node);
    } else {
        mirror_iwbt_insert_before(mirror_impl_v_tree, mirror_iwbt_at(mirror_impl_v_tree, mirror_impl_v_index), mirror_impl_v_node);
    }
})

/**
 * Returns true if the tree is empty and false otherwise.
 */
ghost_impl_inline
ghost_bool mirror_iwbt_is_empty(mirror_iwbt_t* tree) {
    mirror_impl_iwbt_sanity_check(tree);
    return tree->mirror_impl_v_root == ghost_null;
}

/**
 * Returns the last node or null if the map is empty.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_last(mirror_iwbt_t* tree) {
    mirror_impl_iwbt_sanity_check(tree);
    if (tree->mirror_impl_v_root == ghost_null)
        return ghost_null;
    return mirror_iwbt_node_last_in_subtree(tree->mirror_impl_v_root);
}

/**
 */
/* TODO remove tree parameter? or separate this out into always_inline function that ignores tree parameter */
ghost_impl_function
mirror_iwbt_node_t* mirror_iwbt_next(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    ghost_discard(tree);
    mirror_iwbt_log(("next of %p\n", ghost_static_cast(void*, node)));

    /* If we have a right child, we return its leftmost leaf. */
    if (node->mirror_impl_v_last_child != ghost_null) {
        mirror_iwbt_log(("we have a right child, so we're finding its leftmost leaf\n"));
        return mirror_iwbt_node_first_in_subtree(node->mirror_impl_v_last_child);
    }

    /* Otherwise we walk up until we are the left child of a parent. */
    for (;;) {
        mirror_iwbt_node_t* parent = node->mirror_impl_v_parent;
        if (parent == ghost_null)
            break;
        mirror_iwbt_log(("found parent %p\n", ghost_static_cast(void*, parent)));
        if (parent->mirror_impl_v_first_child == node) {
            mirror_iwbt_log(("node is left child, so next is parent %p\n", ghost_static_cast(void*, parent)));
            return parent;
        }
        node = parent;
    }

    /* If we haven't found anything, we are the last node in the list. */
    mirror_iwbt_log(("no more parents. no next.\n"));
    return ghost_null;
})

/**
 * Returns the last child of the given node, or null if it has no last child.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_node_last_child(mirror_iwbt_node_t* mirror_impl_v_node) {
    ghost_assert(mirror_impl_v_node != ghost_null, "");
    return mirror_impl_v_node->mirror_impl_v_last_child;
}

/**
 * Returns the parent of the given node, or null if it is the root of the tree.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_node_parent(mirror_iwbt_node_t* mirror_impl_v_node) {
    ghost_assert(mirror_impl_v_node != ghost_null, "");
    return mirror_impl_v_node->mirror_impl_v_parent;
}

/**
 */
/* TODO remove tree parameter? or separate this out into always_inline function that ignores tree parameter */
ghost_impl_function
mirror_iwbt_node_t* mirror_iwbt_previous(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) GHOST_IMPL_DEF({
    ghost_discard(tree);
    mirror_iwbt_log(("previous of %p\n", ghost_static_cast(void*, node)));

    /* If we have a left child, we return its rightmost leaf. */
    if (node->mirror_impl_v_first_child != ghost_null) {
        mirror_iwbt_log(("we have a left child, so we're finding its rightmost leaf\n"));
        return mirror_iwbt_node_last_in_subtree(node->mirror_impl_v_first_child);
    }

    /* Otherwise we walk up until we are the right child of a parent. */
    for (;;) {
        mirror_iwbt_node_t* parent = node->mirror_impl_v_parent;
        if (parent == ghost_null)
            break;
        mirror_iwbt_log(("found parent %p\n", ghost_static_cast(void*, parent)));
        if (parent->mirror_impl_v_last_child == node) {
            mirror_iwbt_log(("node is right child, so previous is parent %p\n", ghost_static_cast(void*, parent)));
            return parent;
        }
        node = parent;
    }

    /* If we haven't found anything, we are the first node in the list. */
    mirror_iwbt_log(("no more parents. no previous.\n"));
    return ghost_null;
})

/**
 * Remove the given node.
 */
ghost_impl_function
void mirror_iwbt_remove(mirror_iwbt_t* mirror_impl_v_tree, mirror_iwbt_node_t* mirror_impl_v_node) GHOST_IMPL_DEF({
    mirror_iwbt_node_t* parent = mirror_impl_v_node->mirror_impl_v_parent;
    mirror_iwbt_log(("removing %p\n", ghost_static_cast(void*, mirror_impl_v_node)));
    mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);

    /* if the mirror_impl_v_node has no children, we can remove it directly. */
    if (mirror_impl_v_node->mirror_impl_v_weight == 2) {
        ghost_assert(mirror_impl_v_node->mirror_impl_v_first_child == ghost_null, "");
        ghost_assert(mirror_impl_v_node->mirror_impl_v_last_child == ghost_null, "");

        if (parent == ghost_null) {
            ghost_assert(mirror_impl_v_tree->mirror_impl_v_root == mirror_impl_v_node, "");
            mirror_impl_v_tree->mirror_impl_v_root = ghost_null;
        } else {
            if (parent->mirror_impl_v_first_child == mirror_impl_v_node) {
                parent->mirror_impl_v_first_child = ghost_null;
            } else {
                ghost_assert(parent->mirror_impl_v_last_child == mirror_impl_v_node, "");
                parent->mirror_impl_v_last_child = ghost_null;
            }
            mirror_impl_iwbt_decrement_to_root(parent);
            mirror_iwbt_log(("%i\n",__LINE__));
            mirror_impl_iwbt_rebalance(mirror_impl_v_tree, parent);
        }
        mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);
        return;
    }

    /* If the mirror_impl_v_node has only one child, we can replace it with that child. */
    if (mirror_impl_v_node->mirror_impl_v_first_child == ghost_null || mirror_impl_v_node->mirror_impl_v_last_child == ghost_null) {
        mirror_iwbt_node_t* child = (mirror_impl_v_node->mirror_impl_v_first_child != ghost_null) ? mirror_impl_v_node->mirror_impl_v_first_child : mirror_impl_v_node->mirror_impl_v_last_child;
        ghost_assert(child->mirror_impl_v_parent == mirror_impl_v_node, "");

        if (parent == ghost_null) {
            ghost_assert(mirror_impl_v_tree->mirror_impl_v_root == mirror_impl_v_node, "");
            mirror_impl_v_tree->mirror_impl_v_root = child;
            child->mirror_impl_v_parent = ghost_null;
            mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);
        } else {
            if (parent->mirror_impl_v_first_child == mirror_impl_v_node) {
                parent->mirror_impl_v_first_child = child;
            } else {
                ghost_assert(parent->mirror_impl_v_last_child == mirror_impl_v_node, "");
                parent->mirror_impl_v_last_child = child;
            }
            child->mirror_impl_v_parent = parent;
            mirror_impl_iwbt_decrement_to_root(parent);
            mirror_iwbt_log(("%i\n",__LINE__));
            mirror_impl_iwbt_rebalance(mirror_impl_v_tree, parent);
        }
        mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);
        return;
    }

    /* Otherwise we replace it with an adjacent child, chosen from the side
        * with the most entries. Since the mirror_impl_v_node has two children and the
        * replacement is adjacent, the replacement will have at most one child and
        * it will be pointing away from us. */
    {
        mirror_iwbt_node_t* replacement;
        mirror_iwbt_node_t* replacement_child;
        mirror_iwbt_node_t* replacement_parent;
        if (mirror_impl_v_node->mirror_impl_v_first_child->mirror_impl_v_weight > mirror_impl_v_node->mirror_impl_v_last_child->mirror_impl_v_weight) {
            replacement = mirror_iwbt_node_last_in_subtree(mirror_impl_v_node->mirror_impl_v_first_child);
            ghost_assert(replacement != ghost_null, "");
            ghost_assert(replacement->mirror_impl_v_last_child == ghost_null, "");
            replacement_child = replacement->mirror_impl_v_first_child;
        } else {
            replacement = mirror_iwbt_node_first_in_subtree(mirror_impl_v_node->mirror_impl_v_last_child);
            ghost_assert(replacement != ghost_null, "");
            ghost_assert(replacement->mirror_impl_v_first_child == ghost_null, "");
            replacement_child = replacement->mirror_impl_v_last_child;
        }
        replacement_parent = replacement->mirror_impl_v_parent;

        /* TODO we have mirror_iwbt_replace(), should share this code.
            * just need to move it to a mirror_iwbt_impl_replace() that
            * skips sanity checks the balancing is separate */

        /* We start by removing the replacement from the mirror_impl_v_tree. */
        if (replacement_parent->mirror_impl_v_first_child == replacement) {
            replacement_parent->mirror_impl_v_first_child = replacement_child;
        } else {
            ghost_assert(replacement_parent->mirror_impl_v_last_child == replacement, "");
            replacement_parent->mirror_impl_v_last_child = replacement_child;
        }
        if (replacement_child != ghost_null) {
            replacement_child->mirror_impl_v_parent = replacement_parent;
        }

        /* We can now replace ourselves with the replacement. */
        replacement->mirror_impl_v_first_child = mirror_impl_v_node->mirror_impl_v_first_child;
        replacement->mirror_impl_v_last_child = mirror_impl_v_node->mirror_impl_v_last_child;
        if (mirror_impl_v_node->mirror_impl_v_first_child != ghost_null)
            mirror_impl_v_node->mirror_impl_v_first_child->mirror_impl_v_parent = replacement;
        if (mirror_impl_v_node->mirror_impl_v_last_child != ghost_null)
            mirror_impl_v_node->mirror_impl_v_last_child->mirror_impl_v_parent = replacement;
        if (parent == ghost_null) {
            ghost_assert(mirror_impl_v_tree->mirror_impl_v_root == mirror_impl_v_node, "");
            mirror_impl_v_tree->mirror_impl_v_root = replacement;
            replacement->mirror_impl_v_parent = ghost_null;
        } else {
            if (parent->mirror_impl_v_first_child == mirror_impl_v_node) {
                parent->mirror_impl_v_first_child = replacement;
            } else {
                ghost_assert(parent->mirror_impl_v_last_child == mirror_impl_v_node, "");
                parent->mirror_impl_v_last_child = replacement;
            }
            replacement->mirror_impl_v_parent = mirror_impl_v_node->mirror_impl_v_parent;
        }

        /* Lastly, we re-calculate sizes and rebalance. If the replacement parent
            * was not us, then it is some mirror_impl_v_node deeper in the mirror_impl_v_tree, so that's where we
            * need to start. Otherwise we start at the replacement. */
        {
            mirror_iwbt_node_t* fix = (replacement_parent != mirror_impl_v_node) ? replacement_parent : replacement;

            /* TODO: maybe remove this, decrement, increment, etc. and make it all part of rebalance */
            mirror_iwbt_node_t* recalculate = fix;
            while (recalculate != ghost_null) {
                mirror_impl_iwbt_recalculate_size(recalculate);
                recalculate = recalculate->mirror_impl_v_parent;
            }

            mirror_iwbt_log(("%i\n",__LINE__));
            mirror_impl_iwbt_rebalance(mirror_impl_v_tree, fix);
            mirror_impl_iwbt_sanity_check(mirror_impl_v_tree);
        }
    }
})

/**
 * Remove the given node, returning the node after it or null if this was the
 * last node.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_remove_and_next(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) {
    mirror_iwbt_node_t* next;
    ghost_assert(tree != ghost_null, "");
    ghost_assert(node != ghost_null, "");
    next = mirror_iwbt_next(tree, node);
    mirror_iwbt_remove(tree, node);
    return next;
}

/**
 * Remove the given node, returning the node before it or null if this was the
 * first node.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_remove_and_previous(mirror_iwbt_t* tree, mirror_iwbt_node_t* node) {
    mirror_iwbt_node_t* previous;
    ghost_assert(tree != ghost_null, "");
    ghost_assert(node != ghost_null, "");
    previous = mirror_iwbt_previous(tree, node);
    mirror_iwbt_remove(tree, node);
    return previous;
}

/**
 * Removes any node and returns it, or returns null if the map is empty.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_remove_any(mirror_iwbt_t* tree) {
    mirror_iwbt_node_t* node = mirror_iwbt_root(tree);
    if (node != ghost_null)
        mirror_iwbt_remove(tree, node);
    return node;
}

/**
 * Remove the node at the given index, returning it.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_remove_at(mirror_iwbt_t* mirror_impl_v_tree, ghost_size_t mirror_impl_v_index) {
    mirror_iwbt_node_t* node;
    mirror_iwbt_log(("removing at %zi\n", mirror_impl_v_index));
    node = mirror_iwbt_at(mirror_impl_v_tree, mirror_impl_v_index);
    mirror_iwbt_remove(mirror_impl_v_tree, node);
    return node;
}

/**
 */
/* mirror_iwbt_remove_first() TODO */

/**
 */
/* mirror_iwbt_remove_last TODO */

/**
 * Replace the node at the given index, returning the previous node.
 *
 * The new node must not already be in the tree.
 */
ghost_impl_inline
mirror_iwbt_node_t* mirror_iwbt_replace_at(
        mirror_iwbt_t* mirror_impl_v_tree,
        ghost_size_t mirror_impl_v_index,
        mirror_iwbt_node_t* mirror_impl_v_new_node)
{
    mirror_iwbt_node_t* old_node;
    mirror_iwbt_log(("replacing at %zi with %p\n", mirror_impl_v_index, ghost_static_cast(void*, mirror_impl_v_new_node)));
    old_node = mirror_iwbt_at(mirror_impl_v_tree, mirror_impl_v_index);
    mirror_iwbt_replace(mirror_impl_v_tree, old_node, mirror_impl_v_new_node);
    return old_node;
}

/**
 * Swaps the contents of two trees.
 */
ghost_impl_inline
void mirror_iwbt_swap(mirror_iwbt_t* mirror_impl_v_left, mirror_iwbt_t* mirror_impl_v_right) {
    mirror_impl_iwbt_sanity_check(mirror_impl_v_left);
    mirror_impl_iwbt_sanity_check(mirror_impl_v_right);
    {
        mirror_iwbt_t temp = *mirror_impl_v_left;
        *mirror_impl_v_left = *mirror_impl_v_right;
        *mirror_impl_v_right = temp;
    }
}

GHOST_IMPL_FUNCTION_CLOSE

#endif
