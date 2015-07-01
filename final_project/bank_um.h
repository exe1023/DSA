#include "history_LL.h"
#include "md5.h"
#include <unordered_map>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#ifdef BONUS
#include "bonus.h"
#include <queue>
#endif

#define max_L 101
using namespace std;

class Account
{
public:
	string id;
	string password;
	int money;
	bool bankrupt;
	History history;
	Account(const string &ID , const string &md5Password)
	{
		id = ID;
		password = md5Password;
		money = 0;
		bankrupt = false;
	}
	void account_changemoney(const int &Money)
	{
		money += Money;
	}
	void account_transfer(Incident *ptr, const int &Money)
	{
		history.insert(ptr);
		money -= Money;
	}
	void account_merge(Account merged)
	{
		history.merge_history(merged.history , id , merged.id);
		money += merged.money;
	}
	void account_search(const string &target)
	{
		history.search_history(id , target);
	}
	void gobankrupt()
	{
		bankrupt = true;
	}
	bool isbankrupt()
	{
		return bankrupt ;
	}
};
class Bank
{
private:
	unordered_map<string , Account> accounts;
	unordered_map<string , Account>::iterator LoginAccount;
	Incident *akashic; // I don't know how big should the array be , and vector will result in segmentation fault;
#ifdef BONUS
	priority_queue< moneyLend > lendHistory;	 
#endif
	int Time;
	//for transfer, target 1 will make the func end earlier
	int get_score(const string &a, const string &b, bool target, const int &max_score); 
	bool check_map_unused(const string &ID);
	void score1_find(string temp ,const string &ID, string result[] , int &count);
	void best_10_unused_score1(const string &ID , string result[] , int &count);
	void best_10_unused_score2(const string &ID , string result[] , int &count);
	void score_find(string temp , const string &ID , string result[] , int &count , int try_length , int try_score);
	void score_general_solve(const string &ID , string result[] , int &count , int try_length , int try_score);
	void best_10_unused(const string &ID);
	char char_table[63];	//'0' to 'z', ends at '\0'
public:
	Bank()
	{
		Time = 0;
		akashic = new Incident[200000];
		for(int i = '0', j = 0; j < 63 ;j++)
		{
			char_table[j] = i;
			if(i < '9')
				i++;
			else if(i == '9')
				i = 'A';
			else if(i < 'Z')
				i++;
			else if(i == 'Z')
				i = 'a';
			else if(i < 'z')
				i++;
			else if(i == 'z')
				i = '\0';
		}
	}
	void create(const string &ID , const string &Password)
	{
		unordered_map<string , Account>::iterator it = accounts.find(ID);
		if(it != accounts.end())
		{
			printf("ID %s exists, " , ID.c_str());
			best_10_unused(ID);
		}
		else
		{
			Account newaccount(ID , md5(Password));
			accounts.insert( pair<string , Account>(ID , newaccount) );
			printf("success\n");
	    }
	}
	void deletion(const string &ID , const string &Password)
	{
		unordered_map<string , Account>::iterator it = accounts.find(ID);
		if(it == accounts.end())
			printf("ID %s not found\n" , ID.c_str());
		else if (it->second.password != md5(Password))
			printf("wrong password\n");
		else
		{
#ifdef BONUS
			deleteLoan(it->second.id);
#endif			
			accounts.erase(it);
			printf("success\n");
		}


	}
	void login(const string &ID , const string &Password)
	{
		unordered_map<string , Account>::iterator it;
		it = accounts.find(ID);
		if(it == accounts.end())
			printf("ID %s not found\n" , ID.c_str());
		else if(it->second.password != md5(Password))
			printf("wrong password\n");
		else
		{
			LoginAccount = it;
			printf("success\n");
		}
	}
	void deposit(int &Money)
	{
		LoginAccount->second.account_changemoney(Money);
		printf("success, %d dollars in current account\n" ,LoginAccount->second.money);
	}
	void withdraw(int &Money)
	{
		if(LoginAccount->second.money < Money)
			printf("fail, %d dollars only in current account\n" ,LoginAccount->second.money);
		else
		{
			LoginAccount->second.account_changemoney(Money * -1);
			printf("success, %d dollars left in current account\n" ,LoginAccount->second.money);
		}
	}
	void transfer(const string &ID, const int &Money)
	{
		unordered_map<string , Account>::iterator it;
		it = accounts.find(ID);
		if(it == accounts.end())
		{
			printf("ID %s not found, " , ID.c_str());
			//recommend best 10 existing ID
			vector< pair<int, string> > transfer_advice_IDs;
			unordered_map<string , Account>::iterator now;
			now = accounts.begin();
			//simply push the first ID into the vector
			int max_score = 0;
			int temp_score = get_score(ID, now->second.id, 0, max_score);
			max_score = temp_score;
			transfer_advice_IDs.push_back(make_pair(temp_score, now->second.id));
			now++;
			//cmp function for a max heap
			auto advise_cmp = [](const pair<int, string> &a, const pair<int, string> &b)
			{
				if (a.first == b.first)
					return (a.second.compare(b.second) < 0);
				else
					return (a.first < b.first);
			};
			make_heap(transfer_advice_IDs.begin(), transfer_advice_IDs.end(), advise_cmp);
			
			for(now ; now != accounts.end() ; now++)
			{
				if(transfer_advice_IDs.size() < 10) //simply push into the heap
				{
					temp_score = get_score(ID, now->second.id, 0, max_score);
					transfer_advice_IDs.push_back(make_pair(temp_score, now->second.id));
					push_heap(transfer_advice_IDs.begin(), transfer_advice_IDs.end(), advise_cmp);
					if(temp_score > max_score)
						max_score = temp_score;
				}
				else
				{
					temp_score = get_score(ID, now->second.id, 1, max_score);
					if( (temp_score < max_score) || 
					((temp_score == max_score) &&(now->second.id.compare(transfer_advice_IDs.front().second) < 0)) )
					{
						pop_heap(transfer_advice_IDs.begin(), transfer_advice_IDs.end(), advise_cmp);
						transfer_advice_IDs.pop_back();
						transfer_advice_IDs.push_back(make_pair(temp_score, now->second.id));
						push_heap(transfer_advice_IDs.begin(), transfer_advice_IDs.end(), advise_cmp);
						max_score = transfer_advice_IDs.front().first;
					}
				}
			}
			sort_heap(transfer_advice_IDs.begin(), transfer_advice_IDs.end(), advise_cmp);
			for (unsigned i=0; i<transfer_advice_IDs.size()-1; i++)
				printf("%s,",transfer_advice_IDs[i].second.c_str());
			printf("%s\n",transfer_advice_IDs.back().second.c_str());
		}
		else if(LoginAccount->second.money < Money)
			printf("fail, %d dollars only in current account\n" ,LoginAccount->second.money);
		else
		{
			Incident temp(LoginAccount->second.id , ID , 0 , Money , Time);
			akashic[Time] = temp;
			Incident *ptr = &akashic[Time];
			LoginAccount->second.account_transfer(ptr , Money);
			it->second.account_transfer(ptr , Money * -1);
			printf("success, %d dollars left in current account\n",LoginAccount->second.money);
			Time ++;
	    }
	}
	void merge(const string &ID , const string &Password , const string &merged_ID , const string &merged_Password)
	{
		unordered_map<string , Account>::iterator it , merged_it;
		if( (it = accounts.find(ID)) == accounts.end() )
			printf("ID %s not found\n" , ID.c_str());
		else if( (merged_it = accounts.find(merged_ID)) == accounts.end() )
			printf("ID %s not found\n" , merged_ID.c_str());
		else if(it->second.password != md5(Password))
			printf("wrong password1\n");
		else if(merged_it->second.password != md5(merged_Password))
			printf("wrong password2\n");
		else
		{
#ifdef BONUS			
			if(it->second.isbankrupt())
			{
				printf("accout %s is bankrupt, you can not merge\n",it->second.id.c_str());
				return;
			}
			else if(merged_it->second.isbankrupt())
			{
				printf("accout %s is bankrupt, you can not merge\n",merged_it->second.id.c_str());
				return;
			}
			else if(findLoan(merged_it->second.id))
			{
				printf("account %s has loan record, you can not merge\n",merged_it->second.id.c_str());
				return;
			}
#endif
			it->second.account_merge(merged_it->second);
			accounts.erase(merged_it);
			printf("success, %s has %d dollars\n" , ID.c_str() , it->second.money);
		}
	}
	void search(const string &ID)
	{
		LoginAccount->second.account_search(ID);
	}
	void find(const string&);
#ifdef BONUS
	int findLoan(std::string);
	void deleteLoan(std::string);
	void borrow(const int&,const long long int&,const long long int&);
	void operateLoan(const long long int);
#endif
};
void find_wildcard( char target[] , char process[] , int i , int j , bool &flag)
{
	while(process[j] != '\0' && target[i] != '\0')
	{
		if(target[i] == '*')
		{
			if(target[i + 1] == '\0')
			{
				flag = true;
				return;
			}
			else
			{
				while(process[j] != '\0')
				{
					if(process[j] == target[i + 1])
						find_wildcard(target , process , i + 1 , j  , flag);
					j++;
				}
			}
		}
		else if(target[i] == '?')
		{
			j++; i++;
		}
		else
		{
			if(target[i] == process[j])
			{
				i++; j++;
			}
			else
				return;
		}
	}
	if(process[j] == '\0' && target[i] == '\0')
		flag = true;
}

void Bank::find(const string &ID2)
{
//the function is used to emulate ids which fit restriction of ID2
	unordered_map<string , Account>::iterator now;
	vector<string> temp;
	temp.resize(0);
	char target[101];
	strcpy(target,ID2.c_str());
	char process[101];
	bool flag = false;
	for(now = accounts.begin() ; now != accounts.end() ; now++)
	{
		strcpy(process,now->second.id.c_str());
		if(now != LoginAccount)
			find_wildcard(target , process , 0 , 0 , flag);
		if(flag == true)
			temp.push_back(now->second.id);
		flag = false;
	}
	sort(temp.begin(),temp.end());
	for(vector<string>::iterator it = temp.begin(); it != temp.end() ; it++)
	{
		printf("%s",(*it).c_str());
		if( (it+1) != temp.end())
			printf(",");
		else
			printf("\n");
	}
	if(temp.begin() == temp.end())
		printf("\n");
}

int Bank::get_score(const string &a, const string &b, bool target,const int &max_score)
{
	int size_short, size_long;
	int score = 0;
	if (a.size() < b.size())
	{
		size_short = a.size();
		size_long = b.size();
	}
	else
	{
		size_short = b.size();
		size_long = a.size();
	}
	score += (size_long - size_short) * (size_long - size_short +1) / 2;
	if(target == 1 && score > max_score)
		return score;
	for (int i = 0; (i < size_short) && !(target == 1 && score > max_score); i++)
	{
		score += (a[i] != b[i]) ? (size_short-i) : 0;
	}
	return score;
}

bool Bank::check_map_unused(const string &ID)
{
	return (accounts.find(ID) == accounts.end());
}

void Bank::score1_find(string temp ,const string &ID, string result[] , int &count)
{
	temp.push_back('0');
	if(temp.length() > 100)
		return;
	for(int i = 0 ; i < 62 && count < 10; i++)
	{
		temp.back() = char_table[i];
		if(check_map_unused(temp))
		{
			result[count] = temp;
			count ++;
		}
		if(temp == ID)
		{
			score1_find(temp , ID , result , count);
		}
	}
}

void Bank::best_10_unused_score1(const string &ID , string result[] , int &count)
{
	string temp = ID;
	temp.pop_back();
	if(check_map_unused(temp) && temp.length() >= 1)
	{
		result[count] = temp;
		count ++;
	}
	score1_find(temp , ID , result , count);
	
}

void Bank::best_10_unused_score2(const string &ID , string result[] , int &count)
{
	string temp = ID;
	if(temp.length() == 1)
	{
		temp.assign("00");
		for(int j = 0; j < 62 && count < 10; j++)
		{
			if(char_table[j] == ID[0])
				continue;
			temp.front() = char_table[j];
			for(int i = 0; i < 62 && count < 10; i++)
			{
				temp.back() = char_table[i];
				if(check_map_unused(string(temp)))
				{
					result[count] = temp;
					count ++;
				}
			}
		}
	}
	else
	{
		char origin_1 = temp.back(); //the last char in ID
		temp.pop_back();
		char origin_2 = temp.back(); //the second last char in ID
		for(int i = 0; i < 62 && count < 10; i++)
		{
			temp.back() = char_table[i];
			if(char_table[i] == origin_2 && temp.length() <= 98)
			{
				temp.push_back('0');
				for(int j = 0; j < 62 && count < 10; j++)
				{
					if(char_table[j] == origin_1)
						continue;
					temp.back() = char_table[j];
					for(int k = 0; k < 62 && count < 10; k++)
					{
						temp.push_back(char_table[k]);
						if(check_map_unused(string(temp)))
						{
							result[count] = temp;
							count ++;
						}
						temp.pop_back();
					}
				}
				temp.pop_back();
			}
			else
			{
				if(check_map_unused(temp))
				{
					result[count] = temp;
					count ++;
					if(count == 10) break;
				}
				temp.push_back(origin_1);
				if(check_map_unused(temp))
				{
					result[count] = temp;
					count ++;
				}
				temp.pop_back();
			}
		}
	}
}

void Bank::score_find(string temp , const string &ID , string result[] , int &count , int try_length , int try_score)
{
	temp.push_back('0');
	if(temp.length() > 100 || (temp.length() > ID.length() + try_length))
		return;
	int score;
	if(temp.length() > ID.length())
	{
		score = get_score(temp , ID , 0 , 0);
		if(score > try_score)
			return;
		else
		{
			for(int i = 0 ; i < 62 && count < 10 ; i++)
			{
				temp.back() = char_table[i];
				if(score == try_score && check_map_unused(temp))
				{
					result[count] = temp;
					count ++;
				}
				else
					score_find(temp , ID , result , count , try_length , try_score);
			}
		}
	}
	else
	{
		if(ID[ temp.length() - 1 ] != '0')
			score = get_score(temp , ID , 0 , 0);
		else
		{
			temp.back() = '1';
			score = get_score(temp , ID , 0 , 0);
		}
		for(int i = 0 ; i < 62 && count < 10 ; i++)
		{
			temp.back() = char_table[i];
			if(ID[ temp.length() - 1] != char_table[i])
			{
				if(score == try_score && check_map_unused(temp))
				{
					result[count] = temp;
					count ++;
				}
				score_find(temp , ID , result , count , try_length , try_score);
			}
			else
			{
				if(get_score(temp , ID , 0 , 0) == try_score && check_map_unused(temp))
				{
					result[count] = temp;
					count ++;
				}
				score_find(temp , ID , result , count , try_length , try_score);
			}
		}
    }
}

void Bank::score_general_solve(const string &ID , string result[] , int &count , int try_length , int try_score)
{
	string temp = ID;
	if(temp.length() > try_score  )
		temp.erase(temp.length() - try_score  , try_score );
	else
		temp.clear();
	score_find(temp , ID , result , count , try_length , try_score);
}

void Bank::best_10_unused(const string &ID)
{
	string result[10] ;
	int count = 0 , try_score = 1 , try_length = 2;
	while(count < 10)
	{
		if(try_score == 1)
			best_10_unused_score1(ID , result , count);
		else if(try_score == 2)
			best_10_unused_score2(ID , result , count);
		else // score >= 3
		{
			if(( (try_length + 1) * (try_length + 2) / 2) <= try_score )
				try_length ++;
			score_general_solve(ID , result , count , try_length , try_score);  //general solve can not be used to solve try_score = 1;
		}
		try_score ++;
	}
	for(int i = 0 ; i < 9 ; i ++)
		printf("%s," , result[i].c_str());
	printf("%s\n" , result[9].c_str());
}
