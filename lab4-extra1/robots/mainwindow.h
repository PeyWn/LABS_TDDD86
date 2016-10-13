/**
 * TDDD86 Robots
 * This file contains the declaration of the mainwindow class.
 * See mainwindow.cpp for implementation of each member.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QKeyEvent>
#include "qgameoverwindow.h"
#include "constants.h"
#include "GameState.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

protected:
    void keyPressEvent(QKeyEvent *k);

public slots:
    void resetGame();

private:
    QVBoxLayout* mainLayout = nullptr;         // main layout
    QHBoxLayout* labelLayout = nullptr;        // label layout
    QGraphicsView* view = nullptr;             // playing field widget
    QGraphicsScene* scene = nullptr;           // scene for playing field
    QLabel* scoreLabel = nullptr;              // score widget
    QLabel* levelLabel = nullptr;              // level widget
    QGameOverWindow gameOverWindow;  // game over widget

    int score = 0;                                   // current score
    int level = 1;                                   // current level
    int numberOfRobots = MIN_ROBOTS;                 // current no. of robots
    GameState gameState = GameState(numberOfRobots); // current state of game
    bool gameOver = false;

    // private helpers
    bool tryMove(Hero hero, const Point &point);
    void processMove(bool waiting);
    bool outsideBorder(const Point &point) const;
    void displayScore() const;
    void displayLevel() const;
};

#endif // MAINWINDOW_H
