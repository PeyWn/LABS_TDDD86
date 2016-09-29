/*
 * TDDD86 TSP
 * This file contains the implementation of the Node structure.
 * See Node.h for comments about each member.
 * Your code should work properly with an unmodified version of this file.
 */

#include <iomanip>
#include <iostream>
#include <sstream>
#include "Node.h"

Node::Node(Point p, Node* _next)
    : point(p.x, p.y), next(_next) {}

string Node::toString() const {
    stringstream out;
    out << "Node{addr=" << ((void*) this);
    out << ", point=" << point;
    if (next != nullptr) {
        out << ", next=" << ((void*) next);
    } else {
        out << ", next=nullptr";
    }
    out << "}";
    return out.str();
}

ostream& operator <<(ostream& out, const Node& node) {
    out << node.toString();
    return out;
}
