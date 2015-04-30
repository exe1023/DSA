#include <iostream>
#include <string>
int min(int &a , int &b)
{
	if(a > b)
		return b;
	else if(b > a)
		return a;
}
int max(int &a , int &b)
{
	if(a > b)
		return a;
    else if(b > a)
		return b;
}
void swap(int &a , int &b)
{
	int temp = a;
	a = b;
	b = temp;
}
void print(int a , int b , std::string name ,int answer ,int &count)
{
	std::cout << "Case (" << a << ", " << b <<"): " << name << " = " << answer <<", taking " << count <<" iterations\n";
    count = 0;
}
int gcd_reverse(int a , int b , int &count)
{
	for(int i = min(a , b) ; i >= 1 ; i-- , count ++)
	{
		if(a % i == 0 && b % i == 0)
		{
			return i;
		}
	}
}
int gcd_filter(int a , int b , int &count)
{
	for(int i = 2 ; i <= min(a , b) ; i++ )
	{
		count ++;
		if(a % i == 0 && b % i == 0)
			return  i * gcd_filter(a / i , b / i , count);
	}
	return 1;
}

int gcd_filterFast(int a , int b , int s , int &count)
{
	for(int i = s ; i <= min(a , b) ; i++)
	{
		count ++;
		if(a % i == 0 && b % i == 0)
			return i * gcd_filterFast(a/i , b/i , i , count);
	}
	return 1;
}

int gcd_binary(int a , int b , int &count)
{
	int n = min(a , b) , m = max(a,b) , ans = 1;
	while(n != 0 && m != 0)
	{
		count ++;
		if(n % 2 == 0 && m % 2 == 0)
		{
			ans *= 2;
			n /= 2;
			m /= 2;
		}
		else if(n % 2 == 0 && m % 2 != 0)
			n /= 2;
		else if(n % 2 != 0 && m % 2 == 0)
			m /= 2;
		if(n > m)
			swap(n , m);
		m = m - n;
	}
	return  n * ans;
}

int gcd_euclid(int a , int b , int &count)
{
	int n = min(a , b) , m = max(a , b);
	while(m % n != 0)
	{
		count ++;
		int tmp = n;
		n = m % n;
		m = tmp;
	}
	return n;
}

int main(void)
{
	int a = 11260, b = 52000 , count = 0;
	int count_reverse = 0, count_filter = 0, count_filterFast = 0, count_binary = 0, count_euclid = 0;
	while(b <= 54260)
	{
		count ++;
		gcd_reverse(a , b , count_reverse);
        gcd_filter(a , b , count_filter);
        gcd_filterFast(a , b , 2 , count_filterFast);
        gcd_binary(a , b , count_binary);
        gcd_euclid(a , b , count_euclid);
        b ++;
	}
	std::cout << "reverse : " << count_reverse / count << "\n";
	std::cout << "filter : " << count_filter / count << "\n";
	std::cout << "filterfast : " << count_filterFast / count << "\n";
	std::cout << "binary : " << count_binary / count << "\n";
	std::cout << "euclid : " << count_euclid / count << "\n";
}