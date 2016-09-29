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
    if(firstNode == nullptr){
        return;
    }

    Node* curNode = firstNode->next;
    vector<Node*> curToCompNodes;

    while(curNode != firstNode){
        cout << "curNode: " << curNode << endl;

        Node* compNode = curNode->next;

        while (compNode != curNode) {
            cout << "compNode: " << compNode << " next: " << compNode->next << endl;

            /**
            *Checks if a switch would result in a shourter route
            */
            if( (curNode->point.distanceTo(compNode->point) + curNode->next->point.distanceTo(compNode->next->point))
                 < (curNode->point.distanceTo(curNode->next->point) + compNode->point.distanceTo(compNode->next->point))){

                //Add all points from curNode to compNode to a vector
                Node* nodeAfterSwap = compNode->next;
                Node* endOfLoop = compNode;

                Node* vectorAdder = curNode->next;
                do{
                    cout << "add to vector: " << *vectorAdder << endl;
                    curToCompNodes.push_back(vectorAdder);

                    vectorAdder = vectorAdder->next;
                }while(vectorAdder != endOfLoop);

                curNode->next = compNode;

                //Reverse the pointing direction of every node in curToCompnodes
                for(int i = 0; i < curToCompNodes.size() - 1; i++){
                    curToCompNodes[i + 1]->next = curToCompNodes[i];
                }

                curToCompNodes[0]->next = nodeAfterSwap;
                compNode = curToCompNodes[0];
            }

            compNode = compNode->next;
        }

        curNode = curNode->next;
        cout << "new curNode: " << *curNode << endl;
    }
}


