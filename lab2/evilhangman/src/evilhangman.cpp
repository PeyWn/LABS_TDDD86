#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <set>
#include <map>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";
const string FILENAME = "dictionary.txt";

void filterWordLength(int wordLength, queue<string> &wordList);
void getDictionary(queue<string> &dict);

int main() {
    int wordLength;
    int guesses;
    queue<string> wordList;
    set<string> guessedChars;

    cout << "Welcome to Hangman." << endl;

    do{
        getDictionary(wordList);

        cout << "Enter a word length: ";
        cin >> wordLength;
        filterWordLength(wordLength, wordList);
    }while(wordList.size() == 0);

    do{
        cout << "Enter amount of guesses: ";
        cin >> guesses;
    }while(guesses <= 0);

    string input;
    bool showWordC;

    cout << "Do you want to see amount of remaining words? (y/n): ";
    cin >> input;

    showWordC = (input == "y");

    string guessedString;

    for(int i = 0; i < wordLength; i++){
        guessedString.append("-"); //Writes the word with dashes of the length wordLength.
    }

    string guess;
    while(guesses > 0){
        cout << "Guesses left: " << guesses << endl << endl;
        cout << guessedString << endl << endl;

        do{
            cout << "Guess a letter: ";
            cin >> guess;
        }while(guess.length() != 1 || ALPHABET.find(guess) == -1 || guessedChars.find(guess) != guessedChars.end());








        guesses--;
    }


    return 0;
}

void createFamilyes(){

}

void filterWordLength(int wordLength, queue<string> &wordList){
    int wordC = wordList.size();

    for(int i = 0; i < wordC; i++){
        string curWord = wordList.front();
        wordList.pop();

        if(curWord.length() == wordLength){
            wordList.push(curWord);
        }
    }
}


void getDictionary(queue<string> &dict){
    ifstream input;
    input.open(FILENAME);
    string word;

    while(input >> word){
        dict.push(word);
    }
    input.close();
}
