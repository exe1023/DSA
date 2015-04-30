#include "bigdataclass2.0.h"
AdData::AdData(int &nclick , int &nposition , int &ndepth , int &nimpress , ulli ndisplayer , int &nadvertiser , int &nkeyword , int &ntitle , int &ndescription)
{
	click = nclick;
	position = nposition;
	depth = ndepth;
	impress = nimpress;
	propertys.displayer = ndisplayer;
	propertys.advertiser = nadvertiser;
	propertys.keyword = nkeyword;
	propertys.title = ntitle;
	propertys.description = ndescription;
}
void Ctl::adddata(int &click , int &impress)
{
	totalclick += click;
	totalimpress += impress;
}

void get(int &adid , int &query , int &position , int &depth , Userdata &targetuser)
{
	int clicksum = 0 , impresssum = 0;
	map<int , multimap<int , AdData>>::iterator adit = targetuser.admap.find(adid);
	if(adit != targetuser.admap.end())
	{
		pair<multimap<int , AdData>::iterator , multimap<int , AdData>::iterator> range;
		range = adit->second.equal_range(query);
		multimap<int , AdData>::iterator it = range.first , upperbound = range.second;
	    for(it ; it != upperbound ; it++)
	    {
		    if(it->second.position == position && it->second.depth == depth)
		    {
			    clicksum += it->second.click;
			    impresssum += it->second.impress;
		    }
	    }
    }
	printf("%d %d\n",clicksum , impresssum);
}
void clicked(Userdata &targetuser)
{
	map<int ,multimap<int , AdData>>::iterator adit = targetuser.admap.begin() , endpoint = targetuser.admap.end();
	int lastquery = -1;
	for(adit ; adit != endpoint ; adit++)
	{
		multimap<int , AdData>::iterator queryit = adit->second.begin() , queryend = adit->second.end();
		for(queryit ; queryit != queryend ; queryit++)
		{
			if(queryit->second.click >= 1 && queryit->first != lastquery)
			{
				printf("%d %d\n" ,adit->first , queryit->first);
				lastquery = queryit->first;
			}
		}
		lastquery = -1;
	}
}
bool propertycmp(const Propertys &data1 , const Propertys &data2)
{
	if(data1.displayer < data2.displayer)
		return true;
	else if(data1.displayer > data2.displayer)
		return false;
	else if(data1.advertiser < data2.advertiser)
		return true;
	else if(data1.advertiser > data2.advertiser)
		return false;
	else if(data1.keyword < data2.keyword)
		return true;
	else if(data1.keyword > data2.keyword)
		return false;
	else if(data1.title < data2.title)
		return true;
	else if(data1.title > data2.title)
		return false;
	else if(data1.description < data2.description)
		return true;
	else if(data1.description > data2.description)
		return false;
	else
		return false;
}
bool equalproperty(Propertys &data1 , Propertys &data2)
{
	return (data1.displayer == data2.displayer && data1.advertiser == data2.advertiser
		&& data1.keyword == data2.keyword && data1.title == data2.title && data1.description == data2.description);
}
void copyproperty(Propertys &lastprint , Propertys &data)
{
	lastprint.displayer = data.displayer;
	lastprint.advertiser = data.advertiser;
	lastprint.keyword = data.keyword;
	lastprint.title = data.title;
	lastprint.description = data.description;
}
void checkandprint(vector<Propertys>::iterator data , Propertys *lastprint)
{
	if(!equalproperty(*lastprint , *data))
	{
		printf("\t%llu %d %d %d %d\n" , data->displayer , data->advertiser , data->keyword , data->title , data->description);
		copyproperty(*lastprint , *data);
	}
}

void printproperty(multimap<int , AdData> &data1 , multimap<int , AdData> &data2)
{
	vector<Propertys> property1 , property2;
	property1.reserve(512);
	property2.reserve(512);
	multimap<int , AdData>::iterator it1 = data1.begin() , it2 = data2.begin() , endit1 = data1.end() , endit2 = data2.end();
	for(it1 ; it1 != endit1 ; it1++)
		property1.push_back(it1->second.propertys);
	for(it2 ; it2 != endit2 ; it2++)
		property2.push_back(it2->second.propertys);
	sort(property1.begin() , property1.end() , propertycmp);
	sort(property2.begin() , property2.end() , propertycmp);
	vector<Propertys>::iterator vecit1 = property1.begin() , vecit2 = property2.begin() , vecend1 = property1.end() , vecend2 = property2.end();
	Propertys lastprint;
	lastprint.displayer = -1; lastprint.advertiser = -1; lastprint.keyword = -1; lastprint.title = -1; lastprint.description = -1;
	while(vecit1 != vecend1 && vecit2 != vecend2)
	{
		if(propertycmp(*vecit1 , *vecit2))
		{
			checkandprint(vecit1 , &lastprint);
			vecit1 ++;
		}
		else if(propertycmp(*vecit2 , *vecit1))
		{
			checkandprint(vecit2 , &lastprint);
			vecit2 ++;
		}
		else
		{
			checkandprint(vecit1 , &lastprint);
			vecit1++;
			vecit2++;
		}
	}
	if(vecit1 != vecend1)
	{
		for(vecit1 ; vecit1 != vecend1 ; vecit1 ++)
			checkandprint(vecit1 , &lastprint);
	}
	if(vecit2 != vecend2)
	{
		for(vecit2 ; vecit2 != vecend2 ; vecit2 ++)
			checkandprint(vecit2 , &lastprint);
	}

}

void impressed(Userdata &user1 , Userdata &user2)
{
	map<int , multimap<int , AdData>>::iterator adit1 = user1.admap.begin() , adit2 = user2.admap.begin();
	map<int , multimap<int , AdData>>::iterator endit1 = user1.admap.end() , endit2 = user2.admap.end();
	while(adit1 != endit1 && adit2 != endit2)
	{
		if(adit1->first < adit2->first)
			adit1++;
		else if(adit1->first > adit2->first)
			adit2++;
		else
		{
			printf("%d\n" , adit1->first);
			printproperty(adit1->second , adit2->second);
			adit1++;
			adit2++;
		}
	}
}
double Ctl::countctl()
{
	if(totalclick == 0 && totalimpress == 0)
		return 0;
	else
		return (double)totalclick / (double)totalimpress;
}
void profit(int adid , double theta , Userdata *users)
{
	map<int , Ctl> ::iterator ctlit;
	for(int i = 0 ; i < maxuser ; i++)
	{
		ctlit = users[i].ctlmap.find(adid);
		if(ctlit != users[i].ctlmap.end())
		{
			if(ctlit->second.countctl() >= theta)
				printf("%d\n" , i);
		}
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	fp = fopen(argv[1] , "r");
	Userdata *users = new Userdata[maxuser];
	//clock_t t = clock();
	int adid ,click , impress , advertiser , depth , position , query , keyword , title , description , user;
	ulli displayer;
	//int count = 0;
	//cout << "start reading" << endl;
	while(fscanf(fp,"%d %d %llu %d %d %d %d %d %d %d %d %d",&click ,&impress , &displayer , &adid ,
		&advertiser , &depth , &position , &query , &keyword , &title , &description , &user)!= EOF)
	{
		//count ++;
		//if(count % 1000000 == 0)
			//cout << count << endl;
		AdData newdata(click , position , depth , impress , displayer , advertiser , keyword , title , description);
		users[user].admap[adid].insert(pair<int,AdData>(query ,newdata ) );
		users[user].ctlmap[adid].adddata(click , impress);
	}
	fclose(fp);
	//t = clock() - t;
	//cout << "reading time:" << ((float)t/CLOCKS_PER_SEC) << endl;
	string command;
	cin >> command;
	while(command != "quit")
	{
		printf("********************\n");
		if(command == "get")
		{
			scanf("%d %d %d %d %d" , &user , &adid ,&query , &position , &depth );
			get(adid , query , position , depth , users[user]);
		}
		else if(command == "clicked")
		{
			scanf("%d" , &user);
			clicked(users[user]);
		}
		else if(command == "impressed")
		{
			int user1 , user2;
			scanf("%d %d",&user1 , &user2);
			impressed(users[user1] , users[user2]);
		}
		else if(command == "profit")
		{
			double theta;
			scanf("%d %lf" , &adid , &theta  );
			profit(adid , theta , users);
		}
	    printf("********************\n");
	    cin >> command;
	}

}