/**
 * TDDD86 Robots
 * This file contains the implementation of the QResetButton class.
 */

#include "qresetbutton.h"

QResetButton::QResetButton( QWidget* parent) : QLabel(parent) {
    setText("Try again!");
    setAlignment(Qt::AlignCenter);
    setStyleSheet("QResetButton { background-color: rgb(143,122,102); border-radius: 10px; font: bold; color: white; }");
}

void QResetButton::mousePressEvent(QMouseEvent* event) {
    if (event) {
        emit clicked();
    }
}
