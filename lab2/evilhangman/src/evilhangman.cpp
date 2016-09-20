#include <iostream>
#include <string>
#include <queue>
#include <fstream>
#include <set>
#include <map>
#include <vector>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";
const string FILENAME = "dictionary.txt";

//Finds all positions of a character in a string
vector<int> findAllLetters(string word, char letter);

/* Replaces wordList with the biggest subset of words that are still possible after the given guess in hangman.
 * Returns the positions of the guessed char in the words in the subset.
 */
vector<int> advanceWordList(char guess, queue<string> &wordList);

//Inserts the given char in the word at all the positions from the vector.
void insertChars(string &word, vector<int> positions, char newChar);

//Removes all the words from wordlist that are not of the length wordLength.
void filterWordLength(int wordLength, queue<string> &wordList);

//Inserts a dictionary from a file to the given que.
void getDictionary(queue<string> &dict);

int main() {
    string replayAns; //answer to cin if you want to play again.

    do{
        bool play = true;
        set<string> guessedChars;
        int wordLength;
        queue<string> wordList;
        int guesses;

        cout << "Welcome to Hangman." << endl;

        do{     //Handles the first game setting question: word length
            getDictionary(wordList);

            cout << "Enter a word length: ";
            cin >> wordLength;

            filterWordLength(wordLength, wordList);
        }while(wordList.size() == 0);

        do{     //Handles the second game setting question: Amount of guesses
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
        while(play){    //This is the main game loop
            cout << "Guesses left: " << guesses << endl << endl;
            cout << guessedString << endl << endl;

            do{
                cout << "Guess a letter: ";
                cin >> guess;
            }while(guess.length() != 1 ||
                   ALPHABET.find(guess) == -1 ||
                   guessedChars.find(guess) != guessedChars.end()); //Checks that input is valid.

            guessedChars.insert(guess);
            char guessChar = guess[0];

            vector<int> charPos = advanceWordList(guessChar, wordList);

            insertChars(guessedString, charPos, guessChar);

            if(showWordC){
                cout << "Words left: " << wordList.size() << " (I am totally not cheating) " << endl << endl;
            }

            if(charPos.empty()){ //If charPos is empty, the guessed char is not in the word (wrong guess)
                guesses--;
            }

            if(guessedString.find("-") == guessedString.npos){
                cout << "Congrats commrade, you win! The word was: " << guessedString << endl;
                play = false;
                cout << endl;
            }
            else if(guesses == 0){
                cout << "You lose! *Evil laughter*"<< endl;
                cout << "The word was: " << wordList.front() << endl;
                play = false;
            }
        }
        cout << "Play again? (y/n): ";
        cin >> replayAns;
        cout << endl;
    }while(replayAns == "y");

    return 0;
}

void insertChars(string &word, vector<int> positions, char newChar){
    for(int position: positions){
        word[position] = newChar;
    }
}

vector<int> advanceWordList(char guess, queue<string> &wordList){
    map<vector<int>,queue<string>> partitions;

    while(!wordList.empty()){
        string curWord = wordList.front();
        wordList.pop();

        vector<int> curPositions = findAllLetters(curWord, guess);

        if(partitions.find(curPositions) == partitions.end()){
            //Queue of this type does not exist in the map, so create it
            partitions.insert(pair<vector<int>, queue<string>>(curPositions, queue<string>()));
        }

        partitions[curPositions].push(curWord);
    }

    //Find the longest queue
    int longest = 0;
    vector<int> longestKey;
    for(auto wordPairs:partitions){
        if(wordPairs.second.size() > longest){
            longest = wordPairs.second.size();
            longestKey = wordPairs.first;
        }
    }

    wordList = partitions[longestKey]; //Replace wordlist
    return longestKey;
}

vector<int> findAllLetters(string word, char letter){
    vector<int> positions;

    for(int i = 0; i < word.length(); i++){
        if(word[i] == letter){
            positions.push_back(i);
        }
    }

    return positions;
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
