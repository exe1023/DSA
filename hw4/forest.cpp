#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <utility>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;
class Example
{
public:
	int decision;
	double feature[1500];
};
class Best
{
public:
	double threshold;
	double totconf;
	int feapos;
	pair<int , int> leftdecision;
	pair<int , int> rightdecision;
	Best(double a , double b , int c , pair<int,int> d , pair<int,int> e)
	{
		threshold = a; totconf = b; feapos = c; leftdecision = d; rightdecision = e;
	}
	void update(double a , double b , int c , pair<int,int> d , pair<int,int> e)
	{
		threshold = a; totconf = b; feapos = c; leftdecision = d; rightdecision = e;
	}
};
//use evil global variables
vector<Example> examples;
int controller = -1;
bool compare(int case1 , int case2)
{
	return (examples[case1].feature[controller] < examples[case2].feature[controller]);
}
//read the file
Example dealwithexpress(const char *ptr , int &totaltrue , int &totalfalse , int existfeature[])
{
	Example example;
	for(int i = 0 ; i < 1500 ; i++)
		example.feature[i] = 0;
	char *expression = (char*) malloc(strlen(ptr) + 1);
	strcpy(expression , ptr);
	char *express = strtok(expression , " ");
	example.decision = atoi(express);
	if(atoi(express) == 1)
		totaltrue ++;
	else
		totalfalse ++;
	express = strtok(NULL , " :");
	while(express != NULL)
	{
		int position = atoi(express);
		express = strtok(NULL , " :");
		example.feature[position - 1] = atof(express);
		existfeature[position - 1] = 1;
		express = strtok(NULL , " :");
	}
	free(expression);
	return example;
}
//build decision tree
double confusion(int Y , int N)
{
	return (1 - pow ( (double)Y / (Y + N) , 2)  - pow((double) N / (Y + N) , 2) );
}
double totalconfusion(int c , int e , int d , int f)
{
	return ( (double)(c + d )/(c + d + e + f )*confusion(c , d) + (double)(e + f)/(c + d + e + f)*confusion(e , f)   );
}
void findthreshold(int feapos , int totaltrue , int totalfalse ,Best &best , int existfeature[] , vector<int> cases)
{
	int casesize = cases.size();
	controller = feapos;
	sort(cases.begin() , cases.end() , compare);
	pair<int , int> leftdecision , rightdecision;
	leftdecision.first = 0; leftdecision.second = 0;
	rightdecision.first = totaltrue ; rightdecision.second = totalfalse;
	for(int i = 0 ; i < casesize ; i++)
	{
		if(i >= 1 && examples[ cases[i - 1] ].feature[feapos] != examples[ cases[i] ].feature[feapos] )
		{
			//cout << (examples[ cases[i - 1] ].feature[feapos] + examples[ cases[i] ].feature[feapos] ) /2 << endl ;
			if(totalconfusion(leftdecision.first , rightdecision.first ,leftdecision.second , rightdecision.second) < best.totconf || best.totconf == -1)
			{
				best.update( (examples[ cases[i - 1] ].feature[feapos] + examples[ cases[i] ].feature[feapos] ) /2 ,
					        totalconfusion(leftdecision.first , rightdecision.first ,leftdecision.second , rightdecision.second) ,
					        feapos ,leftdecision , rightdecision);
			}
		}
		if(examples[cases[i]].decision == 1)
		{
			leftdecision.first ++; rightdecision.first --;
		}
		else
		{
			leftdecision.second ++; rightdecision.second -- ;
		}
	}
}
void printtab(int tabs)
{
	for(int i = 0 ; i < tabs ; i ++)
		printf("\t");
}
void buildtree( vector<int> &cases ,int totaltrue , int totalfalse , int existfeature[] , int tabs)
{
	int casesize = cases.size();
	if(totaltrue == 0 && totalfalse != 0)
	{
		printtab(tabs);
		printf("F++;\n"  );
		return;
	}
	if(totaltrue != 0 && totalfalse == 0)
	{
		printtab(tabs);
		printf("T++;\n"  );
		return;
	}
	Best best(-1 , -1 , -1 , make_pair(0 , 0) , make_pair(0 , 0));
	for(int i = 0 ; i < 1500 ; i++)
	{
		if(existfeature[i] == 1)
		{
			findthreshold(i ,totaltrue , totalfalse , best , existfeature , cases);
		}
	}
	if(best.feapos == -1)
	{
		printtab(tabs);
		printf("%c++;\n", ((totaltrue > totalfalse) ? 'T' : 'F') );
		return;
	}
	vector<int> newcase1 , newcase2;
	for(int i = 0 ; i < casesize ; i++)
	{
		if(examples[cases[i]].feature[ best.feapos ] > best.threshold)
			newcase1.push_back(cases[i]);
		else
			newcase2.push_back(cases[i]);
	}
	printtab(tabs);
	printf("if(attr[%d] > %lf){\n" ,best.feapos + 1, best.threshold);
	buildtree(newcase1 , best.rightdecision.first , best.rightdecision.second , existfeature , tabs+1);
	printtab(tabs);
	printf("}\n");
	printtab(tabs);
	printf("else{\n");
	buildtree(newcase2 , best.leftdecision.first , best.leftdecision.second , existfeature , tabs+1);
	printtab(tabs);
	printf("}\n");
}
void knuthshuffle(vector<int> &cases)
{
	int size = cases.size();
	for(int i = size - 1 ; i >= 1 ; i--)
	{
		int randnum = rand() % i + 1;
		swap(cases[i] , cases[randnum]);
	}
}

void buildforest(int treenumber , vector<int> &cases , int totaltrue , int totalfalse , int existfeature[] , int tabs)
{
	int size = cases.size();
	printf("int T = 0 , F = 0;\n");
	for(int i = 0 ; i < treenumber ; i++)
	{
		printf("tree%d_predict:\n" , i + 1);
		knuthshuffle(cases);
		vector<int> randcase(cases.begin() , cases.begin() + (size / 3) );
		buildtree(randcase , totaltrue , totalfalse , existfeature , tabs );
	}
	printf("voting:\n");
	printf("\tif(T > F) return 1;\n");
	printf("\telse return -1;\n");
}
int main(int argc , char *argv[])
{
	ifstream fin;
	fin.open(argv[1]);
	int treenumber = atof(argv[2]);
	string expression;
	vector<int> cases;
	srand(time(NULL));
	int totaltrue = 0  , totalfalse = 0 , count = 0 , existfeature[1500] = {0};
	int tabs = 1;
	while(getline(fin , expression))
	{
		examples.push_back( dealwithexpress(expression.c_str(),  totaltrue , totalfalse , existfeature) );
		cases.push_back(count);
		count ++;
	}
	printf("int forest_predict(double *attr){\n");
	buildforest(treenumber , cases  , totaltrue , totalfalse , existfeature , tabs);
	printf("}\n");
}