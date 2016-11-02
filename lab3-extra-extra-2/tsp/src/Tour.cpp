#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"
#include <QApplication>

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

void Tour::farthestInsertion(list<Point>& points){
    double farthestWay = 0;
    Point* point1 = 0;
    Point* point2 = 0;

    //Find the two points farthest away from each other. O(n²)
    for(auto pointA : points){
        for(auto pointB : points){
            double curDist = pointA.distanceTo(pointB);

            if(curDist > farthestWay){
                farthestWay = curDist;
                point1 = new Point(pointA);
                point2 = new Point(pointB);
            }
        }
    }

    firstNode = new Node(*point1);
    Node* secondNode = new Node(*point2, firstNode);
    firstNode->next = secondNode;

    //Remove point 1 and 2
    removeFirstPoint(*point1, points);
    removeFirstPoint(*point2, points);

    //Insert rest of points
    while(!points.empty()){
        Point* farthestPoint;
        Node* insertNode; //Where in tour farthesPoint should be inserted
        double farthestLength = 0;

        for(auto point: points){
            Node* curInsertNode = firstNode;
            double shortPath = (firstNode->point.distanceTo(point) + firstNode->next->point.distanceTo(point));
            Node* tourPointer = firstNode->next;

            double distanceToNodes = firstNode->point.distanceTo(point);

            while(tourPointer != firstNode){
                distanceToNodes += tourPointer->point.distanceTo(point);

                double curDelta = (tourPointer->point.distanceTo(point) + tourPointer->next->point.distanceTo(point));

                if(curDelta < shortPath){
                    shortPath = curDelta;
                    curInsertNode = tourPointer;
                }
                tourPointer = tourPointer->next;
            }

            if(distanceToNodes > farthestLength){
                farthestLength = distanceToNodes;
                farthestPoint = new Point(point);
                insertNode = curInsertNode;
            }
        }

        //Insert the farthest point
        Node* newNode = new Node(*farthestPoint, insertNode->next);
        insertNode->next = newNode;

        removeFirstPoint(*farthestPoint, points);
    }
}

void Tour::removeFirstPoint(const Point& value, list<Point>& points){
    for(auto it = points.begin(); it != points.end(); it++){
        if(value.x == it->x && value.y == it->y){
            points.erase(it);
            return;
        }
    }
}


