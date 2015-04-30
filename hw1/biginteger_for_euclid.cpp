#include "biginteger_for_euclid.h"
using namespace std;
// about initialize
BigInteger::BigInteger()
{
  for(int i = 0 ; i < 256 ; i++)
    number[i] = 0;
  length = 0;
}
void numinitial(BigInteger *builded , const int& num)
{
  for(int i = 0 ; i < 256 ; i++)
    builded->number[i] = 0;
  int factor = 10 , i = 0 , count = 0 , initializer = num;
  while(initializer != 0)
  {
    builded->number[i] = initializer % factor;
    count ++;
    initializer /= 10;
    i ++;
  }
  builded->length = count;
}
BigInteger::BigInteger(const int& num)
{
  numinitial(this , num);
}
BigInteger::BigInteger(const string& num)
{
  for(int i = 0 ; i < 256 ; i ++)
    number[i] = 0;
  int len = num.size();
  for(int i = 0 ; i < len ; i++)
  {
    number[i] = num[len - (i + 1)] - '0';
  }
  length = len;
}
void bigintinitial(BigInteger *builded , const BigInteger& oldnum)
{
  for(int i = 0 ; i < 256 ; i ++)
    builded->number[i] = 0;
  for(int i = 0 ; i < oldnum.length ; i++)
  {
    builded->number[i] = oldnum.number[i];
  }
  builded->length = oldnum.length;
}

BigInteger::BigInteger(const BigInteger& oldnum)
{
  bigintinitial(this , oldnum);
}
BigInteger::~BigInteger()
{
}
// about compare
bool equalbigint(const BigInteger &a ,const BigInteger &b)
{
  if(a.length != b.length)
    return false;
  else
  {
    for(int i = 0 ; i < a.length ; i++)
      if(a.number[i] != b.number[i])
        return false;
  }
  return true;
}
bool comparesmall(const BigInteger* const a , const BigInteger& b)
{
  if(a->length > b.length)
    return false;
  else if(a->length == b.length)
  {
    for(int i = a->length - 1 ; i >= 0 ; i--)
    {
      if(a->number[i]  > b.number[i])
        return false;
      else if(a->number[i] == b.number[i])
        continue;
      else
        return true;
    }
    return false;
  }
  else
    return true;
}
//control the length
void abbreviate(BigInteger &longint)
{
  while(longint.number[longint.length - 1] == 0 && longint.length >= 1)
  {
    longint.length --;
  }
}
// about assign
void assu(BigInteger &a, BigInteger b)
{
  for(int i = 0 ; i < 256 ; i ++)
    a.number[i] = b.number[i];
  a.length = b.length;
}
// about minus
BigInteger minu(const BigInteger* const subtra , const BigInteger &subtrahend)
{
  BigInteger diff;
  int tempnumber[256];
  for(int i = 0 ; i < 256 ; i++)
    tempnumber[i] = subtra->number[i];
  for(int i = 0 ; i < subtra->length ; i++)
    diff.number[i] = tempnumber[i] - subtrahend.number[i];
  for(int i = 0 ; i < subtra->length ; i++)
  {
    if(diff.number[i] < 0)
    {
      diff.number[i] += 10;
      diff.number[i+1] --;
    }
  }
  diff.length = subtra->length;
  abbreviate(diff);
  return diff;
}

//mod
void shift_left(BigInteger &num , int insert)
{
  for(int i = num.length ; i >= 0 ; i--)
    num.number[i] = num.number[i - 1];
  num.number[0] = insert;
  num.length ++;
}
BigInteger mod(const BigInteger* const dividend ,const BigInteger &divisor)
{
  int i = dividend->length - 1;
  BigInteger quotient , sub;
  while(i >= 0)
  {
    shift_left(sub , dividend->number[i]);
    //cout << sub;
    abbreviate(sub);
    if(comparesmall(&divisor , sub) || equalbigint(divisor , sub))
    {
      int subquotient = 0;
      while(comparesmall(&divisor , sub) || equalbigint(divisor , sub))
      {
        assu(sub , minu(&sub , divisor));
        subquotient ++;
      }
      shift_left(quotient , subquotient);
    }
    else
      shift_left(quotient , 0);
    //cout << quotient << quotient.length << "\n";
    i --;
  }
  abbreviate(sub);
  return sub;
}
//operatpr overloading
bool BigInteger::operator<(const BigInteger& compar) const
{
  return comparesmall(this , compar);
}
const BigInteger BigInteger::operator%(const BigInteger& factor) const
{
  return mod(this , factor);
}
BigInteger& BigInteger::operator=(const BigInteger& oldnum)
{
  bigintinitial(this , oldnum);
  return *this;
}
//simple define
bool BigInteger::iszero() const
{
  if(length == 0)
    return true;
  else
    return false;
}
// output
ostream& operator<<(ostream& out, const BigInteger& num)
{
  for(int i = num.length - 1 ; i >= 0 ; i--)
  {
    out << num.number[i] ;
  }
  return out;
}
