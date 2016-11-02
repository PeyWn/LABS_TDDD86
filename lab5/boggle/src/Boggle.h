// This is the .h file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header and replace it with your own

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <grid.h>
#include <lexicon.h>
#include <set>
// TODO: include any other header files you need

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    Boggle();

    void resetBoard();
    void resetBoard(string& letters);

    bool wordInDict(const string& word) const;
    bool longEnough(const string& word) const;
    bool isPlayed(const string& word) const;
    bool wordInBoard(const string& word) const;
    int getPlayerScore() const;
    int getComputerrScore() const;
    set<string> getPlayerWords() const;
    set<string> getComputerWords() const;

    vector<string> findAllWords() const;

    void playerFoundWord(const string& word);
    void computerFoundWord(const string& word);

    string boardToString() const;
private:
    Lexicon wordList;
    int playerScore;
    int computerScore;
    set<string> playerWords;
    set<string> computerWords;
    Grid<char> board;

    bool findWord(const string& word, const int col, const int row, vector<int*> visited) const;
    int getScore(const string& word) const;
    void resetValues();
    bool coordsInVector(const int col, const int row, vector<int*>& coordinates) const;
};

#endif
