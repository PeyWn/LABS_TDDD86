#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
using namespace std;

const string ALPHABET  = "abcdefghijklmnopqrstuvwxyz";
const string FILENAME = "dictionary.txt";

/*Add words of a specific length from a dictionary file to a given set.*/
void getDictionary(set<string> &dict, int wordLength);
/*Prints words from stack to console.*/
void printStack(stack<string> &words);
/*Algorithm for finding the shourtest path from one word to an other, replacing one letter at a time only using words from the english language.*/
void findWordChain(string &w1, string &w2);
/*Add words from wordlist that differs by one letter to referanced word to a vector.*/
void getNeighbors(string &word, set<string> &wordList, vector<string> &neighbors);

int main() {
    cout << "Welcome to TDDD86 Word Chain." << endl;
    cout << "If you give me two English words, I will transform the" << endl;
    cout << "first into the second by changing one letter at a time." << endl;
    cout << endl;

    cout << "Please type two words: ";
    string nWord;
    string w1;
    string w2;

    cin >> w1 >> w2;

    findWordChain(w1, w2);

    cout << "Have a nice day." << endl;

    return 0;
}

void findWordChain(string &w1, string &w2){
    set<string> dict;
    getDictionary(dict, w1.length());

    queue<stack<string>> wordQueue;
    stack<string> w1Stack;
    w1Stack.push(w1);
    wordQueue.push(w1Stack);

    set<string> usedWords;
    usedWords.insert(w1);

    while(!wordQueue.empty()){
        stack<string> curStack = wordQueue.front();
        string topWord = curStack.top();
        wordQueue.pop();

        if(topWord == w2){
            cout << "Chain from " << w2 << " back to " << w1 << ":" << endl;
            printStack(curStack);
            return;
        }
        else{
            vector<string> neighbors;
            getNeighbors(topWord, dict, neighbors);

            for(string neighbor : neighbors){
                if(usedWords.find(neighbor) == usedWords.end()){
                    stack<string> newStack = curStack;
                    newStack.push(neighbor);
                    wordQueue.push(newStack);
                    usedWords.insert(neighbor);
                }
            }
        }
    }
}

void getNeighbors(string &word, set<string> &dict, vector<string> &neighbors){

    for(int letterI = 0; letterI < word.length(); letterI++){
        for(int alphaI = 0; alphaI < ALPHABET.length(); alphaI++){
            string newWord = word;
            newWord[letterI] = ALPHABET[alphaI];

            if(dict.find(newWord) != dict.end()){
                neighbors.insert(neighbors.end(), newWord);
            }
        }
    }
}

void printStack(stack<string> &words){
    while(!words.empty()){
        cout << words.top() << " ";
        words.pop();
    }

    cout << endl;
}

void getDictionary(set<string> &dict, int wordLength){
    ifstream input;
    input.open(FILENAME);
    string word;

    while(input >> word){
        if(word.length() == wordLength){
            dict.insert(word);
        }
    }
    input.close();
}


