/*
 * TDDD86 Trailblazer
 * This file declares code to serve as an "adapter" to convert between various
 * data types used by legacy support code and new code for the current version
 * of this assignment.
 * Specifically, past quarters represented a graph as a Grid<double> and each
 * of its vertices/edges as a Loc (TBLoc) and Edge (TBEdge), while the
 * current version of the assignment represents the graph as a BasicGraph
 * (an extension of Stanford's Graph class) and each vertex/edge as a
 * Node (Vertex) / Arc (Edge) object.
 * This code converts between the two formats to help localize changes.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp
 * Slight modifications by Tommy Farnqvist
 */

#ifndef _adapter_h
#define _adapter_h

#include "grid.h"
#include "set.h"
#include "BasicGraph.h"
#include "types.h"

/* Type: AlgorithmType
 *
 * An enumerated type representing the various algorithms to choose from.
 */
enum AlgorithmType {
    AUTODETECT,
    BFS,
    DFS,
    DIJKSTRA,
    A_STAR
};

/*
 * Finds the shortest path between the locations given by start and end in the
 * specified world.  The cost of moving from one edge to the next is specified
 * by the given cost function.  The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.
 * If no path is found, this function should report an error.
 */
Vector<TBLoc>
shortestPath(TBLoc start,
             TBLoc end,
             const Grid<double>& world,
             double costFn(TBLoc from, TBLoc to, const Grid<double>& world),
             double heuristicFn(TBLoc from, TBLoc to, const Grid<double>& world),
             AlgorithmType algorithm = AUTODETECT);

// Support functions called by the GUI to improve loading times for large graphs.

/*
 * Converts the given graph into a 2D grid structure, which is returned;
 * essentially the opposite of gridToGraph.
 * Used to convert randomly generated mazes into grids for legacy code support.
 */
Grid<double>* graphToGrid(BasicGraph* graph);

/*
 * Converts the given 2D grid structure into a BasicGraph object,
 * using the Grid to grab the vertices and the given cost function to grab
 * the edge weights between neighbors.
 * Returns a pointer to the graph that was created.
 */
BasicGraph* gridToGraph(const Grid<double>& world,
                        double costFn(TBLoc from, TBLoc to, const Grid<double>& world));

/*
 * Makes sure that the given world has already been converted into an equivalent
 * BasicGraph.  If it has not, does so (by calling gridToGraph) and caches the
 * result to be used on future calls.
 * This is done to improve runtime when very large/huge mazes and terrains are
 * loaded and then searched multiple times by the user.
 */
void ensureWorldCache(const Grid<double>& world,
                      double costFn(TBLoc from, TBLoc to, const Grid<double>& world));

/*
 * Removes all entries from the internal cache of BasicGraphs and frees
 * any memory associated with them.
 */
void flushWorldCache();

/*
 * Returns a name for the given row/column location in the given world,
 * such as "r08c17".
 * The reason you have to pass the world is so that I know how many rows/cols
 * it has so I can zero-pad the numbers in the string.
 */
string vertexName(int r, int c, const Grid<double>& world);

/*
 * (Extension)
 *
 * Creates a maze of the specified dimensions using a randomized version of
 * Kruskal's algorithm, then returns a set of all of the edges in the maze.
 *
 * As specified in the assignment handout, the edges you should return here
 * represent the connections between locations in the graph that are passable.
 * Our provided starter code will then use these edges to build up a Grid
 * representation of the maze.
 */
Set<TBEdge> createMaze(int numRows, int numCols);

#endif
