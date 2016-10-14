#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour(){}

Tour::~Tour()
{
    if(firstNode == nullptr){
        return;
    }

    Node* curNode = firstNode;

    do{
        Node* nextNode = curNode->next;
        delete curNode;

        curNode = nextNode;
    }while(curNode != firstNode);
}

void Tour::show() const
{
    if(firstNode == nullptr){
        return;
    }

    Node* curNode = firstNode;

    do{
        cout << *curNode << endl;

        curNode = curNode->next;
    }while(curNode != firstNode);
}

void Tour::draw(QGraphicsScene *scene) const
{
    if(firstNode == nullptr){
        return;
    }

    Node* curNode = firstNode;

    do{
        curNode->point.drawTo(curNode->next->point, scene);

        curNode = curNode->next;
    }while(curNode != firstNode);
}

int Tour::size() const
{
    if(firstNode == nullptr){
        return 0;
    }

    int pointC = 0;
    Node* curNode = firstNode;

    do{
        pointC++;
        curNode = curNode->next;
    }while(curNode != firstNode);

    return pointC;
}

double Tour::distance() const
{
    if(firstNode == nullptr){
        return 0;
    }

    double curDist = 0;
    Node* curNode = firstNode;

    do{
        curDist += curNode->point.distanceTo(curNode->next->point);

        curNode = curNode->next;
    }while(curNode != firstNode);

    return curDist;
}

void Tour::farthestInsertion(list<Point>& points)
{
    int farthestWay = 0;
    Point* point1 = 0;
    Point* point2 = 0;

    //Find the two points farthest away from each other. O(n²)
    for(auto pointA : points){
        for(auto pointB : points){
            int curDist = pointA.distanceTo(pointB);
            if(curDist > farthestWay){
                farthestWay = curDist;
                point1 = &pointA;
                point2 = &pointB;
            }
        }
    }

    firstNode = new Node(point1);
    Node* secondNode = new Node(point2, firstNode);
    FirstNode->next = secondNode;

    //Remove point 1 and 2
    points.remove(*point1);
    points.remove(*point2);
}


