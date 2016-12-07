/*
 * TDDD86 Trailblazer
 * This file declares fundamental types used by the Trailblazer assignment.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp, Keith Schwarz, et al
 * Slight modifications by Tommy Farnqvist
 */

#ifndef _types_h
#define _types_h

/* Type: Loc
 *
 * A type representing a location in the world, represented as a pair of a row
 * and a column.
 */
struct TBLoc {
    int row;
    int col;
};

/* Utility function to construct a Loc from its location. */
TBLoc makeLoc(int row, int col);

/* Type: Color
 *
 * An enumerated type representing a color for a node during an execution of
 * Dijkstra's algorithm or A* search.
 */
enum Color {
    UNCOLORED, WHITE, GRAY, YELLOW, GREEN, RED
};

/* Type: Edge
 *
 * A type representing an edge in the grid, as encoded by its start and end node.
 * This type will be useful when implementing Kruskal's algorithm, though you may
 * also find it useful when writing Dijkstra's algorithm or A* search.
 */
struct TBEdge {
    TBLoc start;
    TBLoc end;
};

/* Utility function to create an Edge from its endpoints. */
TBEdge makeEdge(TBLoc start, TBLoc end);



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* The functions below this point are provided for convenience and are not
 * strictly necessary for your implementation.
 */

/* Comparison operators for Loc and Edge.    Our Map and Set type cannot store
 * custom structs as keys (for a map) or values (for a set) unless they can be
 * compared with the relational operators.  You will probably not directly use
 * these in your solution, but you're welcome to do so if you find them useful.
 */
bool operator <  (TBLoc lhs, TBLoc rhs);
bool operator >  (TBLoc lhs, TBLoc rhs);
bool operator == (TBLoc lhs, TBLoc rhs);
bool operator != (TBLoc lhs, TBLoc rhs);
bool operator <= (TBLoc lhs, TBLoc rhs);
bool operator >= (TBLoc lhs, TBLoc rhs);

bool operator <  (TBEdge lhs, TBEdge rhs);
bool operator >  (TBEdge lhs, TBEdge rhs);
bool operator == (TBEdge lhs, TBEdge rhs);
bool operator != (TBEdge lhs, TBEdge rhs);
bool operator <= (TBEdge lhs, TBEdge rhs);
bool operator >= (TBEdge lhs, TBEdge rhs);

/* Hash function for Loc and Edge.  These functions are provided so that you can
 * store Locs and Edges in our HashMap or HashSet type, which require a hashCode
 * function to be available.    You will probably not directly use these in your
 * solution, but you're welcome to do so if you find them useful.
 */
int hashCode(TBLoc l);
int hashCode(TBEdge e);

#endif
