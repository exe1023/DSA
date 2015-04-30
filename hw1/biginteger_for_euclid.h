#ifndef BIGINTEGER_FOR_EUCLID

#define BIGINTEGER_FOR_EUCLID
#include <iostream>
#include <string>
class BigInteger{
public:
  BigInteger();
  BigInteger(const int&);
  BigInteger(const std::string&);
  BigInteger(const BigInteger&);
  ~BigInteger();

  bool operator<(const BigInteger&) const;
  const BigInteger operator%(const BigInteger&) const;
  BigInteger& operator=(const BigInteger&);
  bool iszero() const;

  friend std::ostream& operator<<(std::ostream&, const BigInteger&);
  int number[256];
  int length;
};


#endif
