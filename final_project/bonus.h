class moneyLend
{
public:
	moneyLend(std::string in_ID , int in_money ,long long int in_deadline)
	{
		id = in_ID;
		money = in_money;
		deadline = in_deadline;
	}
	bool operator <(const moneyLend &next) const
	{
		return this->deadline > next.deadline;
	}
	std::string id;
	int money;
	long long int deadline;
};
