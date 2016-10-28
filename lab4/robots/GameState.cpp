/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"

GameState::GameState(){}

GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; i++) {
        Robot* robot;
        bool badPlacement = false;
        do{
            robot = new Robot();
            badPlacement = !isEmpty(*robot);

            if(badPlacement){
                delete robot;
            }
        }while (badPlacement);

        robots.push_back(robot);
    }
    teleportHero();
}

GameState::~GameState(){
    for(int i = (robots.size() - 1); i >= 0; i--){
        delete robots[i];
        robots.pop_back();
    }
}

GameState::GameState(const GameState& other){
    copy(other);
}

GameState& GameState::operator=(const GameState& other){
    copy(other);
    return *this;
}

void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    for (size_t i = 0; i < robots.size(); ++i){
        robots[i]->draw(scene);
    }
    hero.draw(scene);
}

void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); i++)
        if (!robots[i]->isJunk()){
            robots[i]->moveTowards (hero);
        }
}

int GameState::countCollisions() {
    int numberDestroyed = 0;
    unsigned int i = 0;
    while (i < robots.size()) {
        if ((countRobotsAt (*robots[i]) > 1) && (!robots[i]->isJunk()) ) {
            Robot* oldRobot = robots[i];
            robots[i] = new Junk(*oldRobot);
            delete oldRobot;
            numberDestroyed++;
        } else i++;
    }
    return numberDestroyed;
}

bool GameState::anyRobotsLeft() const {
    int count = 0;

    for(Robot* r : robots){
        if(!r->isJunk()){
            count++;
        }
    }

    return (count != 0);
}

bool GameState::heroDead() const {
    return !isEmpty(hero);
}

bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); i++)
        if (robots[i]->isJunk() && robots[i]->at(unit)){
            return false;
        }
        else if(robots[i]->attacks(unit)){
            return false;
        }
    return true;
}

void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}

Hero GameState::getHero() const {return hero;}

/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {
    return countRobotsAt(unit) == 0;
}

/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (int i = 0; i < robots.size(); i++) {
        if (robots[i]->at(unit)){
            count++;
        }
    }
    return count;
}
/*
 * Copy all values from one GameState to this.
 */

void GameState::copy(const GameState& other){
    hero = other.getHero();

    for(int i = (robots.size() - 1); i >= 0; i--){
        delete robots[i];
        robots.pop_back();
    }

    for(int i = 0; i < other.robots.size(); i++){
        if(other.robots[i]->isJunk()){
            robots.push_back(new Junk(*(other.robots[i])));
        }
        else{
            robots.push_back(new Robot(*(other.robots[i])));
        }
    }
}
