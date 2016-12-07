/*
 * TDDD86 Trailblazer
 * This file declares functions to perform drawing in the graphical user
 * interface (GUI).
 * See gui.cpp for implementation of each function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp, Keith Schwarz, et al
 * Slight modifications by Tommy Farnqvist
 */

#ifndef _gui_h
#define _gui_h

#include "gevents.h"
#include "grid.h"
#include "types.h"

/*
 * An enumerated type tracking what type of world is currently selected so that
 * we can determine which clicked locations are legal.
 */
enum WorldType {
    TERRAIN_WORLD,
    MAZE_WORLD
};

/*
 * An enumerated type representing how large the world is, categorized as one
 * of three different size classes.
 */
enum WorldSize {
    TINY_WORLD,
    SMALL_WORLD,
    MEDIUM_WORLD,
    LARGE_WORLD,
    HUGE_WORLD
};

/*
 * The UI is a state machine that can be in one of four states:
 *
 *  1. Fresh:  Nothing has been done yet.
 *  2. Marked: One location has been selected.
 *  3. Drawn:  Both locations have been selected, and a path is drawn.
 *
 * This enumerated type stores this information.
 */
enum UIState {
    FRESH,
    MARKED,
    DRAWN
};

/*
 * A utility struct that bundles together the state of the world.
 */
struct State {
    Grid<double> world;    // The world.
    WorldType worldType;   // The type of world.
    UIState uiState;       // Which state we're in.
};

/*
 * Highlights the specified cell in the world the given color, which must be
 * either GRAY, YELLOW, or GREEN.
 */
void colorCell(Grid<double>& world, TBLoc loc, Color locColor);

/*
 * Initializes state of the GUI subsystem.
 */
void initializeGUI();

/*
 * Reacts to an action event in the window.
 */
void processActionEvent(GActionEvent e);

/*
 * Reacts to a mouse event in the window.
 */
void processMouseEvent(GMouseEvent e);

#endif
