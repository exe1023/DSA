#include <iostream>
#include <stdio.h>
#include <stack>
#include <string>
#include <ctype.h>
#include <vector>
#include <stdlib.h>
using namespace std;
int checkprecedence(char opr)  // p means plus , m means minus , d means && , t means || , > means >> , < means <<
{
	if(opr == '(' || opr == ')')
		return 1;
	else if(opr == 'p' || opr == 'm' || opr == '!' || opr == '~')
		return 2;
	else if(opr == '*' || opr == '/' || opr == '%' )
		return 3;
	else if(opr == '+' || opr == '-')
		return 4;
	else if(opr == '>' || opr == '<')
		return 5;
	else if(opr == '&' )
		return 6;
	else if(opr == '^')
		return 7;
	else if(opr == '|')
		return 8;
	else if(opr == 'd')
		return 9;
	else if(opr == 't')
		return 10;
}
void replace(char *operan ,bool lastisdigit)
{
	if(*operan == '&' && *(operan + 1) == '&' )
	{
		*operan = 'd';
		*(operan + 1) = 'd';
	}
	else if(*operan == '|' && *(operan + 1) == '|')
	{
		*operan = 't';
		*(operan + 1) = 't';
	}
	if(!lastisdigit)
	{
		if(*operan == '+')
			*operan = 'p';
		else if(*operan == '-')
			*operan = 'm';
	}
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

void printoutput(vector<char> &output)
{
	vector<char>::iterator it = output.begin() , end = output.end();
	for(it ; it != end - 1 ; it++)
	{
		if(*it == '>' || *it == '<' )
			printf("%c%c" ,*it , *it );
		else if(*it == 'd')
			printf("&&");
		else if(*it == 't')
			printf("||");
		else if(*it == 'p' )
			printf("+");
		else if(*it == 'm')
			printf("-");
		else
			printf("%c" , *it);
	}
	printf("\n");
}

int chartoint(vector<char>::iterator &it)
{
	string number;
	while(isdigit(*it))
	{
		number.push_back(*it);
		it ++;
	}
	it --;
	return atoi(number.c_str());
}
int docompute(vector<char> &output)
{
	stack<int> waitnumber;
	vector<char>::iterator it = output.begin() , end = output.end();
	int firstnumber , secondnumber , resultnumber;
	for(it ; it != end ; it++)
	{
		if(*it == ' ')
			continue;
		if(isdigit(*it))
			waitnumber.push(chartoint(it));
		else
		{
			firstnumber = waitnumber.top();
			waitnumber.pop();
			if(*it ==  'p')
				resultnumber = +firstnumber;
			else if(*it == 'm')
				resultnumber = -firstnumber;
			else if(*it == '!')
				resultnumber = !firstnumber;
			else if(*it == '~')
				resultnumber = ~firstnumber;
			else
			{
				secondnumber = waitnumber.top();
				waitnumber.pop();
				if(*it == '*')
					resultnumber = secondnumber * firstnumber;
				else if(*it == '/')
					resultnumber = secondnumber / firstnumber;
				else if(*it == '%')
					resultnumber = secondnumber % firstnumber;
				else if(*it == '+')
					resultnumber = secondnumber + firstnumber;
				else if(*it == '-')
					resultnumber = secondnumber - firstnumber;
				else if(*it == '>')
						resultnumber = secondnumber >> firstnumber;
				else if(*it == '<')
						resultnumber = secondnumber << firstnumber;
				else if(*it == '&')
					resultnumber = secondnumber & firstnumber;
				else if(*it == '^')
					resultnumber = secondnumber ^ firstnumber;
				else if(*it == '|')
					resultnumber = secondnumber | firstnumber;
				else if(*it == 'd')
					resultnumber = secondnumber && firstnumber;
				else if(*it == 't')
					resultnumber = secondnumber || firstnumber;
			}
			waitnumber.push(resultnumber);
		}
	}
	return waitnumber.top();
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
			if(line[i] == ' ')
				continue;
			else if(isdigit(line[i]))
			{
				while(isdigit(line[i]))
				{
					output.push_back(line[i]);
					i++;
				}
				i--;
				output.push_back(' ');
				lastisdigit = true;
			}
			else if(line[i] == ')')
			{
				clearparren(operan , output);
				lastisdigit = true;
			}
			else
			{
				printf("push %c to stack\n" , line[i]);
				if(line[i] == '>' || line[i] == '<' || line[i] == 'd' || line[i] == 't')
					i++;
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
				printf("after dealing with %c current postfix : " , line[i]);
				printoutput(output);
			}
		}
		clearoperan(operan , output);
		printf("Postfix Exp: ");
		printoutput(output);
		printf("RESULT: %d\n" ,docompute(output) );
	}

}