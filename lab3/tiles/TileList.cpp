#include "TileList.h"

TileList::~TileList()
{
    delete[] elements;
}

void TileList::addTile(Tile tile)
{
    if(capacity == size){
        capacity *= 2;
        Tile* newArray = new Tile[capacity];

        for(int i = 0; i < size; i++){
            newArray[i] = elements[i];
        }

        delete[] elements;
        elements = newArray;

    }
    elements[size] = tile;
    size++;
}



void TileList::drawAll(QGraphicsScene* scene) const
{
    for(int i = 0; i < size; i++){
        elements[i].draw(scene);
    }
}

int TileList::indexOfTopTile(const int x, const int y) const
{
    for(int i = (size - 1); i >= 0; i--){
        if (elements[i].contains(x, y)){
            return i;
        }
    }

    return -1;
}

void TileList::raise(const int x, const int y)
{
    Tile targetTile;

    int targetI = indexOfTopTile(x, y);

    if(targetI == -1){
        return;
    }

    targetTile = elements[targetI];

    for(int j = (targetI + 1); j < size; j++){
        elements[j - 1] = elements[j];
    }

    elements[size - 1] = targetTile;
}

void TileList::lower(const int x, const int y)
{
    Tile targetTile;

    int targetI = indexOfTopTile(x, y);

    if(targetI == -1){
        return;
    }

    targetTile = elements[targetI];

    for(int i = targetI; i > 0; i--){
            elements[i] = elements[i-1];
    }
    elements[0] = targetTile;
}

void TileList::remove(const int x, const int y)
{
    int targetI = indexOfTopTile(x, y);

    if(targetI == -1){
        return;
    }

    for(int j = (targetI + 1); j < size; j++){
        elements[j - 1] = elements[j];
    }

    size--;
    return;
}

void TileList::removeAll(const int x, const int y)
{
    int offset = 0;
    for(int i = 0; i < size; i++){
        if (elements[i].contains(x, y)){
            offset++;
        }
        else {
            elements[i - offset] = elements[i];
        }
    }

    size -= offset;
}
