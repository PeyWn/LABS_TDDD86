/*
 * TDDD86 Pattern Recognition
 * This file contains the implementation of the Point class.
 * See Point.h for comments about each member.
 * Your code should work properly with an unmodified version of this file.
 */

#include <limits>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cmath>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include "Point.h"

double Point::slopeTo(const Point& p) const {
    if (x == p.x && y == p.y)
        return  -std::numeric_limits<double>::infinity();
    else if (y == p.y) // horizontal line segment
        return 0.0;
    else if (x == p.x) // vertical line segment
        return  std::numeric_limits<double>::infinity();
    else
        return (static_cast<double>(p.y) - static_cast<double>(y)) /
                (static_cast<double>(p.x) - static_cast<double>(x));
}

void Point::draw(QGraphicsScene *scene) const {
    QGraphicsEllipseItem *item = new QGraphicsEllipseItem(x / (COORD_MAX + 1.0) * scene->width(),
                                                          y / (COORD_MAX + 1.0) * scene->height(),
                                                          1, 1);
    item->setBrush(QBrush(QColor(255, 0, 0)));
    scene->addItem(item);
}

void Point::lineTo(QGraphicsScene *scene, const Point& p) const {
    QGraphicsLineItem *item = new QGraphicsLineItem(x / (COORD_MAX + 1.0) * scene->width(),
                                                    y / (COORD_MAX + 1.0) * scene->height(),
                                                    p.x / (COORD_MAX + 1.0) * scene->width(),
                                                    p.y / (COORD_MAX + 1.0) * scene->height());
    item->setPen(QPen(QColor(0, 0, 255), 0));
    scene->addItem(item);
}

bool Point::operator<(const Point& other) const {
    if (x == other.x)
        return y < other.y;
    else
        return x < other.x;
}

bool Point::operator>(const Point& other) const
{
    return other < *this;
}

ostream& operator<<(std::ostream& out, const Point& p) {
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}
