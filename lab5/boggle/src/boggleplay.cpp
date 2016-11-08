#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
/*
 * Prints all strings in a set with "," in between.
 */
void printWords(set<string> words){
    bool firstWord = true;
    for(auto word : words){
        if(!firstWord){
            cout << ", ";
        }
        firstWord = false;

        cout << "\"" << word << "\"";
    }
}

void playerTurn(Boggle& boggle){
    string input = "";

    do{
        clearConsole();

        if(input != ""){
            if(!boggle.longEnough(input)){
                cout << "That word is not long enough." << endl;
            }
            else if(boggle.isPlayed(input)){
                cout << "You have already guessed that word." << endl;
            }
            else if(!boggle.wordInDict(input)){
                cout << "That word is not in the dictionary." << endl;
            }
            else if(!boggle.wordInBoard(input)){
                cout << "That word can't be formed on this board." << endl;
            }
            else{
                cout << "You found a new word! \"" << input << "\"" << endl;
                boggle.playerFoundWord(input);
            }
        }
        cout << boggle.boardToString() << endl << endl;

        set<string> playerWords = boggle.getPlayerWords();

        cout << "Your words (" << playerWords.size() << "): {";

        printWords(playerWords);
        cout << "}" << endl;

        cout << "Your score: " << boggle.getPlayerScore() << endl;
        cout << "Type a word (or press Enter to end your turn): " << endl;

        getline(cin, input);
        input = trim(toUpperCase(input));

    }while(!input.empty());
}

void computerTurn(Boggle& boggle){
    boggle.computerFindWords();

    set<string> computerWords = boggle.getComputerWords();

    cout << "My fab words are ("<< computerWords.size()<< "): {";

    printWords(computerWords);
    cout << "}" << endl;

    cout << "My score: " << boggle.getComputerrScore() << endl;
}

bool isAlphabetical(string& word){
    for(int i = 0; i < word.size(); i++){
        if(ALPHABET.find(word.at(i)) == ALPHABET.npos){
            return false;
        }
    }

    return true;
}

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    clearConsole();

    if(yesOrNo("Do you want to generate a random board? ")){
        boggle.resetBoard();
    }
    else{
        cout << "Type the 16 letters to appear on the board: ";
        string boardLetters;

        while(true){
            cin >> boardLetters;
            boardLetters = trim(toUpperCase(boardLetters));

            if((boardLetters.length() == 16) && isAlphabetical(boardLetters)){
                break;
            }
            else{
                cout << "That is not a valid 16-letter board String. Try again." << endl;
            }
        }
        boggle.resetBoard(boardLetters);
    }
    cout << "It's your turn!" << endl;

    playerTurn(boggle);

    cout << "It's my turn!"<< endl;

    computerTurn(boggle);

    if(boggle.getComputerrScore() > boggle.getPlayerScore()){
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    }
    else{
        cout << "WOOOOOOOW, you defeated me! Congratulations!" << endl;
    }
}



/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}
