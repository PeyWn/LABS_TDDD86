/**
 * TDDD86 Robots
 * This client program contains the 'main' function to set up the overall
 * program's graphical user interface.
 * Your code should work properly with an unmodified version of this file.
 */

#include <QApplication>
#include "mainwindow.h"
#include "utilities.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    rand_seed();     // seed random number generator

    MainWindow view; // create main window
    view.show();     // display main window

    return a.exec(); // start Qt event loop
}
