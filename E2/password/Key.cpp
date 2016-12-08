/*
 * Implementations of the members declared in Key.h
 */

#include <iomanip>
#include "Key.h"

// convert from char to integer 0-31
Key KEYinit(unsigned char s[]) {
  Key k;
  for (int i{0}; i < C; ++i)
    for (int j{0}; j < R; ++j)
      if (s[i] == ALPHABET[j])
	k.digit[i] = j;
  return k;
}

// compute sum of subset of T[i] for which ith bit of k is 1
Key KEYsubsetsum(const Key& k, const Key T[N])  {
  Key sum = {{0}};
  for (int i{0}; i < N; ++i)
    if (KEYbit(k, i)) {
      sum = sum + T[i];
      cout << setw(2) << i << " "; // for debugging
      cout << T[i] << endl;        // for debugging
    }
  return sum;
}

// return the ith bit of Key k
int KEYbit(const Key& k, int i) {
  return (k.digit[i/B] >> (B - 1 - i % B)) & 1;
}

bool operator==(const Key& k1, const Key& k2)
{
  for (int i = 0; i < C; i++) {
    if (k1.digit[i] != k2.digit[i])
      return false;
  }
  return true;
}

bool operator!=(const Key& k1, const Key& k2)
{
  return !(k1 == k2);
}

bool operator<(const Key& k1, const Key& k2)
{
  int i;
  for (i = 0; i < C; i++) {
    if (k1.digit[i] != k2.digit[i])
      {
	if (k1.digit[i] < k2.digit[i])
	  return true;
	return false;
      }
  }
  return false;
}

bool operator>(const Key& k1, const Key& k2)
{
  return k2 < k1;
}

bool operator<=(const Key& k1, const Key& k2)
{
  return !(k1 > k2);
}

bool operator>=(const Key& k1, const Key& k2)
{
  return !(k1 < k2);
}

// k = lhs + rhs mod 2^N
Key operator+(const Key& lhs, const Key& rhs)
{
  Key c = {{0}};
  int carry{0};
  for (int i{C-1}; i >= 0; --i) {
    c.digit[i]  = (lhs.digit[i] + rhs.digit[i] + carry)  % R;
    carry       = (lhs.digit[i] + rhs.digit[i] + carry) >= R;
  }
  return c;
}

Key& operator++(Key& k)
{
  int i{C-1};
  unsigned char one{1};
  unsigned char dig{0};
  int carry;
  dig         = (k.digit[i] + one)  % R;
  carry       = (k.digit[i] + one) >= R;
  k.digit[i] = dig;
  for (i = C-2; i >= 0; --i) {
    if (!carry)
      break;
    dig         = (k.digit[i] + carry)  % R;
    carry       = (k.digit[i] + carry) >= R;
    k.digit[i] = dig;
  }
  return k;
}

Key operator++(Key& k, int)
{
  Key temp = k;
  ++k;
  return temp;
}

// k = lhs - rhs mod 2^N
Key operator-(const Key& lhs, const Key& rhs)
{
  Key c = {{0}};
  int carry{0};

  // if lhs >= rhs the following will clearly work
  // if lhs < rhs we will have carry == -1 after the for loop
  // but this is ok since we are working mod 2^N
  for (int i{C-1}; i >= 0; --i) {
    c.digit[i]  = (lhs.digit[i] - rhs.digit[i] + carry) % R;
    if ((lhs.digit[i] - rhs.digit[i] + carry) < 0)
      {
	carry = -1;
	c.digit[i] += R;
      }
    else
      carry = 0;
  }

  return c;
}

ostream& operator<<(ostream& os, const Key& k)
{
  for (auto i : k.digit)
    os << ALPHABET[i];
  os << "  ";
  for (int i : k.digit)
    os << setw(2) << i << " ";
  os << "  ";
  for (int i{0}; i < N; i++)
    os << KEYbit(k, i);

  return os;
}
