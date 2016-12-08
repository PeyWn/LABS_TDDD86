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
    start->visited = true;
    path.push_back(start);

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

/*
 * Pathfinding using depth first algorithm.
 */
vector<Node *> depthFirstSearch(BasicGraph& graph, Vertex* start, Vertex* end) {
    vector<Vertex*> path;

    DFSHelp(path, graph, start, end);

    graph.resetData();
    return path;
}

/*
 * Pathfinding using breasth first algorithm.
 */
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

/*
 * Pathfinding using a priority queue (for example Dijkstras, A* algorithm).
 * Takes a lambda-expression to determine the value for priority when inserting into the priority queue.
 * First vertex is the one being inserted, second vertex is end node.
 */
vector<Node *> costFinder(BasicGraph& graph, Vertex* start, Vertex* end, function<double(Vertex*, Vertex*)> priorityFunc) {
    vector<Vertex*> path;
    PriorityQueue<Vertex*> q;
    bool found = false;

    start->cost = 0;
    q.enqueue(start, priorityFunc(start, end));
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
                q.enqueue(n, priorityFunc(n, end));
            }
            else if(newCost < n->cost){
                n->cost = newCost;
                n->previous = curNode;

                /*
                 * There is an extreme special case where n is not part of the priority queue
                 * (cost to curNode < cost to n, but (curNode->cost + curNode -> heuristic) > (n->cost + n->heuristic)),
                 * eg it has already been prossesed.
                 *
                 * A check for if n is in priority queue has to be done. Checking color is the best way to do this, O(1).
                 */
                if(n->getColor() == YELLOW){ //In queue
                    q.changePriority(n, priorityFunc(n, end));
                }
                else{ //Has been prosessed
                    n->setColor(YELLOW);
                    q.enqueue(n, priorityFunc(n, end));
                }
            }
        }
    }

    if(found){
        path.push_back(curNode);
        Vertex* prePointer = curNode->previous;

        while(prePointer != nullptr){
            path.push_back(prePointer);
            prePointer = prePointer->previous;
        }
n
        //Reverse vector to get start vertex first.
        reverse(path.begin(), path.end());
    }

    graph.resetData();
    return path;
}

/*
 * Pathfinding using Dijkstras algorithm.
 */
vector<Node *> dijkstrasAlgorithm(BasicGraph& graph, Vertex* start, Vertex* end) {
    auto prioFunc = [](Vertex* v1, Vertex* v2){return v1->cost;};
    return costFinder(graph, start, end, prioFunc);
}


/*
 * Pathfinding using A* algorithm with built in heuristic from Node calss.
 */
vector<Node *> aStar(BasicGraph& graph, Vertex* start, Vertex* end) {
    auto prioFunc = [](Vertex* v1, Vertex* v2){return (v1->cost + v1->heuristic(v2));};
    return costFinder(graph, start, end, prioFunc);
}

