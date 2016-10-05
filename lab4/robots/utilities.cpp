/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "utilities.h"
#include "constants.h"
#include <cstdlib>
#include <ctime>

void rand_seed() {
    int seed = static_cast<int>(time(0));
    srand(seed);
}

int rand_int(int a, int b) {
    return a + rand() % (b - a + 1);
}
