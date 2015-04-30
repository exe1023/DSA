#include <iostream>

#include "biginteger_for_binary.h"

using namespace std;

BigInteger gcd_by_binary(const BigInteger& a, BigInteger& b){
  BigInteger n, m;
  BigInteger ans(1);

  if (a < b){
    n = a; m = b;
  }
  else{
    n = b; m = a;
  }

  while(!n.iszero() && !m.iszero()){
    //cout  << m << n << ans << "\n";
    if (n.iseven()){
      if (m.iseven()){
        ans *= 2;
        n /= 2;
        m /= 2;
      }
      else{
        n /= 2;
      }
    }
    else if (m.iseven()){
      m /= 2;
    }
    if (m < n){
      BigInteger tmp;
      tmp = n; n = m; m = tmp;
    }
    //cout << "end\n " << m.length << " "<< m << n.length << " " << n << "\n";
    m = m - n;
  }
  return (n * ans);
}

int main(){
  string sa, sb;
  cin >> sa >> sb;
  BigInteger a(sa), b(sb);
  BigInteger test1(4402722) , test2(393939);
  BigInteger test3 = test1 - test2;
  test1 /=2;
  //cout << test1;
  cout << gcd_by_binary(a, b) << endl;

  return 0;
}
