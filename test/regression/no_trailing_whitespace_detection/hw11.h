#ifndef HW11_H
#define HW11_H

/* Constant definitions */

#define PREFIX        (1)
#define POSTFIX       (2)
#define FORWARD       (3)
#define REVERSE       (4)
#define MAX_NODE_SIZE (20)

/* Structure Definitions */

typedef struct tree_node {
  int value;

  struct tree_node *left_child_ptr;
  struct tree_node *right_child_ptr;
} tree_node_t;

/* Function prototypes */

tree_node_t *create_node(int);
void insert_node(tree_node_t **, tree_node_t *);
tree_node_t *search_node(tree_node_t *, int);
void delete_tree(tree_node_t **);
int get_max_depth(tree_node_t *);
int find_values_by_order(tree_node_t *, int);

extern int g_values[MAX_NODE_SIZE];

#endif // HW11_H
