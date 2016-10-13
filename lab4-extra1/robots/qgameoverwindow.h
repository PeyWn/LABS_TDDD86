/**
 * TDDD86 Robots
 * This file contains the declaration of the QGameOverWindow class.
 * See qgameoverwindow.cpp for implementation of each member.
 */

#ifndef QGAMEOVERWINDOW_H
#define QGAMEOVERWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include "qresetbutton.h"

class QGameOverWindow : public QWidget
{
    Q_OBJECT
public:
    explicit QGameOverWindow(QWidget *parent = 0);
    ~QGameOverWindow();

    QResetButton* getResetBtn() const;

signals:

public slots:

private:

    QVBoxLayout *layout = nullptr;
    QLabel* gameover = nullptr;
    QResetButton* reset = nullptr;
};

#endif // QGAMEOVERWINDOW_H
