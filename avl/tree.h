
#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "freeitem.h"
#include "compare.h"
#include "node.h"

typedef struct avl_tree_t {
	avl_node_t *head;
	avl_node_t *tail;
	avl_node_t *top;
	
	avl_compare_t cmp;
	
	avl_freeitem_t freeitem;
} avl_tree_t;

#endif
