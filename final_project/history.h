#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
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

class History
{
public:
	vector< Incident* > incident_ptr;
	void insert(Incident *ptr)
	{
		incident_ptr.push_back(ptr);
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


void History::merge_history(History &merged , string &ID , string &merged_ID)
{
	vector < Incident * >::iterator it = incident_ptr.begin() , merged_it = merged.incident_ptr.begin();
	vector < Incident *> temp;

	// sort method is O(n2) + O(NlogN) . This method is O(N) + O(N)
	while(it != incident_ptr.end() && merged_it != merged.incident_ptr.end() )
	{
		if((*it)->timestamp < (*merged_it)->timestamp )
		{
			temp.push_back(*it);
			it ++;
		}
		else if((*it)->timestamp == (*merged_it)->timestamp)
		{
			if( (*merged_it)->fid == ID )
			{
				(*merged_it)->collision = 1;
				(*merged_it)->tid = ID;
			}
			else if( (*merged_it)->tid == ID )
			{
				(*merged_it)->collision = 2;
				(*merged_it)->fid = ID;
			}
			temp.push_back(*merged_it);
			it ++; merged_it ++;
		}
		else
		{
			if( (*merged_it)->fid == merged_ID)
				(*merged_it)->fid = ID;
			else if( (*merged_it)->tid == merged_ID)
				(*merged_it)->tid = ID;
			temp.push_back(*merged_it);
			merged_it ++;
		}
	}
	while(it != incident_ptr.end())
	{
		temp.push_back(*it);
		it ++;
	}
	while(merged_it != merged.incident_ptr.end())
	{
		if( (*merged_it)->fid == merged_ID)
			(*merged_it)->fid = ID;
		else if( (*merged_it)->tid == merged_ID)
			(*merged_it)->tid = ID;
		temp.push_back(*merged_it);
		merged_it ++;
	}
	incident_ptr = temp;
}
void History::search_history(string id , string target)
{
	vector < Incident * >::iterator it;
	bool record = false;
	for(it = incident_ptr.begin() ; it != incident_ptr.end() ; it++)
	{
		if((*it)->fid == target || (*it)->tid == target)
		{

			if((*it)->collision == 0 && target != id)
			{
				if(!record)
					record = true;
				if((*it)->tid == target)
					printf("To %s %d\n" , (*it)->tid.c_str() , (*it)->money);
				else if((*it)->fid == target)
					printf("From %s %d\n" , (*it)->fid.c_str() , (*it)->money);
			}
			else if((*it)->collision == 1)
			{
				if(!record)
					record = true;
				printf("To %s %d\n", (*it)->tid.c_str() , (*it)->money);
				printf("From %s %d\n" , (*it)->fid.c_str() , (*it)->money);
			}
			else if((*it)->collision == 2)
			{
				if(!record)
					record = true;
				printf("From %s %d\n", (*it)->fid.c_str() , (*it)->money);
				printf("To %s %d\n" , (*it)->tid.c_str() , (*it)->money);
			}
		}
	}
	if(!record)
		printf("no record\n");
}