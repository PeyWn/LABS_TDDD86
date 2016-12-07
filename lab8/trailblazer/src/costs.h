/*
 * TDDD86 Trailblazer
 * This file contains functions for computing the costs of navigating the
 * terrain and maze worlds, as well as heuristics for predicting costs.
 * See costs.cpp for implementation of each function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp, Keith Schwarz, et al
 * Slight modifications by Tommy Farnqvist
 */

#ifndef _costs_h
#define _costs_h

#include <cmath>
#include <limits>
#include "grid.h"
#include "types.h"

// represents 'infinity' for use in your various algorithms as needed
const double POSITIVE_INFINITY =  1.0 / 0.0;
const double NEGATIVE_INFINITY = -1.0 / 0.0;

/* Constants representing the value of a wall and floor cell in a maze. */
const double kMazeWall = 0.0;
const double kMazeFloor = 1.0;

const double kAltitudePenalty = 100;


/*
 * A function that given two adjacent locations in a terrain, returns the cost
 * associated with moving from the first location to the second.
 */
double terrainCost(TBLoc from, TBLoc to, const Grid<double>& world);

/*
 * A function that, given two locations in a terrain, estimates the cost
 * of moving from the first location all the way to the second.
 */
double terrainHeuristic(TBLoc from, TBLoc to, const Grid<double>& world);

/*
 * A function that, given two adjacent locations in a maze, returns the cost
 * associated with moving from the first location to the second.
 */
double mazeCost(TBLoc from, TBLoc to, const Grid<double>& world);

/*
 * A function that, given two locations in a maze, estimates the cost of
 * moving from the first location all the way to the second.
 */
double mazeHeuristic(TBLoc from, TBLoc to, const Grid<double>& world);

/*
 * A heuristic function that always returns 0.  This function is used so that
 * the shortestPath function can be used to run Dijkstra's algorithm, which is
 * the same as running A* search with a zero heuristic function.
 */
double zeroHeuristic(TBLoc from, TBLoc to, const Grid<double>& world);

#endif
