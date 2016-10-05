/**
 * TDDD86 Robots
 * This file contains the implementation of the mainwindow class.
 * See mainwindow.h for comments about each member.
 */

#include <algorithm>
#include "mainwindow.h"
#include "Hero.h"
#include "Robot.h"
#include "Junk.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent) {
    // set default size and color
    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    scene->setBackgroundBrush(QBrush(QColor(204, 192, 179)));
    view = new QGraphicsView(scene);

    // create the main layout
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    // add game board to layout
    mainLayout->insertWidget(0, view);

    // start new game and draw state
    gameState.draw(scene);

    // create label layout
    labelLayout = new QHBoxLayout();

    // create the score widget
    scoreLabel = new QLabel(QString("SCORE: %1").arg(score));
    scoreLabel->setStyleSheet("QLabel { color: rgb(235,224,214); font: 16pt; }");
    scoreLabel->setFixedHeight(35);

    // create the level widget
    levelLabel = new QLabel(QString("LEVEL: %1").arg(level));
    levelLabel->setStyleSheet("QLabel { color: rgb(235,224,214); font: 16pt; }");
    levelLabel->setFixedHeight(35);

    // add score and level widgets to board
    labelLayout->insertWidget(0, levelLabel, 0, Qt::AlignLeft);
    labelLayout->insertWidget(1, scoreLabel, 0, Qt::AlignRight);
    mainLayout->insertLayout(1, labelLayout);

    // style sheet of the game board
    setStyleSheet("MainWindow { background-color: rgb(187,173,160) }");

    // no resizing of game window
    setFixedSize(sizeHint());

    connect(gameOverWindow.getResetBtn(), SIGNAL(clicked()), this, SLOT(resetGame()));
}

MainWindow::~MainWindow() {
    delete scoreLabel;
    delete levelLabel;
    delete labelLayout;
    delete scene;
    delete view;
    delete mainLayout;
}

/*
 * Listens to key press events from the graphical subsystem,
 * and handles the events appropriately:
 *   - '[n]' moves hero in direction [n],
 *           where n = 1, 2, 3, 4, 6, 7, 8, 9 on the numeric keypad
 *   - '5' on the numeric keypad makes the hero wait one turn
 *   - 'T' teleports hero
 */
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (!gameOver) { // only process key presses while game is not over
        Hero hero = gameState.getHero();
        Point pt = hero.asPoint();
        bool actionTaken = false;
        bool waiting = false;

        if (e->modifiers() == Qt::KeypadModifier) {
            switch (e->key()) {
            case Qt::Key_1:
                actionTaken = tryMove(hero, Point{pt.x - 1, pt.y + 1});
                break;
            case Qt::Key_2:
                actionTaken = tryMove(hero, Point{pt.x, pt.y + 1});
                break;
            case Qt::Key_3:
                actionTaken = tryMove(hero, Point{pt.x + 1, pt.y + 1});
                break;
            case Qt::Key_4:
                actionTaken = tryMove(hero, Point{pt.x - 1, pt.y});
                break;
            case Qt::Key_6:
                actionTaken = tryMove(hero, Point{pt.x + 1, pt.y});
                break;
            case Qt::Key_7:
                actionTaken = tryMove(hero, Point{pt.x - 1, pt.y - 1});
                break;
            case Qt::Key_8:
                actionTaken = tryMove(hero, Point{pt.x, pt.y - 1});
                break;
            case Qt::Key_9:
                actionTaken = tryMove(hero, Point{pt.x + 1, pt.y - 1});
                break;
            case Qt::Key_5:
                actionTaken = true;
                waiting = true;
                break;
            default:
                QWidget::keyPressEvent(e);
            }
        } else {
            switch (e->key()) {
            case Qt::Key_T:
                gameState.teleportHero();
                actionTaken = true;
                break;
            default:
                QWidget::keyPressEvent(e);
            }
        }

        if (actionTaken) { // process results of viable move
            processMove(waiting);
        }
    } else {  // game is over - do not process key press
        QWidget::keyPressEvent(e);
    }
}

/*
 * Try to move hero to unit
 */
bool MainWindow::tryMove(Hero hero, const Point& point) {
    if (!outsideBorder(point)) {
        hero.moveTowards(point);
        if (gameState.isSafe(hero)) {
            gameState.moveHeroTowards(point);
            return true;
        }
    }
    return false;
}

/*
 * Process results of viable move
 */
void MainWindow::processMove(bool waiting) {
    gameState.moveRobots();
    score += gameState.countCollisions() * (POINTS_PER_ROBOT +
                                            (waiting ? WAIT_BONUS : 0));
    gameState.draw(scene);
    displayScore();

    if (!gameState.anyRobotsLeft()) { // won level
        numberOfRobots = std::min(MAX_ROBOTS, numberOfRobots + ROBOTS_INC);
        gameState = GameState(numberOfRobots);
        gameState.draw(scene);
        ++level;
        displayLevel();
    } else if (gameState.heroDead()) { // game over
        gameOver = true;
        gameOverWindow.show();
    }
}

/*
 * Is point outside of playing field?
 */
bool MainWindow::outsideBorder(Point const& point) const {
    return point.x > MAX_X
            || point.y > MAX_Y
            || point.x < MIN_X
            || point.y < MIN_Y;
}


void MainWindow::displayScore() const {
    scoreLabel->setText(QString("SCORE: %1").arg(score));
}

void MainWindow::displayLevel() const {
    levelLabel->setText(QString("LEVEL: %1").arg(level));
}

void MainWindow::resetGame() {
    score = 0;
    level = 1;
    numberOfRobots = MIN_ROBOTS;
    gameState = GameState(numberOfRobots);
    gameState.draw(scene);
    displayScore();
    displayLevel();
    gameOver = false;
    gameOverWindow.hide();
}
