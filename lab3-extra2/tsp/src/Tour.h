#ifndef TOUR_H
#define TOUR_H

#include "Node.h"
#include "Point.h"

/**
 *  A class representing a tour between different points.
 */
class Tour {
public:
    /**
     * Create a new empty Tour;
     */
    Tour();

    /**
     *  Deallocate all dynamic memory used by the Tour.
     */
    ~Tour();

    /**
     * Prints the coordinates of all points in the tour to the console.
     */
    void show() const;

    /**
     * Draw the tour on the given scene.
     */
    void draw(QGraphicsScene* scene) const;

    /**
     *  Returns the number of points in the tour.
     */
    int size() const;

    /**
     *  Returns the distance of the tour.
     */
    double distance() const;

    void farthestInsertion(list<Point>& points);

private:
    Node* firstNode = nullptr;
};

#endif // TOUR_H
