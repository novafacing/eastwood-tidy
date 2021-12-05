/* Name, hw11.c, CS 24000, Fall 2021
 * Last updated Oct 26, 2021
 */

/* Add any includes here */
#include "hw11.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
/* Define any global variables here */
int g_values[MAX_NODE_SIZE] = {};

/* Define your functions here */

/* the following function creates a node using the given
 * value and allocates memory as necessary
 */

tree_node_t *create_node(int new_value) {
  assert(new_value > 0);

  tree_node_t *new_node = malloc(sizeof(tree_node_t));
  assert(new_node != NULL);
  new_node->value = new_value;
  new_node->left_child_ptr = NULL;
  new_node->right_child_ptr = NULL;

  return new_node;
} /* create_node() */

/* the following function inserts the passed node 
 * in the correct place based on the hash
 */

void insert_node(tree_node_t **tree_root, tree_node_t *new_node) {
  assert(tree_root != NULL);
  assert(new_node != NULL);
  assert(new_node->left_child_ptr == NULL);
  assert(new_node->right_child_ptr == NULL);

  if ((tree_root == NULL) || (*tree_root == NULL)) {
    (*tree_root) = new_node;
  }
  else {

    if ((*tree_root)->value > new_node->value) {
      if ((*tree_root)->left_child_ptr == NULL) {
        (*tree_root)->left_child_ptr = new_node;
      }
      else {
        insert_node(&(*tree_root)->left_child_ptr, new_node);
      }
    }
    else {
      if ((*tree_root)->right_child_ptr == NULL) {
        (*tree_root)->right_child_ptr = new_node;
      }
      else {
        insert_node(&(*tree_root)->right_child_ptr, new_node);
      }

    }

  }
} /* insert_node() */

/* the following function searches for the node in the tree
 * with the matching value
 */

tree_node_t *search_node(tree_node_t *tree_root, int search_value) {
  assert(search_value > 0);

  if ((tree_root == NULL)) {
    return NULL;
  }
  else {

    if ((tree_root)->value == search_value) {
      return tree_root;
    }
    else {

      tree_node_t *left_child = search_node(tree_root->left_child_ptr,
                                                        search_value);
      if (left_child != NULL) {
        return left_child;
      }
      tree_node_t *right_child = search_node(tree_root->right_child_ptr,
                                                              search_value);
      if (right_child != NULL) {
        return right_child;
      }
    }

  }
  return NULL;

} /* search_node() */

/* the following function deletes an entire tree
 * and frees the associated data
 */

void delete_tree(tree_node_t **tree_root) {
  assert(tree_root != NULL);
  if (*tree_root == NULL) {
    tree_root = NULL;
    return;
  }
  delete_tree(&(*tree_root)->left_child_ptr);
  delete_tree(&(*tree_root)->right_child_ptr);
  free((*tree_root)->left_child_ptr);
  (*tree_root)->left_child_ptr = NULL;
  free((*tree_root)->right_child_ptr);
  (*tree_root)->right_child_ptr = NULL;
  (*tree_root)->value = 0.0;
  free(*tree_root);
  (*tree_root) = NULL;


} /* delete_tree() */

/* the following functiong gets the
 * maximum depth of the tree passed by the argument
 */

int get_max_depth(tree_node_t *tree_root) {
  int depth = 0;
  if (tree_root == NULL) {
    return 0;
  }
  else {
    depth += 1;
    int right_child = get_max_depth(tree_root->right_child_ptr);
    int left_child = get_max_depth(tree_root->left_child_ptr);

    if (right_child > left_child) {
      return right_child + 1;
    }
    else {
      return left_child + 1;
    }
  }
} /* get_max_depth() */

/* the following function asserts the order value in
 * abovr function
 */

int check_order(int order) {
  if (((order == PREFIX)) || ((order == POSTFIX)) || ((order == FORWARD)) ||
                                                     ((order == REVERSE))) {
    return 1;
  }
  else {
    return 0;
  }
} /* check_order() */

/*The following function puts value in an array in in Prefix order
 *and puts it in the array in this order as well
 */

int traverse_preorder(tree_node_t *tree, int count) {
  if ((tree == NULL)) {
    return count;
  }
  g_values[count] = tree->value;
  ++count;
  count = traverse_preorder(tree->left_child_ptr, count);
  count = traverse_preorder(tree->right_child_ptr, count);
  return count;

} /* traverse_preorder() */

/*
 *the following function puts value in an array in in Postfix order
 */

int traverse_postorder(tree_node_t *tree, int count) {
  if ((tree == NULL)) {
    return count;
  }
  count = traverse_postorder(tree->left_child_ptr, count);
  count = traverse_postorder(tree->right_child_ptr, count);
  g_values[count] = tree->value;
  ++count;
  return count;

} /* traverse_postorder() */

/* the following function puts value in an array in in Forward order
 * and puts it into the array in this order as well
 */

int traverse_forward(tree_node_t *tree, int count) {
  if ((tree == NULL)) {
    return count;
  }
  count = traverse_forward(tree->left_child_ptr, count);
  g_values[count] = tree->value;
  ++count;
  count = traverse_forward(tree->right_child_ptr, count);
  return count;

} /* traverse_forward() */

/* the following function puts value in an array in in Reverse order
 * and puts it into the array in this order as well
 */

int traverse_reverse(tree_node_t *tree, int count) {
  if ((tree == NULL)) {
    return count;
  }
  count = traverse_reverse(tree->right_child_ptr, count);
  g_values[count] = tree->value;
  ++count;
  count = traverse_reverse(tree->left_child_ptr, count);
  return count;

} /* traverse_reverse() */

/* the following function puts value in an array in order based on the
 * the passed int that dictates the order
 */

int find_values_by_order(tree_node_t *tree_root, int order) {
  assert(check_order(order));
  if (tree_root == NULL) {
    return 0;
  }
  int counter = 0;
  if (order == PREFIX) {
    counter = traverse_preorder(tree_root, counter);
  }
  else if (order == POSTFIX) {
    counter = traverse_postorder(tree_root, counter);
  }
  else if (order == REVERSE) {
    counter = traverse_reverse(tree_root, counter);
  }
  else {
    counter = traverse_forward(tree_root, counter);
  }
  return counter;
} /* find_values_by_order() */
