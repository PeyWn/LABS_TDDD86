/*
 * TDDD86 TSP
 * This file contains the declaration of the Node structure.
 * See Node.cpp for implementation of each member.
 * Your code should work properly with an unmodified version of this file.
 */

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "Point.h"
using namespace std;

/*
 * Each Node structure represents a single node in a circular linked list
 * used in a TSP tour.
 */
struct Node {
    Point point;     // this nodes point
    Node* next;      // pointer to next node in the list  (nullptr if none)

    /*
     * Constructs a new node storing the given point and next pointer.
     */
    Node(Point p, Node* _next = nullptr);

    /*
     * Returns a string representation of the Node for debugging, such as
     * "Node{addr=0x7e83f4, point=(2.5, 3.5), next=0x43b2a0}".
     *
     * Note that the toString output includes the node's memory address, as well
     * as the memory address where its next pointer points.
     *
     * Keep in mind that toString won't be called if you try to print a Node*.
     * You must print the node itself.
     */
    string toString() const;
};

/*
 * Outputs the given node to the given output stream (e.g. cout) in the same
 * format as returned by its toString member function.
 */
ostream& operator <<(ostream& out, const Node& node);

#endif // END NODE_H
