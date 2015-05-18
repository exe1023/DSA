#include "binomial_heap.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
class Task
{
private:
	int id;
	int priority;
public:
	Task(){id = 0 ; priority = 0;}
	Task(int a , int b){id = a; priority = b;}
	bool operator<=(const Task &a)const
	{
		return (priority <= a.priority);
	}
};
void assign(BinomialHeap<Task> computers[] , int cm , Task task)
{
	computers[cm].insert(task);
	printf("There are %d tasks on computer %d\n" , computers[cm].heapsize() , cm );
}
void excute(BinomialHeap<Task> computers[] , int cm)
{
	
}
int main(void)
{
	int c , w;
	cin >> c >> w;
	BinomialHeap<Task> computers[c];
	char command[10];
	while(scanf("%s" , command) != EOF)
	{
		if(strcmp(command , "assign") == 0)
		{
			int cm , id , p;
			cin >> cm >> id >> p;
			Task task(id , p);
			assign(computers , cm , task);
		}
		else if(strcmp(command , "excute") == 0)
		{
			int cm;
			cin >> cm;
			excute(computers , cm);
		}
		else if(strcmp(command , "merge") == 0)
		{}
	}
}