#include <iostream>
#include <stdio.h>
#include <stack>
#include <string>
#include <ctype.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
using namespace std;
int checkprecedence(char opr)  // p means plus , m means minus ,s means sin , c means cos , e means exp , l means log , o means pow , q means sqrt , f means fabs
{
	if(opr == '(' || opr == ')' || opr == 's' || opr == 'c' || opr == 'e' || opr == 'l' || opr == 'o' || opr == 'q' || opr == 'f')
		return 1;
	else if(opr == 'p' || opr == 'm' )
		return 2;
	else if(opr == '*')
		return 3;
	else if(opr == '+' || opr == '-')
		return 4;
}
void replace(char *operan ,bool lastisdigit)
{
	if(*operan == 's' && *(operan + 1)  == 'q' && *(operan + 2) == 'r' && *(operan + 3) == 't')
		*operan = 'q';
	else if(*operan == 'p' && *(operan + 1) == 'o' && *(operan + 2) == 'w')
		*operan = 'o';
	if(!lastisdigit)
	{
		if(*operan == '+')
			*operan = 'p';
		else if(*operan == '-')
			*operan = 'm';
	}
}
void pushfloat(string &line , int &i , vector<char> &output)
{
	while(isdigit(line[i]))
	{
		output.push_back(line[i]);
		i++;
	}
	if(line[i] == '.')
	{
		output.push_back(line[i]);
		i++;
		while(isdigit(line[i]))
		{
			output.push_back(line[i]);
			i++;
		}
	}
	i--;
    output.push_back(' ');
}
void clearparren(stack<char> &operan , vector<char> &output)
{
	stack<char> temp;
	while(operan.top() != '(')
	{
		output.push_back(operan.top());
		output.push_back(' ');
		operan.pop();
	}
	operan.pop();
	if( !operan.empty() &&(operan.top() == 's' || operan.top() == 'c' || operan.top() == 'e' || operan.top() == 'l' || operan.top() == 'o' || operan.top() == 'q' || operan.top() == 'f' ))
	{
		output.push_back(operan.top());
		output.push_back(' ');
		operan.pop();
	}
}
void clearoperan(stack<char> &operan , vector<char> &output)
{
	while(!operan.empty())
	{
		output.push_back(operan.top());
		output.push_back(' ');
		operan.pop();
	}
}
void printstackfloat(vector<char>::iterator &it)
{
	while(isdigit(*it))
	{
		printf("%c" , *it);
		it++;
	}
	if(*it == '.')
	{
		printf("%c" , *it);
		it ++;
		for(int i = 0 ; i < 6 ; i++)
		{
			if(isdigit(*it))
			{
				printf("%c" , *it);
				it ++;
			}
			else
				printf("0");
		}
		while(isdigit(*it))
			it++;
	}
	else
	{
		printf(".000000");
	}
	it --;
}
void printoutput(vector<char> &output)
{
	vector<char>::iterator it = output.begin() , end = output.end();
	for(it ; it != end - 1 ; it++)
	{
		if(*it == 's')
			printf("sin");
		else if(*it == 'c')
			printf("cos");
		else if(*it == 'e')
			printf("exp");
		else if(*it == 'l')
			printf("log");
		else if(*it == 'o')
			printf("pow");
		else if(*it == 'q')
			printf("sqrt");
		else if(*it == 'f')
			printf("fabs");
		else if(*it == 'p' )
			printf("+");
		else if(*it == 'm')
			printf("-");
		else if(isdigit(*it))
			printstackfloat(it);
		else
			printf("%c" , *it);
	}
	printf("\n");
}
void outprecedence(char compare , stack<char> &operan , vector<char> &output)
{
	if(checkprecedence(compare) == 2 && checkprecedence(operan.top()) == 2)
		return;
	while(!operan.empty() && checkprecedence(compare) >= checkprecedence(operan.top()) && operan.top() != '(')
	{
		output.push_back(operan.top());
		output.push_back(' ');
		operan.pop();
	}
	return;
}
double chartodouble(vector<char>::iterator &it)
{
	string number;
	while(isdigit(*it))
	{
		number.push_back(*it);
		it ++;
	}
	if(*it == '.')
	{
		number.push_back(*it);
		it ++;
		while(isdigit(*it))
		{
			number.push_back(*it);
			it++;
		}
	}
	it --;
	//printf("%.6lf\n" , atof(number.c_str()) );
	return atof(number.c_str());
}
double docompute(vector<char> &output)
{
	stack<double> waitnumber;
	vector<char>::iterator it = output.begin() , end = output.end();
	double firstnumber , secondnumber , resultnumber;
	for(it ; it != end ; it++)
	{
		//printf("encounter %c %d\n" , *it , waitnumber.size());
		if(*it == ' ')
			continue;
		if(isdigit(*it))
			waitnumber.push(chartodouble(it));
		else
		{
			firstnumber = waitnumber.top();
			//printf("deal with %c %lf\n" ,*it  , firstnumber);
			waitnumber.pop();
			if(*it ==  'p')
				resultnumber = +firstnumber;
			else if(*it == 'm')
				resultnumber = -firstnumber;
			else if(*it == 's')
				resultnumber = sin(firstnumber);
			else if(*it == 'c')
				resultnumber = cos(firstnumber);
			else if(*it == 'e')
				resultnumber = exp(firstnumber);
			else if(*it == 'l')
				resultnumber = log(firstnumber);
			else if(*it == 'q')
				resultnumber = sqrt(firstnumber);
			else if(*it == 'f')
				resultnumber = fabs(firstnumber);
			else
			{
				secondnumber = waitnumber.top();
				waitnumber.pop();
				if(*it == 'o')
					resultnumber = pow(secondnumber , firstnumber);
				else if(*it == '+')
					resultnumber = secondnumber + firstnumber;
				else if(*it == '-')
					resultnumber = secondnumber - firstnumber;
				else if(*it == '*')
					resultnumber = secondnumber * firstnumber;
			}
			//printf("%lf\n" , resultnumber);
			waitnumber.push(resultnumber);
		}
	}
	return waitnumber.top();
}
void printstack(stack<char> operan)
{
	stack<char> temp;
	while(!operan.empty())
	{
		temp.push(operan.top());
		operan.pop();
	}
	while(!temp.empty())
	{
		printf("%c" , temp.top());
		temp.pop();
	}
	printf("\n");
}

int main(void)
{
	string line;
	while(getline(cin , line))
	{
		stack<char> operan;
	    vector<char> output;
		int len = line.length();
		bool lastisdigit = false ;
		for(int i = 0 ; i < len ; i++)
		{
			//printf("deal with %c\n" , line[i]);
			replace(&line[i], lastisdigit);
			if(line[i] == ' ' )
				continue;
			else if(isdigit(line[i]))
			{
				pushfloat(line , i , output);
				lastisdigit = true;
			}
			else if(line[i] == '.')
			{
				output.push_back('0');
				pushfloat(line , i , output);
				lastisdigit = true;
			}
			else if(line[i] == ')')
			{
				clearparren(operan , output);
				lastisdigit = true;
			}
			else if(line[i] == ',')
			{
				while(operan.top() != '(')
				{
					output.push_back(operan.top());
					output.push_back(' ');
					operan.pop();
				}
				lastisdigit = false;
			}
			else
			{
				printf("push %c to stack\n" , line[i]);
				if(line[i] == 's' || line[i] == 'c' || line[i] == 'e' || line[i] == 'l' || line[i] == 'o' )
				{
					line[i + 2] = line[i];
					i += 2;
				}
				else if(line[i] == 'q' || line[i] == 'f')
				{
					line[i + 3] = line[i];
					i += 3;
				}
				if(!operan.empty() && line[i] != '(' )
				{
					outprecedence(line[i] , operan , output);
					operan.push(line[i]);
				}
				else if(operan.empty() && line[i] != '(')
					operan.push(line[i]);
				else if(line[i] == '(')
					operan.push(line[i]);
				lastisdigit = false;
			}
			if(!output.empty())
			{
				printf("after dealing with %c current postfix: " , line[i]);
				printoutput(output);
			}
		}
		clearoperan(operan , output);
		printf("Postfix Exp: ");
		printoutput(output);
		printf("RESULT: %.6lf\n" ,docompute(output) );
	}
}