/*
 * TDDD86 Trailblazer
 * This file implements functions to perform drawing in the graphical user
 * interface (GUI).
 * This file also contains the main function to launch the program.
 * See trailblazergui.h for documentation of each public function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * Author: Marty Stepp, Keith Schwarz, et al
 * Slight modifications by Tommy Farnqvist
 */

#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

// code for setting a large stack size on non-Windows GCC platforms
#ifndef _WIN32
#include <sys/resource.h>
#include <stdint.h>
#endif

#include "error.h"
#include "filelib.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gwindow.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "adapter.h"
#include "costs.h"
#include "trailblazer.h"
#include "trailblazergui.h"
using namespace std;

// internal constants
const int MIN_DELAY = 0;
const int MAX_DELAY = 2000;
static int animationDelay = 0;
static bool pathSearchInProgress = false;

// various UI strings
const string kSelectedLocationColor("RED");
const string kPathColor("RED");
const string kBackgroundColor("Black");
const string GUI_STATE_FILE = "trailblazer-gui-state.sav";
const bool SHOULD_SAVE_GUI_STATE = true;
const bool RANDOM_OPTION_ENABLED = false;

/*
 * The size, in pixels, of the displayed world.
 * Number of padding pixels between the border of the window and the the
 * start of the content.
 * Constants controlling the amount we should adjust the size of the window
 * to reflect the extra space used up by the border and controls.
 */
const int kDisplayWidth = 600;
const int kDisplayHeight = 600;
const int kMargin = 5;
const int kWidthAdjustment  =  0;
const int kHeightAdjustment = 75;

/*
 * Maximum number of rows or columns we allow in a world.    This is mostly a
 * safety feature to prevent an OOM on a malformed input file.
 */
const int kMaxRows = 400;
const int kMaxCols = 400;

/*
 * Constants controlling the number of rows/cols in the different sizes of
 * worlds.  The indices into these arrays are controlled by the enumerated
 * type WorldSize.
 */
const int kTerrainNumRows[] = { 10, 33, 65, 129, 257 };
const int kTerrainNumCols[] = { 10, 33, 65, 129, 257 };
const int kMazeNumRows[]    = {  5, 10, 30,  80, 160 };
const int kMazeNumCols[]    = {  5, 10, 30,  80, 160 };

/*
 * Constants reflecting the colors that are used to highlight cells different
 * colors.  This is an array of arrays, where each array contains RGB triplets
 * for the collors.  The ordering here should be consistent with the ordering
 * of the Color type.
 */
const int kColorMultipliers[6][3] = {
    {   0,   0,   0 },   // UNCOLORED
    { 255, 255, 255 },   // WHITE
    { 192, 192, 192 },   // GRAY
    { 255, 255,   0 },   // YELLOW
    {   0, 255,   0 },   // GREEN
    { 255,   0,   0 }    // RED
};


// Internal global variables

static GWindow* gWindow = NULL;
static GChooser* gWorldChooser = NULL;
static GChooser* gAlgorithmList = NULL;
static GSlider* gDelaySlider = NULL;
static GRect* gFirstSelected = NULL;      // which cells were seletected so far
static GRect* gSecondSelected = NULL;
static GTextField* gPositionField = NULL;
static string gPositionFieldText = " r???c???";
static Grid<Color> gMarked;               // which cells have been colored by the user
static Grid<double> gMarkedValues;        // the values we've marked them with
static Vector<GLine*> gHighlightedPath;   // all highlighting lines
static TBLoc gStartLocation;              // their corresponding locations
static TBLoc gEndLocation;
static double gPixelsPerWidth;            // width of each cell
static double gPixelsPerHeight;           // height of each cell
static State state;


// internal function prototypes (see implementations for documentation)

static void colorLocation(TBLoc loc, double value, Color locColor);
static TBLoc coordToLoc(double x, double y);
static void drawWorld(Grid<double>& world);
static void fillRect(int x, int y, int width, int height, string color);
static void findMidpoint(TBLoc loc, double& xc, double& yc);
static Set<string> getFiles(string substr);
//static WorldSize getWorldSize(string worldFile);
static void intro();
static bool loadGUIState();
static void locToCoord(TBLoc loc, double& x, double& y);
static bool readWorldFile(ifstream& input, Grid<double>& world, WorldType& worldType);
static bool regenerateWorld(Grid<double>& world, WorldType& worldType);
static bool registerClick(Grid<double>& world, double x, double y, WorldType worldType);
static void removeAndDelete(GObject* object);
static void removeMarkedSquares();
static void removeOverlays();
static void restoreWorldDisplay();
static void runSearch(State& state);
static void runShortestPath(const Grid<double>& world, WorldType worldType,
                            TBLoc start, TBLoc end, Vector<TBLoc>& path, double& pathCost);
static bool saveGUIState();
static void uncolorSquares();
static void updateDelay(bool forbidZero = false);
static string valueToColor(double value, Color locColor);
static void worldUpdated(Grid<double>& world, WorldType worldType);


// function implementations

#ifndef _WIN32
/*
 * This function increases the system stack size on Unixy platforms (Linux+Mac),
 * making it possible to make many nested calls of depth-first search without
 * crashing the system.
 * Windows MinGW systems set their stack size through compiler/linker flags
 * that are set in the project's .pro file.
 */
static void setStackSize(const rlim_t stackSize) {
#if defined(__USE_LARGEFILE64)
    // 64-bit version
    rlimit64 rl;
    int result = getrlimit64(RLIMIT_STACK, &rl);
#else
    // 32-bit version
    rlimit rl;
    int result = getrlimit(RLIMIT_STACK, &rl);
#endif
    if (result == 0) {
        if (rl.rlim_cur < stackSize || rl.rlim_max < stackSize) {
            rl.rlim_cur = stackSize;
            rl.rlim_max = stackSize;
#if defined(__USE_LARGEFILE64)
            // 64-bit version
            result = setrlimit64(RLIMIT_STACK, &rl);
#else
            result = setrlimit(RLIMIT_STACK, &rl);
#endif
            if (result != 0) {
                cerr << "Warning: Unable to increase system stack size to "
                     << stackSize << endl;
            }
        }
    }
}
#endif

/*
 * Main program.
 */
int main() {
#ifndef _WIN32
    setStackSize(100 * 1024 * 1024);   // increase function call stack
#endif

    setConsoleEcho(true);
    setConsolePrintExceptions(true);

    initializeGUI();

    // Main event loop to process events as they happen.
    while (true) {
        GEvent e = waitForEvent(ACTION_EVENT | MOUSE_EVENT);
        if (e.getEventType() == MOUSE_CLICKED || e.getEventType() == MOUSE_MOVED) {
            processMouseEvent(GMouseEvent(e));
        } else if (e.getEventClass() == ACTION_EVENT) {
            processActionEvent(GActionEvent(e));
        }
    }
    return 0;
}

/*
 * (public function)
 * Colors the specified cell in the world the indicated color.
 */
void colorCell(Grid<double>& world, TBLoc loc, Color locColor) {
    colorLocation(loc, world[loc.row][loc.col], locColor);
    gMarked[loc.row][loc.col] = locColor;

    // possibly delay and then repaint the square, for animation
    if (animationDelay > 0) {
        gWindow->repaint();

        // while I'm here, check if delay updated
        updateDelay(true);

        pause(animationDelay);
    }
}

/*
 * (public function)
 * Initializes state of the GUI subsystem.
 */
void initializeGUI() {
    // Calculate the intended width and height of the window based on the content
    // area size, the margin size, and the adjustment amount.
    int windowWidth = kDisplayWidth + 2 * kMargin + kWidthAdjustment;
    int windowHeight = kDisplayHeight + 2 * kMargin + kHeightAdjustment;

    gWindow = new GWindow(windowWidth, windowHeight);
    gWindow->setWindowTitle("TDDD86 Trailblazer");

    // Add the algorithms list.
    gAlgorithmList = new GChooser();
    gAlgorithmList->addItem("Depth-first Search");
    gAlgorithmList->addItem("Breadth-first Search");
    gAlgorithmList->addItem("Dijkstra's Algorithm");
    gAlgorithmList->addItem("A* Search");
    gWindow->addToRegion(gAlgorithmList, "NORTH");

    gWindow->addToRegion(new GLabel("Delay:"), "NORTH");
    gDelaySlider = new GSlider(MIN_DELAY, MAX_DELAY, MIN_DELAY);
    gWindow->addToRegion(gDelaySlider, "NORTH");
    gWindow->addToRegion(new GButton("Run"), "NORTH");
    gWindow->addToRegion(new GButton("Clear"), "NORTH");
    gPositionField = new GTextField(7);
    gPositionField->setText(gPositionFieldText);
    gWindow->addToRegion(gPositionField, "NORTH");

    // Add in the list of existing world files.
    gWorldChooser = new GChooser();
    Set<string> worldFiles = getFiles("maze") + getFiles("terrain");
    foreach (string worldFile in worldFiles) {
        gWorldChooser->addItem(worldFile);
    }
    if (RANDOM_OPTION_ENABLED) {
        gWorldChooser->addItem("Random Maze (tiny)");
        gWorldChooser->addItem("Random Maze (small)");
        gWorldChooser->addItem("Random Maze (medium)");
        gWorldChooser->addItem("Random Maze (large)");
        gWorldChooser->addItem("Random Maze (huge)");
        gWorldChooser->addItem("Random Terrain (tiny)");
        gWorldChooser->addItem("Random Terrain (small)");
        gWorldChooser->addItem("Random Terrain (medium)");
        gWorldChooser->addItem("Random Terrain (large)");
        gWorldChooser->addItem("Random Terrain (huge)");
    }

    gWindow->addToRegion(new GLabel("World:"), "SOUTH");
    gWindow->addToRegion(gWorldChooser, "SOUTH");
    gWindow->addToRegion(new GButton("Load"), "SOUTH");
    gWindow->addToRegion(new GButton("Exit"), "SOUTH");

    intro();
    if (SHOULD_SAVE_GUI_STATE) {
        loadGUIState();
    }

    if (!regenerateWorld(state.world, state.worldType)) {
        error("Cannot set up initial world properly!");
    }
    state.uiState = FRESH;
    drawWorld(state.world);
}

/*
 * (public function)
 * Reacts to an action event in the window.
 */
void processActionEvent(GActionEvent e) {
    string cmd = e.getActionCommand();
    if (cmd == "Load") {
        // Want to load a new world?  Try to do so and update the UI accordingly.
        if (regenerateWorld(state.world, state.worldType)) {
            drawWorld(state.world);
            state.uiState = FRESH;
        }
    } else if (cmd == "Run") {
        // Rerunning the search is only possible if we already did a search.
        if (state.uiState == DRAWN) {
            uncolorSquares();
            removeOverlays();
            runSearch(state);
        } else {
            cout << "Cannot rerun a search; no search has been done." << endl;
        }
    } else if (cmd == "Clear") {
        // Clearing the display just sets us back to the fresh state.
        restoreWorldDisplay();
        state.uiState = FRESH;
    } else if (cmd == "Exit") {
        // Trying to quit exits the entire program.
        cout << endl;
        cout << "Exiting." << endl;
        if (SHOULD_SAVE_GUI_STATE) {
            saveGUIState();
        }
        exitGraphics();
    }
}

/*
 * (public function)
 * Reacts to a mouse event in the window.
 */
void processMouseEvent(GMouseEvent e) {
    if (e.getEventType() == MOUSE_CLICKED) {
        switch (state.uiState) {
        case DRAWN:
            // Already have something drawn?  Clear it and pretend we're fresh.
            restoreWorldDisplay();
            state.uiState = FRESH;
            // deliberate: don't break.

        case FRESH:
            // In a fresh state?  Try to select what the user clicked on.
            if (registerClick(state.world, e.getX(), e.getY(), state.worldType)) {
                state.uiState = MARKED;
            }
            break;

        case MARKED:
            // Already marked?  Try to select what the user clicked on, then
            // try to find a path if it worked.
            if (registerClick(state.world, e.getX(), e.getY(),
                              state.worldType)) {
                runSearch(state);
                state.uiState = DRAWN;
            }
        }
    } else if (e.getEventType() == MOUSE_MOVED) {
        // update display of current mouse row/col position to aid testing
        TBLoc loc = coordToLoc(e.getX(), e.getY());
        if (state.world.inBounds(loc.row, loc.col)) {
            string newPositionText = vertexName(loc.row, loc.col, state.world);
            if (newPositionText != gPositionFieldText) {
                gPositionFieldText = newPositionText;
                gPositionField->setText(newPositionText);
            }
        }
    }
}

/*
 * Given a physical screen location, maps it to a logical grid location.
 */
static TBLoc coordToLoc(double x, double y) {
    TBLoc loc;
    loc.row = (int) ((y - kMargin) / gPixelsPerHeight);
    loc.col = (int) ((x - kMargin) / gPixelsPerWidth);
    return loc;
}

/*
 * Colors the given location, which has the specified intensity, the indicated
 * color.
 */
static void colorLocation(TBLoc loc, double value, Color locColor) {
    double x, y;
    locToCoord(loc, x, y);

    fillRect(x, y, gPixelsPerWidth, gPixelsPerHeight,
             valueToColor(value, locColor));
}

/*
 * Given a path, returns the cost of that path.
 */
static double costOf(Vector<TBLoc>& path,
                     const Grid<double>& world,
                     double costFn(TBLoc, TBLoc, const Grid<double>&)) {
    double result = 0.0;
    for (int i = 1; i < path.size(); i++) {
        result += costFn(path[i - 1], path[i], world);
    }
    return result;
}

/*
 * Given a path, draws that path on the screen.
 */
static void drawPath(Vector<TBLoc>& path) {
    for (int i = 1; i < path.size(); i++) {
        // highlight connection between path[i - 1] and path[i]
        double srcX, srcY, dstX, dstY;
        findMidpoint(path[i - 1], srcX, srcY);
        findMidpoint(path[i], dstX, dstY);

        GLine* connection = new GLine(srcX, srcY, dstX, dstY);
        connection->setColor(kPathColor);
        connection->setLineWidth(3.0);
        gWindow->add(connection);
        gHighlightedPath.add(connection);
    }
}

/*
 * Draws the specified grid in the world.
 */
static void drawWorld(Grid<double>& world) {
    if (gWindow == NULL) error("Cannot draw world before window is set up.");

    // This should act as if we are repainting the top-level display, so we
    // will remove all overlays.
    removeMarkedSquares();
    removeOverlays();

    // Recompute the size of a cell in the display.
    gPixelsPerWidth = (double) kDisplayWidth / world.numCols();
    gPixelsPerHeight = (double) kDisplayHeight / world.numRows();

    // Draw the background.
    fillRect(0, 0, kDisplayWidth + 2 * kMargin, kDisplayHeight + 2 * kMargin,
             kBackgroundColor);

    // With the redraw, no locations are marked anymore.
    gMarked.resize(world.numRows(), world.numCols());
    gMarkedValues = world;

    // Draw each cell.
    for (int row = 0; row < world.numRows(); row++) {
        for (int col = 0; col < world.numCols(); col++) {
            TBLoc loc = { row, col };
            double x, y;
            locToCoord(loc, x, y);

            fillRect(x, y, gPixelsPerWidth, gPixelsPerHeight,
                     valueToColor(world[row][col], WHITE));
        }
    }

    // Issue a repaint so that we see the changes.
    gWindow->repaint();
}

/*
 * A convenience wrapper that sets the given color and then fills a rectangle
 * of the given dimensions.  Saves a line of code on each call.
 */
static void fillRect(int x, int y, int width, int height, string color) {
    gWindow->setColor(color);
    gWindow->fillRect(x, y, width, height);
}

/*
 * Given a logical grid location, returns the physical screen coordinates of
 * its midpoint.
 */
static void findMidpoint(TBLoc loc, double& xc, double& yc) {
    locToCoord(loc, xc, yc);
    xc += gPixelsPerWidth / 2;
    yc += gPixelsPerHeight / 2;
}

/*
 * Returns which type of algorithm is currently selected in the drop-down
 * menu.
 */
static AlgorithmType getAlgorithmType() {
    if (gWindow == NULL) error("Window has not yet been initialized.");
    string algorithmLabel = gAlgorithmList->getSelectedItem();

    if (algorithmLabel == "Depth-first Search") {
        return DFS;
    } else if (algorithmLabel == "Breadth-first Search") {
        return BFS;
    } else if (algorithmLabel == "Dijkstra's Algorithm") {
        return DIJKSTRA;
    } else if (algorithmLabel == "A* Search") {
        return A_STAR;
    } else {
        error("Invalid algorithm provided.");
        return DIJKSTRA;
    }
}

/*
 * Returns all files in the current directory that start with the given
 * substring prefix.  Used to find all maze and/or terrain files to display.
 */
static Set<string> getFiles(string substr) {
    substr = toLowerCase(substr);
    Vector<string> files;
    listDirectory(".", files);
    Set<string> result;
    foreach (string file in files) {
        string fileLC = toLowerCase(file);
        if (startsWith(fileLC, substr) && endsWith(fileLC, ".txt")) {
            result.add(file);
        }
    }
    return result;
}

/*
 * Given the contents of the world size selector, returns a WorldSize encoding
 * the desired world size.
 */
//static WorldSize getWorldSize(string worldFile) {
//    string worldLC = toLowerCase(worldFile);
//    if (worldLC.find("tiny") != string::npos) {
//        return TINY_WORLD;
//    } else if (worldLC.find("small") != string::npos) {
//        return SMALL_WORLD;
//    } else if (worldLC.find("medium") != string::npos) {
//        return MEDIUM_WORLD;
//    } else if (worldLC.find("large") != string::npos) {
//        return LARGE_WORLD;
//    } else if (worldLC.find("huge") != string::npos) {
//        return HUGE_WORLD;
//    } else {
//        error("Invalid world size provided.");
//        return SMALL_WORLD;
//    }
//}

/*
 * Prints an introductory text message on the screen.
 */
static void intro() {
    cout << "Welcome to TDDD86 Trailblazer!" << endl;
    cout << "This program searches for paths through graphs representing mazes" << endl;
    cout << "and rocky terrains.  It demonstrates several graph algorithms for" << endl;
    cout << "finding paths, such as depth-first search (DFS), breadth-first" << endl;
    cout << "search (BFS), Dijkstra's Algorithm, and A* search." << endl;
}

/*
 * Restores the previously saved GUI state, including which algorithm is
 * currently selected, the animation delay, and the world file chosen.
 * If the saved state does not exist or is corrupt, returns false and
 * uses a default initial state.
 */
static bool loadGUIState() {
    ifstream input;
    input.open(GUI_STATE_FILE.c_str());
    if (input.fail()) {
        return false;
    }
    string algorithm;
    getline(input, algorithm);
    if (input.fail()) {
        return false;
    }

    string line;
    getline(input, line);
    if (input.fail()) {
        return false;
    }
    int delay = stringToInteger(line);

    string worldFile;
    getline(input, worldFile);
    if (input.fail()) {
        return false;
    }
    input.close();

    // delete the save state file in case there is a crash loading a world
    deleteFile(GUI_STATE_FILE);

    gAlgorithmList->setSelectedItem(algorithm);
    gDelaySlider->setValue(delay);
    gWorldChooser->setSelectedItem(worldFile);
    return true;
}

/*
 * Given a logical grid location, maps it to a physical screen location.
 */
static void locToCoord(TBLoc loc, double& x, double& y) {
    x = loc.col * gPixelsPerWidth + kMargin;
    y = loc.row * gPixelsPerHeight + kMargin;
}

/*
 * Tries to read a world file from the specified stream.  On success, returns
 * true and updates the input parameters to mark the type of the world and
 * the world contents.  On failure, returns false, but may still modify the
 * input parameters.
 */
static bool readWorldFile(ifstream& input, Grid<double>& world, WorldType& worldType) {
    try {
        // Enable exceptions on the stream so that we can handle errors using try-
        // catch rather than continuously testing everything.
        input.exceptions(ios::failbit | ios::badbit);

        // The file line of the file identifies the type, which should be either
        // "terrain" or "maze."
        string type;
        input >> type;

        if (type == "terrain") {
            worldType = TERRAIN_WORLD;
        } else if (type == "maze") {
            worldType = MAZE_WORLD;
        } else {
            cerr << "world file does not contain type (terrain/maze) as first line." << endl;
            return false;
        }

        // Read the size of the world.
        int numRows, numCols;
        input >> numRows >> numCols;

        if (numRows <= 0 || numCols <= 0 ||
                numRows >= kMaxRows || numRows >= kMaxCols) {
            cerr << "world file contains invalid number of rows/cols: "
                 << numRows << "," << numCols << endl;
            return false;
        }

        world.resize(numRows, numCols);

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                double value;
                input >> value;
                if (input.fail()) {
                    cerr << "Illegal input file format; row #" << (row+1)
                         << "does not contain " << numCols << " valid numbers" << endl;
                    return false;
                }

                // Validate the input based on the type of world.
                if (worldType == MAZE_WORLD) {
                    if (value != kMazeWall && value != kMazeFloor) {
                        cerr << "world file contains invalid square value of " << value
                             << ", must be " << kMazeFloor << " or " << kMazeWall << endl;
                        return false;
                    }
                } else {  // worldType == TERRAIN_WORLD
                    if (value < 0.0 || value > 1.0) {
                        cerr << "world file contains invalid terrain value of " << value
                             << ", must be 0.0 - 1.0" << endl;
                        return false;
                    }
                }
                world[row][col] = value;
            }
        }

        return true;
    } catch (...) {
        // Something went wrong, so report an error.
        cerr << "exception thrown while reading world file" << endl;
        return false;
    }
}

/*
 * Generates a new world based on the user's preferences.
 */
static bool regenerateWorld(Grid<double>& world, WorldType& worldType) {
    string worldFile = gWorldChooser->getSelectedItem();
    // code for generating random worlds commented out for this quarter
//    if (startsWith(worldFile, "Random")) {
//        // To make this gracefully handle exceptions, we'll make a dummy world and
//        // world type that we'll set up out here.
//        Grid<double> newWorld;
//        WorldSize worldSize = getWorldSize(worldFile);
//        WorldType newType = (worldFile.find("errain") == string::npos ? MAZE_WORLD : TERRAIN_WORLD);

//        // Based on the type of world, generate a different world of an appropriate size.
//        if (newType == TERRAIN_WORLD) {
//            int numRows = kTerrainNumRows[worldSize];
//            int numCols = kTerrainNumCols[worldSize];
//            newWorld = generateRandomTerrain(numRows, numCols);
//        } else if (newType == MAZE_WORLD) {
//            int numRows = kMazeNumRows[worldSize];
//            int numCols = kMazeNumCols[worldSize];

//            // The default student code throws an exception, which we must handle.
//            try {
//                // The maze's logical number of rows and columns is not the same as its
//                // physical number of rows and columns in the grid.  In particular, an
//                // m x n maze has size (2m - 1) x (2n - 1), so we rescale the size of the
//                // maze based on the number of logical rows and columns we want.
//                newWorld = generateRandomMaze(numRows / 2 + 1, numCols / 2 + 1);
//            } catch (const ErrorException& e) {
//                cout << e.what() << endl;
//                return false;
//            }
//        } else {
//            error("Invalid world type provided.");
//        }

//        world = newWorld;
//        worldType = newType;
//        worldUpdated(world, worldType);
//        return true;
//    } else {
        // not random (most common); load a pre-defined maze or terrain input file
        cout << endl;
        cout << "Loading world from " << worldFile << " ..." << endl;
        ifstream input;
        input.open(worldFile.c_str());
        if (input.fail()) {
            error(string("Unable to open input file ") + worldFile);
            return false;
        }

        // Try reading in the world file.  If we can't, report an error.
        Grid<double> newWorld;
        WorldType newWorldType;
        if (!readWorldFile(input, newWorld, newWorldType)) {
            cerr << worldFile << " is not a valid world file." << endl;
            return false;
        }

        world = newWorld;
        worldType = newWorldType;
        worldUpdated(world, worldType);
        return true;
//    }
}

/*
 * Registers that a click occurred, storing the location appropriately and
 * updating the graphics.  If the click was on an invalid location, returns
 * false.  If the click was at a valid location, returns true.
 */
static bool registerClick(Grid<double>& world, double x, double y,
                          WorldType worldType) {
    if (gWindow == NULL) {
        error("Window has not yet been initialized.");
    }
    if (gFirstSelected != NULL && gSecondSelected != NULL) {
        error("Two tiles have already been selected.");
    }

    // Determine where we clicked and make sure it's in-bounds.
    TBLoc loc = coordToLoc(x, y);
    if (!world.inBounds(loc.row, loc.col) ||
            (worldType == MAZE_WORLD && world[loc.row][loc.col] == kMazeWall)) {
        return false;
    }

    // Add the selection to the display.
    double selectionX, selectionY;
    locToCoord(loc, selectionX, selectionY);
    GRect* selection = new GRect(selectionX, selectionY, gPixelsPerWidth, gPixelsPerHeight);
    selection->setLineWidth(2.0);
    selection->setColor(kSelectedLocationColor);
    gWindow->add(selection);

    // Store the selection appropriately based on whether this is the first or
    // second click.
    if (gFirstSelected == NULL) {
        gFirstSelected = selection;
        gStartLocation = loc;
    } else {
        gSecondSelected = selection;
        gEndLocation = loc;
    }

    return true;
}

/*
 * Utility function to remove a GObject from the display window and deallocate
 * it.
 */
static void removeAndDelete(GObject* object) {
    if (object != NULL) {
        gWindow->remove(object);
        delete object;
    }
}

/*
 * Removes the highlighted squares from the display.
 */
static void removeMarkedSquares() {
    removeAndDelete(gFirstSelected);
    removeAndDelete(gSecondSelected);
    gFirstSelected = gSecondSelected = NULL;
}

/*
 * Removes the line overlays.
 */
static void removeOverlays() {
    // Clear the lines drawn from the path.
    for (int i = 0; i < gHighlightedPath.size(); i++) {
        removeAndDelete(gHighlightedPath[i]);
    }
    gHighlightedPath.clear();
}

/*
 * Clears all cells that are currently selected and deselects any currently-
 * selected squares.
 */
static void restoreWorldDisplay() {
    if (gWindow == NULL) {
        error("Window has not yet been initialized.");
    }

    uncolorSquares();
    removeMarkedSquares();
    removeOverlays();

    // Repaint the window so the changes show.
    gWindow->repaint();
}

/*
 * Runs the currently selected graph path-searching algorithm on the current
 * world, then displays information about the path that was found.
 */
static void runSearch(State& state) {
    try {
        updateDelay();
        Vector<TBLoc> path;
        double pathCost;
        runShortestPath(state.world, state.worldType, gStartLocation,
                        gEndLocation, path, pathCost);
        cout << "Path length: " << path.size() << endl;
        cout << "Path cost: " << pathCost << endl;
        int greenGray = 0;
        int yellow = 0;
        for (int r = 0; r < gMarked.numRows(); r++) {
            for (int c = 0; c < gMarked.numCols(); c++) {
                Color color = gMarked.get(r, c);
                if (color == GREEN || color == GRAY) {
                    greenGray++;
                } else if (color == YELLOW) {
                    yellow++;
                }
            }
        }
        cout << "Locations visited: " << greenGray << endl;

    } catch (const ErrorException& e) {
        cout << e.what() << endl;
    }
}

/*
 * Computes the shortest path between the start and end locations, displaying
 * it on the screen and returning its length.
 */
static void runShortestPath(const Grid<double>& world, WorldType worldType,
                            TBLoc start, TBLoc end,
                            Vector<TBLoc>& path, double& pathCost) {
    AlgorithmType algType = getAlgorithmType();

    // Determine which cost/heuristic functions to use.
    double (*costFn)(TBLoc, TBLoc, const Grid<double>&);
    double (*hFn)(TBLoc, TBLoc, const Grid<double>&);

    if (worldType == TERRAIN_WORLD) {
        costFn = terrainCost;
        hFn = terrainHeuristic;
    } else if (worldType == MAZE_WORLD) {
        costFn = mazeCost;
        hFn = mazeHeuristic;
    } else {
        error("Unknown world type.");
    }

    // Invoke the student's shortestPath function to find out the cost of the path.
    pathSearchInProgress = true;
    path = shortestPath(start, end, world, costFn, hFn, algType);
    pathSearchInProgress = false;

    if (path.isEmpty()) {
        cout << "Warning: Returned path is empty." << endl;
    } else if (path[0] != start) {
        cout << "Warning: Start of path is not the start location." << endl;
    } else if (path[path.size() - 1] != end) {
        cout << "Warning: End of path is not the end location." << endl;
    }

    drawPath(path);
    pathCost = costOf(path, world, costFn);
}

/*
 * Saves the GUI's current state, including which algorithm is
 * currently selected, the animation delay, and the world file chosen.
 * If the saved state does not exist or is corrupt, returns false and
 * uses a default initial state.
 */
static bool saveGUIState() {
    string algorithm = gAlgorithmList->getSelectedItem();
    int delay = gDelaySlider->getValue();
    string worldFile = gWorldChooser->getSelectedItem();
    ofstream output;
    output.open(GUI_STATE_FILE.c_str());
    if (output.fail()) {
        return false;
    }
    output << algorithm << endl;
    output << delay << endl;
    output << worldFile << endl;
    if (output.fail()) {
        return false;
    }
    output.flush();
    output.close();
    return true;
}

/*
 * Reads the delay slider and uses its value to decide on an animation delay.
 */
static void updateDelay(bool forbidZero) {
    int delay = gDelaySlider->getValue();
    double percent = 100.0 * delay / MAX_DELAY;
    // convert scale so delays don't increase so rapidly
    if (percent == 0.0) {
        delay = forbidZero ? 1 : 0;
    } else if (percent <= 10) {
        delay = MAX_DELAY / 1000;
    } else if (percent <= 20) {
        delay = MAX_DELAY / 500;
    } else if (percent <= 30) {
        delay = MAX_DELAY / 200;
    } else if (percent <= 40) {
        delay = MAX_DELAY / 100;
    } else if (percent <= 50) {
        delay = MAX_DELAY / 50;
    } else if (percent <= 60) {
        delay = MAX_DELAY / 25;
    } else if (percent <= 70) {
        delay = MAX_DELAY / 10;
    } else if (percent <= 80) {
        delay = MAX_DELAY / 5;
    } else if (percent <= 90) {
        delay = MAX_DELAY / 2;
    } else {  // percent > 90
        delay = MAX_DELAY;
    }

    animationDelay = delay;
}

/*
 * Given an elevation and a color, determines what #rrggbb format color to use
 * to draw it.
 */
static string valueToColor(double value, Color locColor) {
    // To make non-gray colors show up better, we artificially cap the lowest
    // possible intensity at 0.2, rather than 0.0.  This next step is a linear
    // remapping of the range [0, 1] to [0.2, 1].
    if (locColor != WHITE) {
        value = 0.8 * value + 0.2;
    }

    stringstream hexValue;
    hexValue << "#" << hex << setfill('0');
    for (int i = 0; i < 3; i++) {
        int intensity = int(value * kColorMultipliers[locColor][i]);
        hexValue << setw(2) << intensity;
    }

    return hexValue.str();
}

/*
 * Restores all squares to their original colors.
 */
static void uncolorSquares() {
    // Restore all colored cells to their original condition.
    for (int row = 0; row < gMarked.numRows(); row++) {
        for (int col = 0; col < gMarked.numCols(); col++) {
            if (gMarked[row][col] != UNCOLORED && gMarked[row][col] != WHITE) {
                TBLoc loc = { row, col };
                colorLocation(loc, gMarkedValues[row][col], WHITE);

                // Unmark this cell; it's no longer colored.
                gMarked[row][col] = UNCOLORED;
            }
        }
    }
}

/*
 * Called anytime the current world is changed, so that we can update the
 * cache of Grid -> Graph.
 */
static void worldUpdated(Grid<double>& world, WorldType worldType) {
    flushWorldCache();
    if (worldType == TERRAIN_WORLD) {
        ensureWorldCache(world, terrainCost);
    } else if (worldType == MAZE_WORLD) {
        ensureWorldCache(world, mazeCost);
    } else {
        error("Unknown world type.");
    }
}
