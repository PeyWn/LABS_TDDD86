TEMPLATE = app
QT += widgets
HEADERS += mainwindow.h \
    Tile.h \
    TileList.h
SOURCES += mainwindow.cpp \
    tilemain.cpp \
    Tile.cpp \
    TileList.cpp
QMAKE_CXXFLAGS += -std=c++11
macx {
    cache()
}
