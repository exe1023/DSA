#include <stdio.h>
#include <stdlib.h>
#include "avl.h"
#include "bst.h"
#include "rb.h"
#include <string.h>
void preorder_integer_avl(const struct avl_node *node)
{
	if(node == NULL)
		return;
	printf("%s " , (char *)node->avl_data);
	if(node->avl_link[0] != NULL || node->avl_link[1] != NULL)
	{
		putchar('(');
		preorder_integer_avl(node->avl_link[0]);
		putchar(',');
		putchar(' ');
		preorder_integer_avl(node->avl_link[1]);
		putchar(')');
	}
}
void preorder_integer_bst(const struct bst_node *node)
{
	if(node == NULL)
		return;
	printf("%s " , (char *)node->bst_data);
	if(node->bst_link[0] != NULL || node->bst_link[1] != NULL)
	{
		putchar('(');
		preorder_integer_bst(node->bst_link[0]);
		putchar(',');
		putchar(' ');
		preorder_integer_bst(node->bst_link[1]);
		putchar(')');
	}
}
void preorder_integer_rb(const struct rb_node *node)
{
	if(node == NULL)
		return;
	printf("%s " , (char *)node->rb_data);
	if(node->rb_link[0] != NULL || node->rb_link[1] != NULL)
	{
		putchar('(');
		preorder_integer_rb(node->rb_link[0]);
		putchar(',');
		putchar(' ');
		preorder_integer_rb(node->rb_link[1]);
		putchar(')');
	}
}
int string_compare(const void *pa , const void *pb , void *param)
{
	const char *a = (const char *)pa;
	const char *b = (const char *)pb;
	if(strcmp(a , b) < 0) return -1;
	else if(strcmp(a , b) > 0) return +1;
	else return 0;
}

int main(){
	struct avl_table *avltree;
	struct bst_table *bsttree;
	struct rb_table *rbtree;
	avltree = avl_create(string_compare , NULL , NULL);
	bsttree = bst_create(string_compare , NULL , NULL);
	rbtree = rb_create(string_compare , NULL , NULL);
	for(int i = 0 ; i < 32 ; i++)
	{
		char *element = (char *)malloc(200);
		scanf("%s" , element);
		avl_probe(avltree , element);
		bst_probe(bsttree , element);
		rb_probe(rbtree , element);
	}
	preorder_integer_avl(avltree->avl_root);
	printf("\n");
	preorder_integer_bst(bsttree->bst_root);
	printf("\n");
	preorder_integer_rb(rbtree->rb_root);
	printf("\n");
	return 0;
}