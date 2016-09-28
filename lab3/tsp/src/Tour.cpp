// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <iostream>
#include "Tour.h"
#include "Node.h"
#include "Point.h"

Tour::Tour()
{

}

Tour::~Tour()
{
    if(firstNode == 0){
        return;
    }

    Node* curNode = firstNode;

    do{
        Node* nextNode = curNode->next;
        delete curNode;

        curNode = nextNode;
    }while(curNode != firstNode);
}

void Tour::show()
{
    if(firstNode == 0){
        return;
    }

    Node* curNode = firstNode;

    do{
        cout << *firstNode << endl;

        curNode = curNode->next;
    }while(curNode != firstNode);
}

void Tour::draw(QGraphicsScene *scene)
{
    if(firstNode == 0){
        return;
    }

    Node* curNode = firstNode;

    do{
        curNode->point.drawTo(curNode->next->point, scene);

        curNode = curNode->next;
    }while(curNode != firstNode);
}

int Tour::size()
{
    if(firstNode == 0){
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

double Tour::distance()
{
    if(firstNode == 0){
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

void Tour::insertNearest(Point p)
{
    if(firstNode == nullptr){
        firstNode = new Node(p);
        firstNode->next = firstNode;
        return;
    }

    Node* bestNode = firstNode;
    Node* curNode = firstNode->next;


    while(curNode != firstNode){
        if(curNode->point.distanceTo(p) < bestNode->point.distanceTo(p)){
            bestNode = curNode;
        }

        curNode = curNode->next;
    }

    bestNode->next = new Node(p, bestNode->next);

}

void Tour::insertSmallest(Point p)
{
    // TODO: write this member
}
