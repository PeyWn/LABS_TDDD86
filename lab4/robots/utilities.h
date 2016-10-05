/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

struct Point {
    int x;
    int y;
};

/**
 * Sets the seed of the random number generator.
 */
void rand_seed();

/**
 *  Returns a random integer in a range.
 *  @param a the bottom of the range
 *  @param b the top of the range
 *  &return a random number x having a <= x and x <= b
 */
int rand_int(int a, int b);

#endif // UTILITIES_H
