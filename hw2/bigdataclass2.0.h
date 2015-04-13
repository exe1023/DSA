#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <algorithm>
#include <time.h>
#include <vector>
#define maxuser 24000000
typedef unsigned long long int ulli;
using namespace std;

struct Propertys
	{
		ulli displayer;
	    int advertiser;
	    int keyword;
	    int title;
	    int description;
    };
class AdData{
public:
	int click;
	int position;
	int depth;
	int impress;
	struct Propertys propertys;
	AdData(int &nclick , int &nposition , int &ndepth , int &nimpress , ulli ndisplayer , int &nadvertiser , int &nkeyword , int &ntitle , int &ndescription);
};
class Ctl{
public:
	void adddata(int &click , int &impress);
	double countctl();
private:
	int totalclick = 0;
	int totalimpress = 0;
};

class Userdata{
public:
	map<int , multimap<int , AdData>> admap;
	map<int , Ctl> ctlmap;
};
