extern "C" {
#include "avl_ntudsa.h"
}
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#define maxfriend 100000
using namespace std;
int int_compare(const void *pa , const void *pb , void *param)
{
	int a = *(const int *)pa;
	int b = *(const int *)pb;
	if(a < b) return -1;
	else if(a > b) return +1;
	else return 0;
}

void mergetree(struct avl_node *node, struct avl_table *bigger)
{
	if(node == NULL)
		return;
	for(int i = 0 ; i < node->avl_cnt ; i++)
		avl_probe(bigger , node->avl_data);
	if(node->avl_link[0] != NULL || node->avl_link[1] != NULL)
	{
		mergetree(node->avl_link[0] , bigger);
		mergetree(node->avl_link[1] , bigger);
	}
	delete node;
	return;
}

void checkmerge(int parent[] ,int depth[] ,struct avl_table *avltree[], int owner1 , int owner2 , int actualowner[])
{
	if(depth[owner1] >= depth[owner2])
	{
		mergetree(avltree[owner2]->avl_root , avltree[owner1]);
		parent[owner2] = owner1;
		if(depth[owner1] == depth[owner2])
			depth[owner1] ++;
	}
	else if(depth[owner1] < depth[owner2])
	{
		mergetree(avltree[owner1]->avl_root , avltree[owner2]);
		parent[owner1] = owner2;
		actualowner[owner2] = actualowner[owner1];
	}
}

int findparent(int parent[] , int number)
{
	if(parent[number] == number)
		return number;
	else
		return findparent(parent , parent[number]);
}
void maxgame(long long int s , struct avl_node *node , long long int &gamecnt)
{
	if(node == NULL)
		return;
	if( node->avl_sum[0] <= s)
	{
		gamecnt += node->avl_cnode[0];
		s -= node->avl_sum[0];
		long long int number = s / (long long int)node->avl_data;
		if(number > node->avl_cnt)
			number = node->avl_cnt;
		gamecnt += number;
		s -= (node->avl_data * number);
		if(number == node->avl_cnt)
		{
			if(node->avl_sum[1] <= s)
			{
				gamecnt += node->avl_cnode[1];
				s -= node->avl_sum[1];
			}
			else
			{
				maxgame(s , node->avl_link[1] , gamecnt);
			}
		}
	}
	else
	{
		maxgame(s , node->avl_link[0] , gamecnt);
	}
}

int main(void)
{
	int n , m;
	struct avl_table **avltree = new struct avl_table*[maxfriend];
	int depth[maxfriend] = {0} , parent[maxfriend] , actualowner[maxfriend];
	scanf("%d %d" , &n , &m);
	for(int i = 0 ; i < n ; i ++)
	{
		int price;
		scanf("%d" , &price);
		avltree[i] = avl_create(int_compare , NULL , NULL);
		avl_probe(avltree[i] , price);
		parent[i] = i;
		actualowner[i] = i;
	}
	for(int i = 0 ; i < m ; i++)
	{
		int command;
		scanf("%d" , &command);
		if(command == 1)
		{
			int boring , interest;
			scanf("%d %d" , &boring , &interest);
			int owner1 = findparent(parent , boring - 1) , owner2 = findparent(parent , interest - 1);
			if(owner1 != owner2)
			{
				checkmerge(parent , depth , avltree , owner1 , owner2 , actualowner);
			}
		}
		else
		{
			int interest;
			long long int s;
			scanf("%d %lld" , &interest , &s);
			int owner = findparent(parent , interest - 1);
			long long int gamecnt = 0;
			maxgame(s , avltree[owner]->avl_root , gamecnt);
			printf("%d %lld\n",actualowner[owner] + 1 , gamecnt);
		}
	}
}