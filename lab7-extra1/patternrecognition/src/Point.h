/*
 * TDDD86 Pattern Recognition
 * This file contains the declaration of the Point class.
 * See Point.cpp for implementation of each member.
 * Your code should work properly with an unmodified version of this file.
 */

#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <QGraphicsScene>
using namespace std;

static const int COORD_MAX = 32767; // max value of x and y coordinates

/*
 * Each Point object represents an immutable single point
 * with integer coordinates in the Euclidean plane.
 */
class Point {
public:

    Point() = delete;
    Point(unsigned int _x, unsigned int _y) : x(_x), y(_y){}

    /**
     * Slope between this point and p
     *
     * If the points are the same, negative infinity is returned
     * If the line between the points is horizontal positive zero is returned
     * If the line between the points is vertical positive infinity is returned
     */
    double slopeTo(const Point& p) const;
    /**
     * Draw point to scene
     */
    void draw(QGraphicsScene* scene) const;
    /**
     * Draw line from this point to p to scene
     */
    void lineTo(QGraphicsScene* scene, const Point& p) const;

    /**
     * Is this point lexicographically smaller than p?
     * Comparing x-ccordinates and breaking ties by y-coordinates
     */
    bool operator<(const Point&) const;
    bool operator>(const Point&) const;

    friend ostream& operator<<(ostream&, const Point&);

private:
    unsigned int x, y; // position
};

#endif // POINT_H
