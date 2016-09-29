/*
 * TDDD86 Tiles
 * This file contains the declaration of the Tile structure.
 * See Tile.cpp for implementation of each member.
 * Your code should work properly with an unmodified version of this file.
 */

#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <string>
#include <QGraphicsScene>
using namespace std;

/*
 * Each Tile structure represents a single rectangular tile in the simulation.
 * A tile knows its position, size, and color.
 * It contains members that can draw itself, tell you whether it touches a
 * given x/y pixel, and print it on the console for debugging.
 */
struct Tile {
public:
    int x = 0;         // x position of tile's top-left corner
    int y = 0;         // y position of tile's top-left corner
    int width = 0;     // width of tile in pixels
    int height = 0;    // height of tile in pixels
    int r = 0;         // R component of tile's color
    int g = 0;         // G component of tile's color
    int b = 0;         // B component of tile's color

    /*
     * Returns true if this Tile touches the given x/y pixel.
     * For example, if (x, y) is (10, 20) and width x height are 5 x 30,
     * the tile returns true for (10 <= x <= 14, 20 <= y <= 49) inclusive.
     */
    bool contains(int x, int y) const;

    /*
     * Draws the given tile onto the given QGraphicsScene.
     */
    void draw(QGraphicsScene* scene) const;

    /*
     * Returns a string representation of the tile, such as
     * "Tile{x=10,y=20,width=5,height=30,r=255,g=255,b=255}".
     */
    string toString() const;
};

/*
 * Outputs the given tile to the given output stream (e.g. cout) in the same
 * format as returned by its toString member function.
 */
ostream& operator<<(ostream& out, const Tile& tile);

#endif // TILE_H
