/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QColor>

// GUI constants
const int MIN_X = 0;
const int MIN_Y = 0;
const int MAX_X = 39;
const int MAX_Y = 39;
const int UNIT_WIDTH = 10;
const int UNIT_HEIGHT = 10;
static const int SCENE_WIDTH = 400;
static const int SCENE_HEIGHT = 400;
const int JUNK_RADIUS = 9;
const QColor JUNK_COLOR = QColor(0, 0, 0);
const QColor ROBOT_COLOR = QColor(245, 95, 60);
const QColor HERO_COLOR = QColor(237, 207, 114);

// game constants
const int MIN_ROBOTS = 10;
const int MAX_ROBOTS = 80;
const int ROBOTS_INC = 5;	// Additional robots per round
const int POINTS_PER_ROBOT = 10;
const int WAIT_BONUS = 20;	// Per robot

#endif // CONSTANTS_H
