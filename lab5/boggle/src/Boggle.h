#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <grid.h>
#include <lexicon.h>
#include <set>

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;

    Boggle();

    /**
     * @brief Resets scores and words for player and computer as well as randomizes the board.
     */
    void resetBoard();

    /**
     * @brief Resets scores and words for player and computer as well as sets the board to a given string.
     * @param 16 letter string
     */
    void resetBoard(string& letters);

    /**
     * @brief makes the computer play a perfect turn of boggle.
     */
    void computerFindWords();

    /**
     * @brief Checks is the given word exists in the dictionary.
     * @param word to search for.
     */
    bool wordInDict(const string& word) const;

    /**
     * @brief Checks if a word is long enough.
     * @param word
     */
    bool longEnough(const string& word) const;

    /**
     * @brief checks if word has been found by the player.
     * @param word
     */
    bool isPlayed(const string& word) const;

    /**
     * @brief Checks if a word is on the board.
     * @param word
     */
    bool wordInBoard(const string& word) const;

    int getPlayerScore() const;
    int getComputerrScore() const;
    set<string> getPlayerWords() const;
    set<string> getComputerWords() const;

    /**
     * @brief Finds all possible words on board that are long enough.
     */
    vector<string> findAllWords() const;

    /**
     * @brief Adds a word to the players found words and awards points.
     * @param word
     */
    void playerFoundWord(const string& word);

    string boardToString() const;
private:
    Grid<char> board;
    Lexicon wordList;
    int playerScore;
    int computerScore;
    set<string> playerWords;
    set<string> computerWords;


    /**
     * @brief traverses board and adds all possible words to the foundWords vector.
     */
    void traverseBoard(const int col, const int row, const string prefix, vector<int*> visited, vector<string> &foundWords) const;
    bool findWord(const string& word, const int col, const int row, vector<int*> visited) const;
    int getScore(const string& word) const;
    void resetValues();
    bool coordsInVector(const int col, const int row, vector<int*>& coordinates) const;
    void insertUnique(const string word, vector<string> &wordList) const;
};

#endif
