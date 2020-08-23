#include <stdio.h>
#include <unistd.h>

struct rb_node {
        unsigned long  __rb_parent_color;
        struct rb_node *rb_right;
        struct rb_node *rb_left;
	unsigned long key;
};

struct rb_root {
        struct rb_node *rb_node;
};

#define rb_parent(r)   ((struct rb_node *)((r)->__rb_parent_color & ~3))

#define RB_RED          0
#define RB_BLACK        1

#define __rb_parent(pc)    ((struct rb_node *)(pc & ~3))

#define __rb_color(pc)     ((pc) & 1)
#define __rb_is_black(pc)  __rb_color(pc)
#define __rb_is_red(pc)    (!__rb_color(pc))
#define rb_color(rb)       __rb_color((rb)->__rb_parent_color)
#define rb_is_red(rb)      __rb_is_red((rb)->__rb_parent_color)
#define rb_is_black(rb)    __rb_is_black((rb)->__rb_parent_color)

int rb_tree_validate_parent(struct rb_node *node)
{
	if (!node)
		return 0;

	if (!rb_parent(node))
		return 0;

	if(rb_parent(node)->rb_left != node 
			&& rb_parent(node)->rb_right != node )
		return 11;

	return 0;
}

int rb_tree_validate_color(struct rb_node *node)
{
	if (!node)
		return 0;

	if(rb_is_black(node))
		return 0;

	if(node->rb_left && rb_is_red(node->rb_left))
		return 1;

	if(node->rb_right && rb_is_red(node->rb_right))
		return 2;

	return 0;
}

int rb_tree_validate_depth(struct rb_node *node, int depth, int curr_depth)
{
	if (node->rb_left && node->rb_right)
			return 0;

	return depth != curr_depth;
}

/* Try with best effort to avoid recursive and addtional stack. */
int firo_inorder_traversal(struct rb_node *root)
{
        struct rb_node *tmp, *curr, *rightmost;
        int depth = 0, curr_depth = 0, rv = 0;

        rightmost = tmp = root;
        while(tmp = tmp->rb_right)
		rightmost = tmp;

        curr = tmp = root;
	while (tmp = tmp->rb_left) {
                depth += rb_is_black(tmp);
		curr = tmp;		// Leftmost
	}

	curr_depth = depth;
	/* We traverse all nodes from leftmost to rightmost */
        while (1)
        {
		printf("debug %d \n", curr->key);
		(void)fflush(stdout);
		rv = rb_tree_validate_parent(curr);
		if (rv) {
			printf("BUG %d parent pointer corrupted.\n", curr->key);
			return rv;
		}

		// printf("debug %d parent\n", curr->key);
		rv = rb_tree_validate_color(curr);
		if (rv)
			return rv;

		printf("node %d depth %d\n", curr->key, curr_depth);
		rv = rb_tree_validate_depth(curr, depth, curr_depth);
		if (rv) {
			printf("BUG %d depth\n", curr->key);
			return rv;
		}

		//sleep(1);
		if (curr == rightmost)
			goto out;

		/* Try to find the successor */
		if (curr->rb_right)
		{
			/* Successor is in the right sub-tree */
			curr = curr->rb_right;
			curr_depth += rb_is_black(curr);
			while (curr->rb_left) {
				curr = curr->rb_left;
				curr_depth += rb_is_black(curr);
			}
		} else {
			/* Successor is in upper nodes */
			while ((tmp = rb_parent(curr)) && 
						curr == tmp->rb_right) {
				curr_depth -= rb_is_black(curr);
				curr = tmp;
			}
			curr_depth -= rb_is_black(curr);
			curr = tmp;
		}
        }

out:
        return rv;    
}


int rb_tree_validate(struct rb_root *root)
{
	struct rb_node *n;
	if (!root)
		return 0;

	n =  root->rb_node;
	if (!n)
		return 0;

	if (rb_is_red(n)) {
		printf("Root node is red\n");
		return 1;
	}

	return firo_inorder_traversal(n);
}

static void rb_set_parent_color(struct rb_node *rb,
                                       struct rb_node *p, int color)
{
        rb->__rb_parent_color = (unsigned long)p | color;
}

static void rb_set_l_r(struct rb_node *n, struct rb_node *l, struct rb_node *r)
{
	n->rb_left = l;
	n->rb_right = r;
}

int main()
{
	struct rb_root root;
	struct rb_node n1, n2, n3, n4, n5, n6;
	int rv;
/* 
            1 
          /   \
        2      3 
      /  \	\
    4     5	6
*/
	n1.key = 1;
	n2.key = 2;
	n3.key = 3;
	n4.key = 4;
	n5.key = 5;
	n6.key = 6;

	rb_set_parent_color(&n1, NULL, RB_BLACK);
	//rb_set_l_r(&n1, NULL, NULL);
	rb_set_l_r(&n1, &n2, &n3);

	rb_set_parent_color(&n2, &n1, RB_RED);
	rb_set_l_r(&n2, &n4, &n5);

	rb_set_parent_color(&n3, &n1, RB_BLACK);
	rb_set_l_r(&n3, NULL, &n6);

	rb_set_parent_color(&n4, &n2, RB_BLACK);
	rb_set_l_r(&n4, NULL, NULL);

	rb_set_parent_color(&n5, &n2, RB_BLACK);
	rb_set_l_r(&n5, NULL, NULL);

	rb_set_parent_color(&n6, &n3, RB_BLACK);
	rb_set_l_r(&n6, NULL, NULL);


	root.rb_node = &n1;
	rv = rb_tree_validate(&root);
	printf("rb-tree %d\n", rv);

	return 0;
}
