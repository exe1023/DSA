#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <cstddef>
using namespace std;
class Incident
{
public:
	string fid;
	string tid;
	short collision; // 0 means no collision , 1 means "to" will print first , 2 means "from" will print first
	int money;
	int timestamp;
	Incident(string , string , short , int , int);
	Incident()
	{
		collision = 0;
	}
};
typedef struct node
{
	struct node *next;
	Incident *ptr;
}Incident_node;

class History
{
public:
	Incident_node *head;
	Incident_node *tail;
	bool size;
	History()
	{
		head = NULL;
		tail = NULL;
		size = false;
	}
	void insert(Incident *PTR)
	{
		Incident_node *node = new Incident_node;
		node->ptr = PTR; node->next = NULL;
		if(size == false)
		{
			head = node; tail = node; size = true;
		}
		else
		{
			tail->next = node;
			tail = tail->next;
		}
	}
	void merge_history(History& , string& ,string&);
	void search_history(string , string );
};
Incident::Incident(string fID , string tID , short Collision , int Money , int Timestamp )
{
	fid = fID;
	tid = tID;
	collision = Collision;
	money = Money;
	timestamp = Timestamp;
}


void dealwith_collision(Incident *b , string &ID)
{
	if(b->fid == ID)
	{
		b->collision = 1;
		b->tid = ID;
	}
	else if(b->tid == ID)
	{
		b->collision = 2;
		b->fid = ID;
	}
}
void dealwith_nocollision(Incident *b , string &ID , string &merged_ID)
{
	if(b->fid == merged_ID)
		b->fid = ID;
	else if(b->tid == merged_ID)
		b->tid = ID;
}
void History::merge_history(History &merged , string &ID , string &merged_ID)
{
	Incident_node *now = head , *last  , *merged_now = merged.head , *merged_last ;
	Incident_node *newlist_now , *newhead;
	if(now != NULL && merged_now != NULL)
	{
		if((now->ptr)->timestamp < (merged_now->ptr)->timestamp)
		{
			Incident_node *temp = new Incident_node;
			temp->ptr = now->ptr; temp->next = NULL;
			newhead = temp; newlist_now = temp;
			last = now; now = now->next;
			delete last;
		}
		else if((now->ptr)->timestamp == (merged_now->ptr)->timestamp)
		{
			dealwith_collision(merged_now->ptr , ID);
			Incident_node *temp = new Incident_node;
			temp->ptr = now->ptr; temp->next = NULL;
			newhead = temp; newlist_now = temp;
			last = now; now = now->next;
			merged_last = merged_now; merged_now = merged_now->next;
			delete last; delete merged_last;
		}
		else
		{
			dealwith_nocollision(merged_now->ptr , ID , merged_ID);
			Incident_node *temp = new Incident_node;
			temp->ptr = merged_now->ptr; temp->next = NULL;
			newhead = temp; newlist_now = temp;
			merged_last = merged_now; merged_now = merged_now->next;
			delete merged_last;
		}
	}
	else if(now != NULL && merged_now == NULL)
	{
		Incident_node *temp = new Incident_node;
		temp->ptr = now->ptr; temp->next = NULL;
		newhead = temp; newlist_now = temp;
		last = now; now = now->next;
		delete last;
	}
	else if(now == NULL && merged_now != NULL)
	{
		dealwith_nocollision(merged_now->ptr , ID , merged_ID);
		Incident_node *temp = new Incident_node;
		temp->ptr = merged_now->ptr; temp->next = NULL;
		newhead = temp; newlist_now = temp;
		merged_last = merged_now; merged_now = merged_now->next;
		delete merged_last;
	}
	else
	{
		return;
	}
	while(now != NULL && merged_now != NULL)
	{
		if( (now->ptr)->timestamp < (merged_now->ptr)->timestamp )
		{
			Incident_node *temp = new Incident_node;
			temp->ptr = now->ptr; temp->next = NULL;
			newlist_now->next = temp;   newlist_now = newlist_now->next;
			last = now; now = now->next;
			delete last;
		}
		else if((now->ptr)->timestamp == (merged_now->ptr)->timestamp)
		{
			dealwith_collision(merged_now->ptr , ID);
			Incident_node *temp = new Incident_node;
			temp->ptr = now->ptr; temp->next = NULL;
			newlist_now->next = temp; newlist_now = newlist_now->next;
			last = now; now = now->next;
			merged_last = merged_now; merged_now = merged_now->next;
			delete last; delete merged_last;
		}
		else
		{
			dealwith_nocollision(merged_now->ptr , ID , merged_ID);
			Incident_node *temp = new Incident_node;
			temp->ptr = merged_now->ptr; temp->next = NULL;
			newlist_now->next = temp; newlist_now = newlist_now->next;
			merged_last = merged_now; merged_now = merged_now->next;
			delete merged_last;
		}
	}
	while(now != NULL)
	{
		Incident_node *temp = new Incident_node;
		temp->ptr = now->ptr; temp->next = NULL;
		newlist_now->next = temp; newlist_now = newlist_now->next;
		last = now; now = now->next;
		delete last;
	}
	while(merged_now != NULL)
	{
		dealwith_nocollision(merged_now->ptr , ID , merged_ID);
		Incident_node *temp = new Incident_node;
		temp->ptr = merged_now->ptr; temp->next = NULL;
		newlist_now->next = temp; newlist_now = newlist_now->next;
		merged_last = merged_now; merged_now = merged_now->next;
		delete merged_last;
	}
	head = newhead;
	tail = newlist_now;
	size = true;
}
void History::search_history(string id , string target)
{

	Incident_node *now = head;
	bool record = false;
	while(now != NULL)
	{
		if((now->ptr)->fid == target || (now->ptr)->tid == target)
		{
			if((now->ptr)->collision == 0 && target != id)
			{
				record = true;
				if((now->ptr)->tid == target)
					printf("To %s %d\n" , (now->ptr)->tid.c_str() ,(now->ptr)->money );
				else
					printf("From %s %d\n" , (now->ptr)->fid.c_str() ,(now->ptr)->money );
			}
			else if((now->ptr)->collision == 1)
			{
				record = true;
				printf("To %s %d\n", (now->ptr)->tid.c_str() , (now->ptr)->money);
				printf("From %s %d\n", (now->ptr)->fid.c_str() , (now->ptr)->money);
			}
			else if((now->ptr)->collision == 2)
			{
				record = true;
				printf("From %s %d\n", (now->ptr)->fid.c_str() , (now->ptr)->money);
				printf("To %s %d\n", (now->ptr)->tid.c_str() , (now->ptr)->money);
			}
		}
		now = now->next;
	}
	if(!record)
		printf("no record\n");
}