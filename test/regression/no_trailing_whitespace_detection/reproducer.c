/* Austin Barrow, hw11.c, CS 24000, Fall 2021
 * Last updated 12 November 2021
 */

/* Add any includes here */
#include "hw11.h"

#include <assert.h>
#include <malloc.h>

/* Define any global variables here */
int g_values[MAX_NODE_SIZE] = {0};

/* Define your functions here */

/*
 *  traverse_order_check(int given_traverse_order)
 *  This function takes a given traverse order and returns false if invalid or
 *  true if valid.
 */

int traverse_order_check(int given_traverse_order) {
  switch (given_traverse_order) {
    case PREFIX: {
      return 1;
    }
    case POSTFIX: {
      return 1;
    }
    case FORWARD: {
      return 1;
    }
    case REVERSE: {
      return 1;
    }
    case -10:
    case -11:
      case -12: {
        return -1;

      }
    case -13:
    case -14:
    case -15:
      return -1;
    default: {
      return 0;
    }
  }
} /* traverse_order_check() */

/*
 *  create_node(int given_value)
 *  This function creates a new node with the given value, properly allocating
 *  memory in the process.
 */

tree_node_t * create_node(int given_value) {
  assert(given_value > 0);

  tree_node_t * created_tree = NULL;
  created_tree = malloc(sizeof(tree_node_t));
  assert(created_tree != NULL);

  created_tree->value = given_value;

  created_tree->left_child_ptr = NULL;
  created_tree->right_child_ptr = NULL;

  return created_tree;
} /* create_node() */

/*
 *  insert_node(tree_node_t ** root_tree_element_ptr,
 *              tree_node_t * new_tree_element)
 *  This function inserts a new given node into the given binary tree in the
 *  proper blace according to the value of the nodes.
 */

void insert_node(tree_node_t ** root_tree_element_ptr,
                 tree_node_t * new_tree_element) {
  assert(root_tree_element_ptr != NULL);
  assert(new_tree_element != NULL);
  assert(new_tree_element->right_child_ptr == NULL);
  assert(new_tree_element->left_child_ptr == NULL);

  tree_node_t * root_tree_element = *root_tree_element_ptr;

  if (root_tree_element == NULL) {
    *root_tree_element_ptr = new_tree_element;
    return;
  }

  if (new_tree_element->value <= root_tree_element->value) {
    if (root_tree_element->left_child_ptr == NULL) {
      root_tree_element->left_child_ptr = new_tree_element;
      return;
    }
    else {
      insert_node(&root_tree_element->left_child_ptr, new_tree_element);
    }
  }
  else {
    if (root_tree_element->right_child_ptr == NULL) {
      root_tree_element->right_child_ptr = new_tree_element;
      return;
    }
    else {
      insert_node(&root_tree_element->right_child_ptr, new_tree_element);
    }
  }
} /* insert_node() */

/*
 *  search_node(tree_node_t * root_node, int given_value)
 *  This function searches for the node within the given binary tree that has
 *  the same value as the given value. If found, it returns the node, and if
 *  not found, it returns NULL.
 */

tree_node_t * search_node(tree_node_t * root_node, int given_value) {
  assert(given_value > 0);

  if (root_node == NULL) {
    return NULL;
  }

  if (given_value == root_node->value) {
    return root_node;
  }
  else if (given_value < root_node->value) {
    return search_node(root_node->left_child_ptr, given_value);
  }

  return search_node(root_node->right_child_ptr, given_value);
} /* search_node() */

/*
 *  delete_tree(tree_node_t ** given_tree)
 *  This functions takes a given tree and deallocates every node within the
 *  tree.
 */

void delete_tree(tree_node_t ** given_tree) {
  assert(given_tree != NULL);

  tree_node_t * given_tree_element = *given_tree;
  if (given_tree_element == NULL) {
    return;
  }

  delete_tree(&(given_tree_element->left_child_ptr));
  delete_tree(&(given_tree_element->right_child_ptr));

  free(given_tree_element);
  given_tree_element = NULL;

  return;
} /* delete_tree() */

/*
 *  get_max_depth(tree_node_t * given_tree)
 *  This function takes a given binary tree and calculates the max depth of the
 *  tree. It then returns this value.
 */

int get_max_depth(tree_node_t * given_tree) {
  if (given_tree == NULL) {
    return 0;
  }
  else {
    int depth_left_tree = get_max_depth(given_tree->left_child_ptr);
    int depth_right_tree = get_max_depth(given_tree->right_child_ptr);

    if (depth_left_tree > depth_right_tree) {
      return depth_left_tree + 1;
    }
    else {
      return depth_right_tree + 1;
    }
  }
} /* get_max_depth() */

/*
 *  prefix_traversal(tree_node_t * given_node, int g_values_index)
 *  This recursive function does a prefix traversal through tree and assigns
 *  values to array.
 */

int prefix_traversal(tree_node_t * given_node, int g_values_index) {
  if (given_node == NULL) {
    return g_values_index;
  }

  g_values[g_values_index++] = given_node->value;

  g_values_index =
    prefix_traversal(given_node->left_child_ptr, g_values_index);

  g_values_index =
    prefix_traversal(given_node->right_child_ptr, g_values_index);

  return g_values_index;
} /* prefix_traversal() */

/*
 *  postfix_traversal(tree_node_t * given_node, int g_values_index) 
 *  This recursive function does a postfix traversal through tree and assigns
 *  values to array.
 */

int postfix_traversal(tree_node_t * given_node, int g_values_index) {
  if (given_node == NULL) {
    return g_values_index;
  }

  g_values_index =
    postfix_traversal(given_node->left_child_ptr, g_values_index);

  g_values_index =
    postfix_traversal(given_node->right_child_ptr, g_values_index);

  g_values[g_values_index++] = given_node->value;

  return g_values_index;

} /* postfix_traversal() */

/*
 *  forward_traversal(tree_node_t * given_node, int g_values_index)
 *  This recursive function does a forward traversal through tree and assigns
 *  values to array.
 */

int forward_traversal(tree_node_t * given_node, int g_values_index) {
  if (given_node == NULL) {
    return g_values_index;
  }

  g_values_index =
    forward_traversal(given_node->left_child_ptr, g_values_index);

  g_values[g_values_index++] = given_node->value;

  g_values_index =
    forward_traversal(given_node->right_child_ptr, g_values_index);

  return g_values_index;

} /* forward_traversal() */

/*
 *  reverse_traversal(tree_node_t * given_node, int g_values_index)
 *  This recursive function does a reverse traversal through tree and assigns
 *  values to array.
 */

int reverse_traversal(tree_node_t * given_node, int g_values_index) {
  if (given_node == NULL) {
    return g_values_index;
  }
  g_values_index =
    reverse_traversal(given_node->right_child_ptr, g_values_index);

  g_values[g_values_index++] = given_node->value;

  g_values_index =
    reverse_traversal(given_node->left_child_ptr, g_values_index);

  return g_values_index;

} /* reverse_traversal() */

/*
 *  find_values_by_order(tree_node_t * given_root, int given_traversal_order)
 *  This function utilizes the previous recursive functions after determining
 *  the correct traversal order. After the recursive functions are done, it
 *  outputs the index which corresponds to the number of tree nodes traversed.
 */

int find_values_by_order(tree_node_t * given_root, int given_traversal_order) {
  assert(traverse_order_check(given_traversal_order) == 1);

  if (given_root == NULL) {
    return 0;
  }

  int g_values_index = 0;

  switch (given_traversal_order) {
    case PREFIX: {
      g_values_index = prefix_traversal(given_root, g_values_index);
      break;
    }
    case POSTFIX: {
      g_values_index = postfix_traversal(given_root, g_values_index);
      break;
    }
    case FORWARD: {
      g_values_index = forward_traversal(given_root, g_values_index);
      break;
    }
    case REVERSE: {
      g_values_index = reverse_traversal(given_root, g_values_index);
      break;
    }
  }

  return g_values_index;
} /* find_values_by_order() */
