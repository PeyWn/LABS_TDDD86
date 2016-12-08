/*
 * Decrypt subset-sum encrypted password using brute force.
 */

#include <iostream>
#include <chrono>
#include "Key.h"

using namespace std;

int
main(int argc, char* argv[])
{
  unsigned char buffer[C+1];     // temporary string buffer
  Key candidate = {{0}};         // a password candidate
  Key encrypted;                 // the encrypted password
  Key candenc;                   // the encrypted password candidate
  Key zero = {{0}};              // the all zero key
  Key T[N];                      // the table T

  if (argc != 2)
    {
      cout << "Usage:" << endl << argv[0] << " password < rand8.txt" << endl;

      return 1;
    }

  encrypted = KEYinit((unsigned char *) argv[1]);

  // read in table T
  for (int i{0}; i < N; ++i)
    {
      scanf("%s", buffer);
      T[i] = KEYinit(buffer);
    }

  auto begin = chrono::high_resolution_clock::now();

  // try all possible subsets of T
  do
    {
      candenc = KEYsubsetsum(candidate, T);
      if (candenc == encrypted)
	cout << candidate << endl;
      ++candidate;
    } while (candidate != zero);

  auto end = chrono::high_resolution_clock::now();
  cout << "Decryption took "
       << std::chrono::duration_cast<chrono::seconds>(end - begin).count()
       << " seconds." << endl;

  return 0;
}
