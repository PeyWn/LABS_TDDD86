/**********************************************************************
 *  Encrypt a file using subset-sum algorithm.
 *
 *  Execution:      % encrypt password < rand8.txt
 *                  vbskbezp
 *
 *  Note: need to set C = 8 in Key.h before compiling and executing
 *        with rand8.txt.
 *
 **********************************************************************/

#include <iostream>
#include "Key.h"

using namespace std;

int
main(int argc, char* argv[])
{
  unsigned char buffer[C+1];     // temporary string buffer
  Key password;                  // the user supplied password
  Key encrypted;                 // the encrypted password
  Key T[N];                      // the table T

  if (argc != 2)
    {
      cout << "Usage:" << endl << argv[0] << " password < rand8.txt" << endl;

      return 1;
    }

  password = KEYinit((unsigned char *) argv[1]);
  // print out input password
  cout << "   " << password << endl;

  // read in table T
  for (int i{0}; i < N; ++i)
    {
      scanf("%s", buffer);
      T[i] = KEYinit(buffer);
    }

  // compute subset sum
  encrypted = KEYsubsetsum(password, T);

  // print results
  cout << "   " << encrypted << endl;

  return 0;
}
