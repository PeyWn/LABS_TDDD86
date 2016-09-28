#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"

/**
 *  An ordered list to keep Tiles in.
 */
class TileList {
public:
    /*
     * Creates a new empty TileList of
     */
    TileList();

    /*
     * Deallocates all memory used by the TileList
     */
    ~TileList();

    /*
     * Adds a Tile to the end of the list
     */
    void addTile(Tile tile);

    /*
     * Draws all tiles in the list from the beginning to the end, so that the tiles
     * in the beginning appear behind the tiles in the end of the list.
     */
    void drawAll(QGraphicsScene* scene) const;

    /*
     * Finds the last tile that covers the current coordinates and returns the index.
     */
    int indexOfTopTile(const int x, const int y) const;

    /*
     * Moves the last tile that covers the current coordinates to the beginning of the list.
     */
    void lower(const int x, const int y);

    /*
     * Moves the last tile that covers the current coordinates to the end of the list.
     */
    void raise(const int x, const int y);

    /*
     * Removes the last tile that covers the current coordinates.
     */
    void remove(const int x, const int y);

    /*
     * Removes all tiles that covers the current coordinates.
     */
    void removeAll(const int x, const int y);

private:
    int size = 0;
    int capacity = 10;
    Tile* elements = new Tile[capacity];
};

#endif // TILELIST_H
