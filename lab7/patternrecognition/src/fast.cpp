#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <chrono>
#include "Point.h"

// constants
static const int SCENE_WIDTH = 512;
static const int SCENE_HEIGHT = 512;

void render_points(QGraphicsScene* scene, const vector<Point>& points) {
    for(const auto& point : points) {
        point.draw(scene);
    }
}

void render_line(QGraphicsScene* scene, const Point& p1, const Point& p2) {
    p1.lineTo(scene, p2);
}

void finishSet(const Point& curP, set<Point>* points, QGraphicsScene* scene){
    if(points->size() >= 3){
        points->insert(curP);
        render_line(scene, *points->begin(), *points->rbegin());
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // open file
    string filename = "input56.txt";
    ifstream input;
    input.open(filename);

    // the vector of points
    vector<Point> points;

    // read points from file
    int N;
    int x;
    int y;

    input >> N;

    for (int i = 0; i < N; ++i) {
        input >> x >> y;
        points.push_back(Point(x, y));
    }
    input.close();

    // setup graphical window
    QGraphicsView *view = new QGraphicsView();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    view->setScene(scene);
    // draw points to screen all at once
    render_points(scene, points);
    view->scale(1, -1); //screen y-axis is inverted
    view->resize(view->sizeHint());
    view->setWindowTitle("Uber Jolted Turbo Fast Pattern Recognition");
    view->show();

    // sort points by natural order
    // makes finding endpoints of line segments easy
   stable_sort(points.begin(), points.end());
    auto begin = chrono::high_resolution_clock::now();

    for (int i = 0 ; points.size() > 2; i++) {
        Point& curP = points.at(points.size()-1);
        points.pop_back();

        auto dSlope = [&curP](const Point& p1, const Point& p2) -> bool{return p1.slopeTo(curP) < p2.slopeTo(curP);};
        sort(points.begin(), points.end(), dSlope);

        set<Point> curTilt = {points.at(0)};

        for(int j = 1; j < points.size(); j++){
            if(points.at(j).slopeTo(curP) == curTilt.begin()->slopeTo(curP)){
                curTilt.insert(points.at(j));
            }
            else{
                finishSet(curP, &curTilt, scene);

                curTilt.clear();
                curTilt.insert(points.at(j));
            }
        }

        finishSet(curP, &curTilt, scene);
    }

    auto end = chrono::high_resolution_clock::now();
    cout << "Computing line segments took "
         << std::chrono::duration_cast<chrono::milliseconds>(end - begin).count()
         << " milliseconds." << endl;

    return a.exec(); // start Qt event loop
}
