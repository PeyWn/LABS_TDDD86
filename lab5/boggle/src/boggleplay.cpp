#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
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

    //TODO
    //Play rest of game
    cout << boggle.boardToString() << endl;

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
