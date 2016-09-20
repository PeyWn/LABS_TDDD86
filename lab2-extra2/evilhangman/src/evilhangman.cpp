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
vector<int> advanceWordList(char guess, vector<string> &wordList, set<char> &guesses);

//Inserts the given char in the word at all the positions from the vector.
void insertChars(string &word, vector<int> positions, char newChar);

//Removes all the words from wordlist that are not of the length wordLength.
void filterWordLength(int wordLength, vector<string> &wordList);

//Inserts a dictionary from a file to the given que.
void getDictionary(vector<string> &dict);

void partitionWordList(char guess, vector<string> &wordList, map<vector<int>,vector<string>> &partitions);

vector<int> findLargestPartition(map<vector<int>,vector<string>> &partitions);

string removeFromString(string alfa, set<char> &charsToRm);

int main() {
    string replayAns; //answer to cin if you want to play again.

    do{
        bool play = true;
        set<char> guessedChars;
        int wordLength;
        vector<string> wordList;
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
                   guessedChars.find(guess[0]) != guessedChars.end()); //Checks that input is valid.

            char guessChar = guess[0];
            guessedChars.insert(guessChar);

            vector<int> charPos = advanceWordList(guessChar, wordList, guessedChars);

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

vector<int> advanceWordList(char guess, vector<string> &wordList, set<char> &guesses){
    map<vector<int>,vector<string>> partitions;


    partitionWordList(guess, wordList, partitions);

    string loopChars = removeFromString(ALPHABET, guesses);

    int highScore = 0;
    vector<int> highScoreKey;

    for(auto partPair : partitions){
        int score = 0;
        for(char c : loopChars){
            map<vector<int>,vector<string>> guessPartitions;
            partitionWordList(c, partPair.second, guessPartitions);
            score += guessPartitions[findLargestPartition(guessPartitions)].size();
        }
        if(score > highScore){
            highScore = score;
            highScoreKey = partPair.first;
        }
        cout << "Score: " << score  << " PartitionSize: " << partPair.second.size() << " Current HighScore: " << highScore << endl;
    }

    wordList = partitions[highScoreKey]; //Replace wordlist
    return highScoreKey;
}

string removeFromString(string alfa, set<char> &charsToRm){
    string newString;
    for(char c : alfa){
        if(charsToRm.find(c) == charsToRm.end()){
            newString.append(1,c);
        }
    }

    return newString;
}

vector<int> findLargestPartition(map<vector<int>,vector<string>> &partitions){
    //Find the longest vector
    int longest = 0;
    vector<int> longestKey;

    for(auto wordPairs:partitions){
        if(wordPairs.second.size() > longest){
            longest = wordPairs.second.size();
            longestKey = wordPairs.first;
        }
    }

    return longestKey;
}

void partitionWordList(char guess, vector<string> &wordList, map<vector<int>,vector<string>> &partitions){
    for(string curWord : wordList){
        vector<int> curPositions = findAllLetters(curWord, guess);

        if(partitions.find(curPositions) == partitions.end()){
            //Queue of this type does not exist in the map, so create it
            partitions.insert(pair<vector<int>, vector<string>>(curPositions, vector<string>()));
        }

        partitions[curPositions].push_back(curWord);
    }
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

void filterWordLength(int wordLength, vector<string> &wordList){
    vector<string> correctWords;
    for(string curWord : wordList){
        if(curWord.length() == wordLength){
            correctWords.push_back(curWord);
        }
    }

    wordList = correctWords;
}

void getDictionary(vector<string> &dict){
    ifstream input;
    input.open(FILENAME);
    string word;

    while(input >> word){
        dict.push_back(word);
    }
    input.close();
}
