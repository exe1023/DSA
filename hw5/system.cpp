#include "binomial_heap.h"
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
class Task
{
private:
	int id;
	int priority;
public:
	Task(){id = 0 ; priority = 0;}
	Task(int a , int b){id = a; priority = b;}
	int taskid(){return id;}
	int taskp(){return priority;}
	bool operator<=(const Task &a)const
	{
		return (priority <= a.priority);
	}
	bool operator==(const Task &a)const
	{
		return (priority == a.priority);
	}
	bool operator>(const Task &a)const
	{
		return (priority > a.priority);
	}
};
bool idcmp(Task a , Task b)
{
	return (a.taskid() < b.taskid());
}
void assign(BinomialHeap<Task> computers[] , int cm , Task task)
{
	computers[cm].insert(task);
	printf("There are %d tasks on computer %d.\n" , computers[cm].heapsize() , cm );
}
void execute(BinomialHeap<Task> computers[] , int cm)
{
	vector<Task> excuted;
	excuted.push_back(computers[cm].pop());
	if(computers[cm].heapsize() > 0)
	{
		Task temp = computers[cm].pop();
		while(computers[cm].heapsize() > 0 && temp == excuted[0])
		{
			excuted.push_back(temp);
			temp = computers[cm].pop();
		}
		if(computers[cm].heapsize() == 0 && temp == excuted[0])
			excuted.push_back(temp);
		else
			computers[cm].insert(temp);
	}
	sort(excuted.begin() , excuted.end() , idcmp);
	for(vector<Task>::iterator it = excuted.begin() ; it != excuted.end() ; it++ )
	{
		printf("Computer %d executed task %d.\n" , cm , (*it).taskid());
	}
}
void merge(BinomialHeap<Task> computers[] , int cm1 , int cm2 , int w)
{
	if(computers[cm2].heapsize() >= w)
	{
		computers[cm1].merge(computers[cm2]);
		computers[cm2].reset();
		printf("The largest priority number is now %d on %d.\n" , (computers[cm1].max()).taskp() , cm1);
	}
	else
		printf("Merging request failed.\n");
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
		else if(strcmp(command , "execute") == 0)
		{
			int cm;
			cin >> cm;
			execute(computers , cm);
		}
		else if(strcmp(command , "merge") == 0)
		{
			int cm1 , cm2;
			cin >> cm1 >> cm2;
			merge(computers , cm1 , cm2 , w);
		}
	}
}