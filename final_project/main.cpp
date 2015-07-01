#include <string>
#include <string.h>
#include <cstdio>

#ifdef MAP
#include "bank.h"
#endif

#ifdef UMAP
#include "bank_um.h"
#endif

#ifdef BONUS
#include "bonus.cpp"
#endif 

#define Max_L 101
using namespace std;

int main()
{
	Bank bank;
	char cmd[20];
	char ID1[Max_L], ID2[Max_L], password1[Max_L], password2[Max_L];
	int money;

#ifdef BONUS
	int money_2;
	int day;
	int stock_number;
	long long int realTime = 0 , deadline;
#endif

	while (scanf("%s" ,cmd) != EOF)
	{
		
		if (strcmp(cmd , "login") == 0) {
			scanf("%s %s", ID1, password1);
			bank.login(string(ID1), string(password1));
		} else if (strcmp(cmd , "create") == 0) {
			scanf("%s %s", ID1, password1);
			bank.create(string(ID1), string(password1));
		} else if (strcmp(cmd , "delete") == 0) {
			scanf("%s %s", ID1, password1);
			bank.deletion(string(ID1), string(password1));
		} else if (strcmp(cmd , "merge") == 0) {
			scanf("%s %s", ID1, password1);
			scanf("%s %s", ID2, password2);
			bank.merge(string(ID1), string(password1), string(ID2), string(password2));
		} else if (strcmp(cmd , "deposit") == 0) {
			scanf("%d" , &money);
			bank.deposit(money);
		} else if (strcmp(cmd , "withdraw") == 0) {
			scanf("%d" , &money);
			bank.withdraw(money);
		} else if (strcmp(cmd , "transfer") == 0) {
			scanf("%s %d" , ID2, &money);
			bank.transfer(string(ID2) , money);
		} else if (strcmp(cmd , "find") == 0) {
			scanf("%s", ID2);
			bank.find(string(ID2));
		} else if (strcmp(cmd , "search") == 0) {
			scanf("%s", ID2);
			bank.search(string(ID2));
		}
#ifdef BONUS		
// money lending system
// operation:
// borrow: you can borrow money from the bank with interest 5% (86400 iteration per circle)
// command: borrow [money] [day]
// all the operation will work automatically

		else if(strcmp(cmd,"borrow") == 0) 
		{
			scanf("%d %d",&money_2, &day);
			deadline = realTime + day * second_per_day;
			bank.borrow(money_2, deadline, realTime);
		}
#endif 
		else
		{
			printf("wrong operation!\n");
		}

#ifdef BONUS
//operating loan 
		bank.operateLoan(realTime);
		realTime += 3600; // TA can modify this line to test. Default is 3600.
		//the enhancement of realTime means seconds per iteration
		//you can also modify second_per_day in bonus.cpp t
#endif
	}
	return 0;
}