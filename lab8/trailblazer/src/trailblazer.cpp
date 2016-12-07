#include "costs.h"
#include "trailblazer.h"
#include <queue>
#include <pqueue.h>
#include <algorithm>

using namespace std;

/*
 * Recursiv help function for Depth First Search.
 */
bool DFSHelp (vector<Vertex*>& path, BasicGraph& graph, Vertex* start, Vertex* end){
    start->setColor(GREEN);
    path.push_back(start);
    start->visited = true;
    if(start == end){
        return true;
    }
    for(Node* n : graph.getNeighbors(start)){
        if(!n->visited && DFSHelp(path, graph, n, end)){
            return true;
        }
    }
    path.pop_back();
    start->setColor(GRAY);

    return false;
}

vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;

    DFSHelp(path, graph, start, end);
    graph.resetData();
    return path;
}


vector<Node *> breadthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;
    queue<Vertex*> q;
    bool found = false;
    q.push(start);
    start->setColor(YELLOW);
    start->visited = true;

    while(!q.empty()){
        q.front()->setColor(GREEN);
        if(q.front() == end){
            found = true;
            break;
        }
        for(Node* n : graph.getNeighbors(q.front())){
            if(!n->visited){
                n->setColor(YELLOW);
                n->visited = true;
                n->previous = q.front();
                q.push(n);
            }
        }

        q.pop();
    }
    if(found){
        path.push_back(q.front());
        Node* prePointer = q.front()->previous;
        while(prePointer != nullptr){
            path.push_back(prePointer);
            prePointer = prePointer->previous;
        }

        reverse(path.begin(), path.end());
    }
    graph.resetData();
    return path;
}

vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;
    PriorityQueue<Vertex*> q;
    bool found = false;
    q.enqueue(start, 0);
    start->cost = 0;
    start->setColor(YELLOW);
    start->visited = true;

    Vertex* curNode;

    while(!q.isEmpty()){
        curNode = q.dequeue();
        curNode->setColor(GREEN);
        if(curNode == end){
            found = true;
            break;
        }

        for(Arc* a: graph.getArcSet(curNode)){
            double newCost = (curNode->cost + a->cost);
            Vertex* n = a->finish;
            if(!n->visited){
                n->setColor(YELLOW);
                n->cost = newCost;
                n->visited = true;
                n->previous = curNode;
                q.enqueue(n, newCost);
            }
            else if(newCost < n->cost){
                n->cost = newCost;
                n->previous = curNode;

                q.changePriority(n, newCost);
            }
        }

    }
    if(found){
        path.push_back(curNode);
        Node* prePointer = curNode->previous;
        while(prePointer != nullptr){
            path.push_back(prePointer);
            prePointer = prePointer->previous;
        }

        reverse(path.begin(), path.end());
    }
    graph.resetData();
    return path;
}

vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;
    PriorityQueue<Vertex*> q;
    bool found = false;
    start->cost = 0;
    q.enqueue(start, start->heuristic(end));
    start->setColor(YELLOW);
    start->visited = true;

    Vertex* curNode;

    while(!q.isEmpty()){
        curNode = q.dequeue();
        curNode->setColor(GREEN);
        if(curNode == end){
            found = true;
            break;
        }

        for(Arc* a: graph.getArcSet(curNode)){
            Vertex* n = a->finish;
            double newCost = (curNode->cost + a->cost);
            if(!n->visited){
                n->setColor(YELLOW);
                n->cost = newCost;
                n->visited = true;
                n->previous = curNode;
                q.enqueue(n, (newCost + n->heuristic(end)));
            }
            else if(newCost < n->cost){
                n->cost = newCost;
                n->previous = curNode;

                q.changePriority(n, (newCost + n->heuristic(end)));
            }
        }

    }
    if(found){
        path.push_back(curNode);
        Node* prePointer = curNode->previous;
        while(prePointer != nullptr){
            path.push_back(prePointer);
            prePointer = prePointer->previous;
        }

        reverse(path.begin(), path.end());
    }
    graph.resetData();
    return path;
}
