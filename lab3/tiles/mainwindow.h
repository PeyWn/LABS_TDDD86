/*
 * TDDD86 Tiles
 * This file contains the declaration of the mainwindow class.
 * See mainwindow.cpp for implementation of each member.
 * Your code should work properly with an unmodified version of this file.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include "TileList.h"

class MainWindow : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *k);

private:
    void addRandomTile(TileList& tlist);

    QGraphicsScene* scene;
    TileList tlist;
};

#endif // MAINWINDOW_H
