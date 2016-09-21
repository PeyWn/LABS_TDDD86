#ifndef TILELIST_H
#define TILELIST_H

#include <QGraphicsScene>
#include "Tile.h"

class TileList {
public:
    TileList();
    ~TileList();
    void addTile(Tile tile);
    void drawAll(QGraphicsScene* scene);
    int indexOfTopTile(int x, int y);
    void lower(int x, int y);
    void raise(int x, int y);
    void remove(int x, int y);
    void removeAll(int x, int y);

private:
    int listSize;
    Tile elements[];
};

#endif // TILELIST_H
