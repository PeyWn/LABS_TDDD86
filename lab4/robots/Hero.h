/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef HERO_H
#define HERO_H

#include "Unit.h"
#include <QGraphicsScene>

class Hero : public Unit {
public:
    Hero();

    /*
    * Draws this hero onto the given QGraphicsScene.
    */
    void draw(QGraphicsScene *scene) const;
};

#endif // HERO_H
