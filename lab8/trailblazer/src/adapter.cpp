/*
 * TDDD86 Trailblazer
 * This file implements code to serve as an "adapter" to convert between various
 * data types used by legacy support code and new code for the current version
 * of this assignment.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp
 * Slight modifcations by Tommy Farnqvist
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
#include "graph.h"
#include "map.h"
#include "random.h"
#include "BasicGraph.h"
#include "costs.h"
#include "trailblazer.h"
#include "trailblazergui.h"
#include "types.h"
#include "adapter.h"

// global variables
static Map<Grid<double>*, BasicGraph*> WORLD_CACHE;
static double (*heuristicFunction)(TBLoc from, TBLoc to, const Grid<double>& world) = NULL;


// function prototype declarations
static double heuristicAdapter(Node* const from, Node* const to, const Grid<double>& world);


// function implementations

BasicGraph* gridToGraph(const Grid<double>& world,
                        double costFn(TBLoc from, TBLoc to, const Grid<double>& world)) {
    BasicGraph* graph = new BasicGraph();

    // add vertices
    int rows = world.numRows();
    int cols = world.numCols();
    int rowColDigits = 0;
    for (int temp = max(rows, cols); temp > 0; temp /= 10) {
        rowColDigits++;
    }
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            string name = vertexName(r, c, world);
            Vertex* v = new Vertex(name, r, c);
            v->m_gridValue = world.get(r, c);
            // cout << "  ensureWorldCache adding vertex " << name << endl;
            graph->addVertex(v);
        }
    }

    // add edges
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Vertex* v = graph->getVertex(vertexName(r, c, world));
            for (int dr = -1; dr <= 1; dr++) {
                for (int dc = -1; dc <= 1; dc++) {
                    int nr = r + dr;
                    int nc = c + dc;
                    if ((dr == 0 && dc == 0) || !world.inBounds(nr, nc)) {
                        continue;
                    }
                    Vertex* neighbor = graph->getVertex(vertexName(nr, nc, world));
                    double cost = costFn(makeLoc(r, c), makeLoc(nr, nc), world);
                    if (cost != POSITIVE_INFINITY) {
                        Edge* e = new Edge(v, neighbor, cost);
                        e->m_startRow = r;
                        e->m_startCol = c;
                        e->m_finishRow = nr;
                        e->m_finishCol = nc;
                        graph->addEdge(e);
                        // cout << "  ensureWorldCache adding edge from "
                        //      << v->name << " to " << neighbor->name
                        //      << " (cost " << setprecision(4) << cost << ")" << endl;
                    }
                }
            }
        }
    }

    return graph;
}

Grid<double>* graphToGrid(BasicGraph* graph) {
    int maxRow = -1;
    int maxCol = -1;
    foreach (Vertex* v in graph->getVertexSet()) {
        maxRow = max(maxRow, v->m_row);
        maxCol = max(maxCol, v->m_col);
    }
    if (maxRow < 0 || maxCol < 0) {
        return NULL;
    }

    // initially set all to be walls
    Grid<double>* grid = new Grid<double>(maxRow + 1, maxCol + 1);
    for (int r = 0; r <= maxRow; r++) {
        for (int c = 0; c <= maxCol; c++) {
            grid->set(r, c, kMazeWall);
        }
    }
    foreach (Vertex* v in graph->getVertexSet()) {
        grid->set(v->m_row, v->m_col, v->m_gridValue);
    }

    return grid;
}

void ensureWorldCache(const Grid<double>& world,
                      double costFn(TBLoc from, TBLoc to, const Grid<double>& world)) {
    Grid<double>* const pWorld = const_cast<Grid<double>*>(&world);
    if (!WORLD_CACHE.containsKey(pWorld)) {
        cout << "Preparing world model ..." << endl;
        BasicGraph* graph = gridToGraph(world, costFn);
        cout << "World model completed." << endl;
        WORLD_CACHE[pWorld] = graph;
    }
}

void flushWorldCache() {
    foreach (Grid<double>* grid in WORLD_CACHE) {
        BasicGraph* graph = WORLD_CACHE[grid];
        delete graph;
    }
    WORLD_CACHE.clear();
}

Vector<TBLoc>
shortestPath(TBLoc start,
             TBLoc end,
             const Grid<double>& world,
             double costFn(TBLoc from, TBLoc to, const Grid<double>& world),
             double heuristicFn(TBLoc from, TBLoc to, const Grid<double>& world),
             AlgorithmType algorithm) {
    // modified by Marty to use an actual Graph object
    ensureWorldCache(world, costFn);
    cout << endl;

    Grid<double>* const pWorld = const_cast<Grid<double>*>(&world);
    BasicGraph* graph = WORLD_CACHE[pWorld];
    // graph->resetData();   // make the student worry about this

    heuristicFunction = heuristicFn;
    Vertex::setWorld(world);
    Vertex::setHeuristicFunction(heuristicAdapter);

    // convert start/end from Loc to Vertex
    Vertex* startVertex = graph->getVertex(vertexName(start.row, start.col, world));
    Vertex* endVertex   = graph->getVertex(vertexName(end.row, end.col, world));
    if (startVertex == NULL) {
        error(string("Graph can not find start vertex with name \"") + vertexName(start.row, start.col, world) + "\"");
        foreach (Vertex* v in graph->getVertexSet()) {
            cout << v->name << " ";
        }
        cout << endl;
    }
    if (endVertex == NULL) {
        error(string("Graph can not find end vertex with name \"") + vertexName(start.row, start.col, world) + "\"");
        foreach (Vertex* v in graph->getVertexSet()) {
            cout << v->name << " ";
        }
        cout << endl;
    }

    cout << "Looking for a path from " << startVertex->name
         << " to " << endVertex->name << "." << endl;

    vector<Vertex*> result;
    switch (algorithm) {
    case BFS:
        cout << "Executing breadth-first search algorithm ..." << endl;
        result = breadthFirstSearch(*graph, startVertex, endVertex);
        break;
    case DIJKSTRA:
        cout << "Executing Dijkstra's algorithm ..." << endl;
        result = dijkstrasAlgorithm(*graph, startVertex, endVertex);
        break;
    case A_STAR:
        cout << "Executing A* algorithm ..." << endl;
        result = aStar(*graph, startVertex, endVertex);
        break;
    case DFS:
    default:
        cout << "Executing depth-first search algorithm ..." << endl;
        result = depthFirstSearch(*graph, startVertex, endVertex);
        break;
    }

    cout << "Algorithm complete." << endl;

    // convert vector<Vertex*> to Vector<Loc>
    Vector<TBLoc> locResult;
    foreach (Vertex* v in result) {
        locResult.add(makeLoc(v->m_row, v->m_col));
    }
    return locResult;
}

Set<TBEdge> createMaze(int /* numRows */, int /* numCols */) {
    Set<TBEdge> set;
    return set;
}

static double heuristicAdapter(Node* const from, Node* const to, const Grid<double>& world) {
    if (heuristicFunction == NULL) {
        return 0.0;
    } else {
        return heuristicFunction(makeLoc(from->m_row, from->m_col),
                                 makeLoc(to->m_row, to->m_col), world);
    }
}

string vertexName(int r, int c, const Grid<double>& world) {
    // zero-pad the number of rows/cols for better alphabetic sorting
    int digits = 0;
    for (int temp = max(world.numRows(), world.numCols()); temp > 0; temp /= 10) {
        digits++;
    }
    ostringstream out;
    out << 'r' << setw(digits) << setfill('0') << r
        << 'c' << setw(digits) << setfill('0') << c;
    return out.str();
}
