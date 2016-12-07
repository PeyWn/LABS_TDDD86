/*
 * TDDD86 Trailblazer
 * This file contains functions for computing the costs of navigating the
 * terrain and maze worlds, as well as heuristics for predicting costs.
 * See costs.h for declarations of each function and related constants.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp, Keith Schwarz, et al
 * Slight modifications by Tommy Farnqvist
 */

#include "costs.h"
#include <cmath>
#include <limits>
using namespace std;

/*
 * The cost of moving from one location to another in the world is computed as
 *
 * distance(loc1, loc2) * k * |Delta h|
 *
 * This means that the cost of moving from one point to another depends on two
 * factors: the change in elevation and the absolute distance between those
 * points.	Moving in a cardinal direction has base cost 1, while moving
 * diagonally has base cost sqrt(2).
 *
 * The secondary cost associated with moving in the terrain is based on the
 * change in the height between the two points.	 This implementation makes the
 * cost linear in the difference in height.
 */
double terrainCost(TBLoc from, TBLoc to, const Grid<double>& world) {
    // The cost of moving from a location to itself is 0.
	if (from == to) return 0.0;

    // Confirm that the locations are adjacent to one another.
	int drow = abs(to.row - from.row);
	int dcol = abs(to.col - from.col);
	if (drow > 1 || dcol > 1) {
		error("Non-adjacent locations passed into cost function.");
	}

    // Determine the absolute distance between the points.
	double distance = sqrt(double(drow * drow + dcol * dcol));
    double dheight = fabs(world.get(to.row, to.col) - world.get(from.row, from.col));
	return distance + kAltitudePenalty * dheight;
}

/*
 * Our terrain heuristic simply returns the straight-line distance between
 * the two points, plus the height differential scaled appropriately.
 */
double terrainHeuristic(TBLoc from, TBLoc to, const Grid<double>& world) {
	int drow = to.row - from.row;
	int dcol = to.col - from.col;
    double dheight = fabs(world.get(to.row, to.col) - world.get(from.row, from.col));
	return sqrt((double) (drow * drow + dcol * dcol)) + kAltitudePenalty * dheight;
}

/*
 * The cost of moving in a maze is 1.0 when moving in cardinal directions from
 * floors to floors and is infinite otherwise.	This prevents any motion across
 * walls or diagonally.
 */
double mazeCost(TBLoc from, TBLoc to, const Grid<double>& world) {
    // The cost of moving from a location to itself is 0.
	if (from == to) return 0.0;

    // Confirm that the locations are adjacent to one another.
	int drow = abs(to.row - from.row);
	int dcol = abs(to.col - from.col);
	if (drow > 1 || dcol > 1) {
		error("Non-adjacent locations passed into cost function.");
	}
	
    // Moving diagonally costs infinitely much.
    if (drow == 1 && dcol == 1) {
        // return numeric_limits<double>::infinity();
        return POSITIVE_INFINITY;
    }

    // See if we're moving to or from a wall.
    if (world.get(from.row, from.col) == kMazeWall
            || world.get(to.row, to.col) == kMazeWall) {
        // return numeric_limits<double>::infinity();
        return POSITIVE_INFINITY;
    }

	return 1.0;
}

/*
 * The maze heuristic is the rectangular "Manhattan" distance for moving around
 * in a grid world.
 */
double mazeHeuristic(TBLoc from, TBLoc to, const Grid<double>& /*world*/) {
	return abs(from.row - to.row) + abs(from.col - to.col);
}

/*
 * The zero heuristic, unsurprisingly, returns zero and ignores its
 * arguments.
 */
double zeroHeuristic(TBLoc, TBLoc, const Grid<double>&) {
	return 0.0;
}
