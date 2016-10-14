/*
 * TDDD86 TSP
 * This file contains the declaration of the Point structure.
 * See Point.cpp for implementation of each member.
 * Your code should work properly with an unmodified version of this file.
 */

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <string>
#include <QGraphicsScene>
using namespace std;

/*
 * Each Point structure represents a single point in the Euclidean plane.
 */
struct Point {
    const double x; // x position of point
    const double y; // y position of point

    Point(double _x, double _y);

    /*
     * Returns Euclidean distance between this point and that.
     */
    double distanceTo(Point that) const;

    /*
     * Draws the point onto the given QGraphicsScene.
     */
    void draw(QGraphicsScene* scene) const;

    /*
     * Draws the line from this point to that onto the given QGraphicsScene.
     */
    void drawTo(Point that, QGraphicsScene* scene) const;

    /*
     * Returns a string representation of the point, such as
     * (2.5, 3.5).
     */
    string toString() const;
};

/*
 * Outputs the given point to the given output stream (e.g. cout) in the same
 * format as returned by its toString member function.
 */
ostream& operator<<(ostream& out, const Point& p);

#endif // POINT_H
