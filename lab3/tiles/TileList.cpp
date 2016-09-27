// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "TileList.h"

TileList::TileList(){}

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



void TileList::drawAll(QGraphicsScene* scene)
{
    for(int i = 0; i < size; i++){
        elements[i].draw(scene);
    }
}

int TileList::indexOfTopTile(int x, int y)
{
    int tileI = -1;

    for(int i = 0; i < size; i++){
        if (elements[i].contains(x, y)){
            tileI = i;
        }
    }

    return tileI;
}

void TileList::raise(int x, int y)
{
    Tile targetTile;

    for(int i = (size - 1); i >= 0; i--){
        if(elements[i].contains(x,y)){
            targetTile = elements[i];

            for(int j = (i + 1); j < size; j++){
                elements[j - 1] = elements[j];
            }

            elements[size - 1] = targetTile;
            return;
        }
    }
}

void TileList::lower(int x, int y)
{
    Tile targetTile;
    bool haveTargetTile = false;

    for(int i = size-1; i >= 0; i--){
        if (haveTargetTile){
            elements[i+1] = elements[i];
        }
        else if(elements[i].contains(x,y)){
            targetTile = elements[i];
            haveTargetTile = true;
        }
    }
    if (haveTargetTile){
        elements[0] = targetTile;
    }

}

void TileList::remove(int x, int y)
{
    for(int i = (size - 1); i >= 0; i--){
        if(elements[i].contains(x,y)){
            for(int j = (i + 1); j < size; j++){
                elements[j - 1] = elements[j];
            }
            size--;
            return;
        }
    }
}

void TileList::removeAll(int x, int y)
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
