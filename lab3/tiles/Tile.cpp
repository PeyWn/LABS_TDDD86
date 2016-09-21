/*
 * TDDD86 Tiles
 * This file contains the implementation of the Tile structure.
 * See Tile.h for comments about each member.
 * Your code should work properly with an unmodified version of this file.
 */

#include <QGraphicsRectItem>
#include "Tile.h"

bool Tile::contains(int x, int y) const {
    return this->x <= x && x < this->x + this->width &&
            this->y <= y && y < this->y + this->height;
}

void Tile::draw(QGraphicsScene *scene) const {
    QGraphicsRectItem *item = new QGraphicsRectItem(x, y, width, height);
    item->setBrush(QBrush(QColor(r, g, b)));
    scene->addItem(item);
}

string Tile::toString() const {
    return "Tile{x=" + to_string(x) + ",y=" + to_string(y)
            + ",width=" + to_string(width) + ",height=" + to_string(height)
            + ",r=" + to_string(r) + ",g=" +to_string (g) + ",b="  + to_string(b) + "}";
}

ostream& operator<<(ostream& out, const Tile& tile) {
    out << tile.toString();
    return out;
}
