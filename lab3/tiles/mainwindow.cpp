/*
 * TDDD86 Tiles
 * This file contains the implementation of the mainwindow class.
 * See mainwindow.h for comments about each member.
 */

#include <QPointF>
#include <random>
#include "mainwindow.h"
#include "Tile.h"
#include "TileList.h"

// constants
static const int SCENE_WIDTH = 600;
static const int SCENE_HEIGHT = 400;
static const int MIN_SIZE = 20;
static const int MAX_SIZE = 100;
static const int MIN_COLOR = 50;
static const int MAX_COLOR = 255;
static const int TILE_COUNT = 50;
static const bool RANDOM = true;   // set to false for repeatable output

MainWindow::MainWindow(QWidget *parent) :
    QGraphicsView(parent)
{
    // setup graphical window
    scene = new QGraphicsScene();
    this->setScene(scene);
    this->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    this->setFixedSize(sizeHint());

    // place several random tiles on the screen
    this->setWindowTitle("Tiles");
    for (int i = 0; i < TILE_COUNT; ++i) {
        addRandomTile(tlist);
    }
    tlist.drawAll(scene);
}

/*
 * Listens to mouse press events from the graphical subsystem,
 * and handles the events appropriately:
 *   - A left-click is a 'raise' action.
 *   - A shift-left-click is a 'lower' action.
 *   - A right-click (or ctrl-click) is a 'delete' action.
 *   - A shift-right-click (or shift-ctrl-click) is a 'delete all' action.
 */
void MainWindow::mousePressEvent(QMouseEvent *e)
{
    QPointF pt = mapToScene(e->pos());
    bool modified = false;
    switch (e->button()) {
    case Qt::LeftButton:
        switch (e->modifiers()) {
        case Qt::NoModifier:
            tlist.raise(pt.x(), pt.y());
            modified = true;
            break;
        case Qt::ShiftModifier:
                tlist.lower(pt.x(), pt.y());
                modified  = true;
            break;
        }
        break;
    case Qt::RightButton:
        switch (e->modifiers()) {
        case Qt::NoModifier:
            tlist.remove(pt.x(), pt.y());
            modified = true;
            break;
        case Qt::ShiftModifier:
                tlist.removeAll(pt.x(), pt.y());
                modified = true;
            break;
        }
        break;
    default:
        QGraphicsView::mousePressEvent(e);
    }
    if (modified)
        scene->clear();
        tlist.drawAll(scene);
}

/*
 * Listens to key press events from the graphical subsystem,
 * and handles the events appropriately:
 *   - 'N' creates a new random tile.
 */
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_N:
        addRandomTile(tlist);
        scene->clear();
        tlist.drawAll(scene);
        break;
    default:
        QGraphicsView::keyPressEvent(e);
    }
}


/*
 * Creates a new tile with a random x/y position, width, height, and color,
 * and adds it to the given tile list.
 */
void MainWindow::addRandomTile(TileList &tlist)
{
    Tile tile;
    std::random_device rd;

    // possibly use the same random numbers every time for testing
    static std::default_random_engine e(RANDOM ? rd() : 42);

    std::uniform_int_distribution<unsigned> u1(MIN_COLOR, MAX_COLOR);
    std::uniform_int_distribution<unsigned> u2(MIN_SIZE, MAX_SIZE);
    tile.width = u2(e);
    tile.height = u2(e);
    std::uniform_int_distribution<unsigned> u3(0, SCENE_WIDTH - tile.width - 1);
    std::uniform_int_distribution<unsigned> u4(0, SCENE_HEIGHT - tile.height - 1);
    tile.x = u3(e);
    tile.y = u4(e);
    tile.r = u1(e);
    tile.g = u1(e);
    tile.b = u1(e);
    tlist.addTile(tile);
}
