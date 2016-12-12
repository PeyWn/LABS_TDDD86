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

void Tour::show()
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

void Tour::draw(QGraphicsScene *scene)
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

int Tour::size()
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

double Tour::distance()
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
    if(firstNode == nullptr){
        firstNode = new Node(p);
        firstNode->next = firstNode;
        return;
    }

    Node* bestNode = firstNode;
    double bestWay = (firstNode->point.distanceTo(p)
                      + p.distanceTo(firstNode->next->point)
                      - firstNode->point.distanceTo(firstNode->next->point));

    Node* curNode = firstNode->next;

    while(curNode != firstNode){
        //Calculates the delta distance of the path if insertion would happen after curPoint.
        double curWay = (curNode->point.distanceTo(p)
                         + p.distanceTo(curNode->next->point)
                         - curNode->point.distanceTo(curNode->next->point));

        if(curWay < bestWay){
            bestNode = curNode;
            bestWay = curWay;
        }

        curNode = curNode->next;
    }

    bestNode->next = new Node(p, bestNode->next);
}

void Tour::delCrossingLines()
{
    /*
    Node* newFirst = firstNode->next;
    reverseOrder(firstNode->next, firstNode);
    newFirst->next = firstNode;
    */
    Node* compNode1 = firstNode;
    Node* compNode2;

    do{
        compNode2 = compNode1->next;
        while(compNode2 != compNode1){
            //find cross
            cout << "d1: " << (compNode1->point.distanceTo(compNode1->next->point) +
                               compNode2->point.distanceTo(compNode2->next->point)) << endl;

            cout << "d2: " << (compNode1->point.distanceTo(compNode2->point) +
                               compNode1->next->point.distanceTo(compNode2->next->point)) << endl;
            if((compNode1->point.distanceTo(compNode1->next->point) +
                compNode2->point.distanceTo(compNode2->next->point)) >
                    (compNode1->point.distanceTo(compNode2->point) +
                     compNode1->next->point.distanceTo(compNode2->next->point))){
                cout << "switch?" << endl;
                //Do switch
                Node* comp1Next = compNode1->next;
                Node* comp2Next = compNode2->next;

                reverseOrder(comp1Next, compNode2);
                comp1Next->next = comp2Next;
                compNode1->next = compNode2;
            }

            compNode2 = compNode2->next;
        }
        compNode1 = compNode1->next;
    }while(compNode1 != firstNode);
}

void Tour::reverseOrder(Node* start, Node* end){
    Node* refNode = start;
    Node* curNode = start->next;
    Node* curNextNode = start->next->next;

    while(refNode != end){
        curNode->next = refNode;

        refNode = curNode;
        curNode = curNode->next;
        curNextNode = curNode->next;

        //cout << curNode->toString() << " " << curNextNode->endl;
    }
}
