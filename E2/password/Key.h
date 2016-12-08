/*
 * A structure for representing C digit numbers in base R.
 */

#ifndef KEY_H
#define KEY_H

#include <iostream>
#include <string>
using namespace std;

static const int C = 5;         // number of characters in password
static const int B = 5;         // number of bits per character
static const int R = (1 << B);  // size of alphabet (32)
static const int N = (B * C);   // number of bits per password

static const string ALPHABET = "abcdefghijklmnopqrstuvwxyz012345";

/****************************************************************
 * An extended precision base R integer consisting of C digits.
 * An array packaged in a struct for easy memory management and
 * pass-by-value.
 ****************************************************************/
typedef struct {
  unsigned char digit[C];
} Key;

/****************************************************************
 * Initialize k from a character string.
 * Example: s = "abcdwxyz"  =>  k = 0 1 2 3 22 23 24 25
 ****************************************************************/
Key  KEYinit(unsigned char s[]);

/****************************************************************
 * Add and return the subset of the integers T[i] that are
 * indexed by the bits of k. Do sum mod 2^N.
 ****************************************************************/
Key  KEYsubsetsum(const Key& k, const Key T[N]);

int  KEYbit(const Key& k, int i);        // return the ith bit of k

bool operator==(const Key&, const Key&);
bool operator!=(const Key&, const Key&);
bool operator<(const Key&, const Key&);
bool operator>(const Key&, const Key&);
bool operator<=(const Key&, const Key&);
bool operator>=(const Key&, const Key&);
Key  operator+(const Key&, const Key&);  // return a + b mod 2^N
Key  operator++(Key&, int);              // postfix increment
Key&  operator++(Key&);                  // prefix increment
Key  operator-(const Key&, const Key&);  // return a - b mod 2^N
ostream& operator<<(ostream&, const Key&);

#endif
