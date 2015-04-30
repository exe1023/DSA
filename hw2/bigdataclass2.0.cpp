#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <set>
#include <time.h>
using namespace std;
class AdData{
public:
	int click;
	int position;
	int depth;
	int impress;
	string displayer;
	int advertiser;
	int keyword;
	int title;
	int description;
	AdData(int &nclick , int &nposition , int &ndepth , int &nimpress , char ndisplayer[] , int &nadvertiser , int &nkeyword , int &ntitle , int &ndescription);
};

class Adidquery{
public:
	int adid;
	int query;
	Adidquery(int &nadid , int &nquery);
	bool operator<(const Adidquery&) const;
};
