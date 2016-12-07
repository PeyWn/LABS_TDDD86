/*
 * TDDD86 Trailblazer
 * This file contains the declaration of some useful graph types,
 * specifically the Node and Arc structures used in the typical graph.
 * We also declare BasicGraph, an instantiation of
 * Stanford's Graph class using Node and Arc as its type parameters.
 *
 * See BasicGraph.cpp for implementation of each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp
 * Slight modifications by Tommy Farnqvist
 */

#ifndef _basicgraph_h
#define _basicgraph_h

#include <string>
#include "graph.h"
#include "grid.h"
#include "set.h"
#include "trailblazergui.h"
using namespace std;

/*
 * Forward declarations of Node/Arc structures so that they can refer
 * to each other mutually.
 */
struct Node;
struct Arc;

/*
 * Canonical Node structure implementation needed by Graph class template.
 * A Node structure represents a vertex in the graph.
 */
struct Node {
public:
    string name;      // required by Stanford Graph; vertex's name
    Set<Arc*> arcs;   // edges outbound from this vertex; to neighbors

    /*
     * The following three fields are 'supplementary data' inside each vertex.
     * You can use them in your path-searching algorithms to store various
     * information related to the vertex
     */
    double cost;      // cost to reach this vertex (initially 0; you can set this)
    bool visited;     // whether this vertex has been visited before (initally false; you can set this)
    Node* previous;   // vertex that comes before this one (initially nullptr; you can set this)

    /*
     * The following three fields are used internally and not by your code.  You
     * must not use their values nor rely on them in any way in your algorithms.
     */
    int m_row;
    int m_col;
    double m_gridValue;

    /*
     * Constructs a vertex with the given name and optional extra information.
     * The extra information is used internally to convert it back to a Grid
     * entry for legacy support if needed.
     */
    Node(string name = "", int row = 0, int col = 0, double gridValue = 0.0);

    /*
     * Returns the color of this node, if any.  Initially WHITE.
     */
    Color getColor() const;

    /*
     * Returns a 'heuristic' value, or rough estimation, of the distance between
     * this vertex and the given other vertex.
     * The heuristic function is guaranteed to be an 'admissible heuristic',
     * meaning that it is never an overestimation of the distance.
     * That property is important for the heuristic function to work properly
     * when used with the A* search algorithm.
     */
    double heuristic(Node* const other) const;

    /*
     * Wipes the supplementary data of this vertex back to its initial state.
     * Specifically, sets cost to 0, visited to false, and previous to nullptr.
     */
    void resetData();

    /*
     * Sets the color of this vertex to be the given color.
     * The color must be one of WHITE, GRAY, YELLOW, or GREEN.
     * Future calls to getColor will return the color you pass here.
     */
    void setColor(Color c);

    /*
     * Returns a string representation of this vertex for debugging, such as
     * "Node{name=r13c42, cost=11, visited=true, previous=r12c41, neighbors={r12c41, r12c43}}".
     */
    string toString() const;

    /*
     * These static functions are used by the provided client code to set up
     * information about the world and heuristic functions used by nodes.
     * You should not call these functions directly in your path algorithms.
     */
    static void setHeuristicFunction(double (* func)(Node* const from, Node* const to, const Grid<double>& world));
    static void setWorld(const Grid<double>& world);

private:
    Color m_color;   // node's color as passed to setColor
    static double (* s_heuristicFunction)(Node* const from, Node* const to,
                                          const Grid<double>& world);
    static Grid<double>* s_world;
};

/*
 * Makes a node printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing a node is not the same as printing a node pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
ostream& operator<<(ostream& out, const Node& node);

/*
 * You can refer to a Node as a Vertex if you prefer.
 */
#define Vertex Node

/*
 * Canonical Arc structure implementation needed by Graph class template.
 * An Arc structure represents an edge in the graph.
 */
struct Arc {
public:
    Node* start;    // edge's starting vertex (required by Graph)
    Node* finish;   // edge's ending vertex (required by Graph)
    double cost;    // edge weight (required by Graph)
    bool visited;   // whether this edge has been visited before (initally false; you can set this)

    /*
     * Constructs a new edge between the given start/end vertices with
     * the given cost.
     */
    Arc(Node* start = nullptr, Node* finish = nullptr, double cost = 0.0);

    /*
     * Wipes the supplementary data of this vertex back to its initial state.
     * Specifically, sets visited to false.
     */
    void resetData();

    /*
     * Returns a string representation of this edge for debugging, such as
     * "Arc{start=r12c42, finish=r12c41, cost=0.75}".
     */
    string toString() const;

    /*
     * The following fields are used internally and not by your code.  You
     * must not use their values nor rely on them in any way in your algorithms.
     */
    int m_startRow;
    int m_startCol;
    int m_finishRow;
    int m_finishCol;
};

/*
 * Makes an arc printable to an output stream.
 * See toString for an example of the output format.
 * Note that printing an arc is not the same as printing an arc pointer.
 * If you try to print a pointer, you will just see its address in hex.
 */
ostream& operator<<(ostream& out, const Arc& arc);

/*
 * You can refer to an Arc as an Edge if you prefer.
 */
#define Edge Arc


/*
 * BasicGraph is just basically an instantiation of Graph using Node and Arc
 * as its template parameters.  It also adds a few convenience functions such
 * as mirroring members like "addArc" with an equivalent more familiar name
 * like "addEdge".
 *
 * There are a few convenience functions related to neighbors, like isNeighbor.
 * BasicGraph contains a DFS implementation called isReachable, not found
 * in the normal Stanford Graph class.
 *
 * There are also a few functions added to make edges more convenient to work with:
 * getEdge(v1, v2) to get the edge between a given pair of vertices,
 * and getInverseArc(arc) to get the edge v2 -> v1 for a given edge v1 -> v2.
 */
class BasicGraph : public Graph<Node, Arc> {
public:
    Arc* getArc(Node* v1, Node* v2) const;
    Arc* getArc(string v1, string v2) const;
    Arc* getEdge(Node* v1, Node* v2) const;
    Arc* getEdge(string v1, string v2) const;
    Arc* getInverseArc(Arc* arc) const;
    Arc* getInverseEdge(Arc* arc) const;
    bool isNeighbor(string v1, string v2) const;
    bool isNeighbor(Node* v1, Node* v2) const;
    void resetData();

    /*
     * The members below are mirrors of ones from Graph but with 'Node' changed
     * to 'Vertex' and/or 'Arc' changed to 'Edge', with identical behavior,
     * and so they are not documented in detail.  See Graph documentation.
     */
    Arc* addEdge(string v1, string v2, double cost = 0.0, bool directed = true);
    Arc* addEdge(Node* v1, Node* v2, double cost = 0.0, bool directed = true);
    Arc* addEdge(Arc* e, bool directed = true);
    Node* addVertex(string name);
    Node* addVertex(Node* v);
    const Set<Arc*>& getEdgeSet() const;
    const Set<Arc*>& getEdgeSet(Node* v) const;
    const Set<Arc*>& getEdgeSet(string v) const;
    Node* getVertex(string name) const;
    const Set<Node*>& getVertexSet() const;
    void removeEdge(string v1, string v2, bool directed = true);
    void removeEdge(Node* v1, Node* v2, bool directed = true);
    void removeEdge(Arc* e, bool directed = true);
    void removeVertex(string name);
    void removeVertex(Node* v);
};

#endif
