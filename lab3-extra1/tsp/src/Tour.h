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
    void show();

    /**
     * Draw the tour on the given scene.
     */
    void draw(QGraphicsScene* scene);

    /**
     *  Returns the number of points in the tour.
     */
    int size();

    /**
     *  Returns the distance of the tour.
     */
    double distance();

    /**
     * Inserts the point p into the Tour.
     * P is inserted after the closest point in the existing tour.
     */
    void insertNearest(Point p);

    /**
     * Inserts the point p into the Tour.
     * P is inserted where it adds the least distance to the tour.
     */
    void insertSmallest(Point p);

    void delCrossingLines();

private:
    Node* firstNode = nullptr;
};

#endif // TOUR_H
