#define interest_now 0.005
#define second_per_day 86400

void Bank::borrow(const int &money ,const long long int &deadline,const long long int &realTime)
{
	if(LoginAccount->second.isbankrupt())
	{
		printf("your account is already bankrupt,you cannot have a loan anymore!\n");
	}
	else
	{
		int money_with_interest = money + money*((deadline - realTime)/second_per_day)*interest_now;
		lendHistory.push(moneyLend(LoginAccount->second.id,money_with_interest,deadline));
		LoginAccount->second.account_changemoney(money);
		printf("you borrow %d dollars from the bank\n",money);
	}
}
int Bank::findLoan(std::string findID)
{
//very very slow.....and with bad memory management
	int result = 0;
	priority_queue< moneyLend > alternative;
	while( lendHistory.size() != 0 )
	{
		moneyLend temp = lendHistory.top();
		if(temp.id == findID)
			result = 1;
		alternative.push(temp);
		lendHistory.pop();
	}
	lendHistory = alternative;
	return result;
}
void Bank::deleteLoan(std::string findID)
{
	priority_queue< moneyLend > alternative;
	while( lendHistory.size() != 0 )
	{
		moneyLend temp = lendHistory.top();
		if(temp.id != findID)
			alternative.push(temp);
		lendHistory.pop();
	}
	lendHistory = alternative;
}
void Bank::operateLoan(const long long int realTime)
{
	if(!lendHistory.empty())
	{
		moneyLend now_min = lendHistory.top();
		if(now_min.deadline == realTime) 
		{

			unordered_map<std::string , Account>::iterator it = accounts.find(now_min.id);
			if(it != accounts.end())
			{
				if( it->second.money < now_min.money)
				{
					printf("account %s 's loan is due,there is no enough money in this account to pat the loan.\n",it->second.id.c_str());
					it->second.money = 0;
					it->second.gobankrupt();
					printf("account %s is bankrupt!\n",it->second.id.c_str());
				}
				else
				{
					it->second.account_changemoney(-1 * now_min.money);
					printf("account %s 's loan is due,the rest money in the account is %d\n",it->second.id.c_str(),it->second.money);
				}
				lendHistory.pop();
			}
			else
				printf("Wow , ID:%s who borrowed the money may be suicided\n" , now_min.id.c_str());
		}
	}
}