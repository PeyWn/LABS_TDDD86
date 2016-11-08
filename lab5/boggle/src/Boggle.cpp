#include <sstream>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"
#include <algorithm>

//Deltas for neighbors of a coordinate
static const int deltaC = 8;
static const int deltas[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {  // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Boggle::Boggle() : board(BOARD_SIZE, BOARD_SIZE), wordList(DICTIONARY_FILE), playerWords(), computerWords(){}

void Boggle::resetBoard(){
    resetValues();
    for(int col = 0; col < BOARD_SIZE; col++){
        for(int row = 0; row < BOARD_SIZE; row++){
            int dice = 4*row + col;
            char c = CUBES[dice].at(randomInteger(0,CUBE_SIDES-1));
            board.set(row, col, c);
        }
    }
    shuffle(board);
}

void Boggle::resetBoard(string& letters){
    resetValues();

    int charI = 0;
    for(int col = 0; col < BOARD_SIZE; col++){
        for(int row = 0; row < BOARD_SIZE; row++){
            board.set(row, col, letters.at(charI));
            charI++;
        }
    }
}

bool Boggle::wordInDict(const string& word)const {
    return wordList.contains(word);
}

bool Boggle::longEnough(const string& word)const {
    return word.length() >= BOARD_SIZE;
}

bool Boggle::isPlayed(const string& word)const {
    return (playerWords.find(word) != playerWords.end());
}

bool Boggle::wordInBoard(const string& word) const{
    for(int col = 0; col < BOARD_SIZE; col++){
        for(int row = 0; row < BOARD_SIZE; row++){
            vector<int*> newVec;
            if(findWord(word, col, row, newVec)){
                return true;
            }
        }
    }

    return false;
}

vector<string> Boggle::findAllWords() const{
    vector<string> words;
    for(int col = 0; col < BOARD_SIZE; col++){
        vector<int*> newVec;
        for(int row = 0; row < BOARD_SIZE; row++){
            traverseBoard(col, row, "", newVec, words);
        }
    }
    return words;
}

void Boggle::computerFindWords() {
    vector<string> allWords = findAllWords();

    for(auto word : allWords){
        if(playerWords.find(word) == playerWords.end()){
            computerWords.insert(word);
            computerScore += getScore(word);
        }
    }
}

int Boggle::getPlayerScore()const {
    return playerScore;
}
int Boggle::getComputerrScore()const {
    return computerScore;
}
set<string> Boggle::getPlayerWords() const {
    return playerWords;
}
set<string> Boggle::getComputerWords() const {
    return computerWords;
}

void Boggle::playerFoundWord(const string& word){
    playerScore += getScore(word);
    playerWords.insert(word);
}


string Boggle::boardToString() const{
    ostringstream os;
    for(int col = 0; col < BOARD_SIZE; col++){
        for(int row = 0; row < BOARD_SIZE; row++){
            os << board.get(row, col);
        }
        os << endl;
    }

    return os.str();
}


void Boggle::traverseBoard(const int col, const int row, const string prefix, vector<int*> visited, vector<string> &foundWords) const{
    string newPrefix = prefix + board.get(row, col);
    if(!wordList.containsPrefix(newPrefix)){
        return;
    }

    if(longEnough(newPrefix) && wordList.contains(newPrefix)){
        insertUnique(newPrefix, foundWords);
    }

    int newCoords[] = {col, row};
    visited.push_back(newCoords);

    for(int i = 0; i < deltaC; i++){
        int newRow = row + deltas[i][0];
        int newCol = col + deltas[i][1];

        if(board.inBounds(newRow, newCol)
                && !coordsInVector(newCol, newRow, visited)){
            traverseBoard(newCol, newRow, newPrefix, visited, foundWords);
        }
    }

}

bool Boggle::findWord(const string& word, const int col, const int row, vector<int*> visited) const{
    if(word.length()==0){
        return true;
    }
    else if(word.at(0) != board.get(row, col)){
        return false;
    }
    else{
        int newCoords[] = {col, row};
        visited.push_back(newCoords);

        for(int i = 0; i < deltaC; i++){
            int newRow = row + deltas[i][0];
            int newCol = col + deltas[i][1];

            if(board.inBounds(newRow, newCol) &&
                    !coordsInVector(newCol, newRow, visited) &&
                    findWord(word.substr(1, word.npos), newCol, newRow, visited)){
                return true;
            }
        }

        return false;
    }
}

int Boggle::getScore(const string& word) const{
    return word.length()-BOARD_SIZE+1;
}

void Boggle::resetValues(){
    playerScore = 0;
    computerScore = 0;
    playerWords.clear();
    computerWords.clear();
}

bool Boggle::coordsInVector(const int col, const int row, vector<int*>& coordinates) const{
    for(auto coord : coordinates){
        if(coord[0] == col && coord[1] == row){
            return true;
        }
    }

    return false;
}

void Boggle::insertUnique(const string word, vector<string> &wordList) const{
    if(find(wordList.begin(), wordList.end(), word) == wordList.end()){
        wordList.push_back(word);
    }
}
