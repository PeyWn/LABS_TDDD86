/**
 * TDDD86 Robots
 * This file contains the declaration of the QResetButton class.
 * See qresetbutton.cpp for implementation of each member.
 */

#ifndef QRESETBUTTON_H
#define QRESETBUTTON_H

#include <QLabel>

class QResetButton : public QLabel
{
    Q_OBJECT
public:
    QResetButton( QWidget* parent = 0);

signals:
    void clicked();

public slots:

protected:
    void mousePressEvent(QMouseEvent* event);

};

#endif // QRESETBUTTON_H
