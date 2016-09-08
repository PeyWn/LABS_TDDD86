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

set<string> getDictionary();
void printStack(stack<string> &words);
void findWordChain(string &w1, string &w2);
vector<string> getNeighbors(string &word, set<string> &wordList);

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
    set<string> dict = getDictionary();

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
            printStack(curStack);
            return;
        }
        else{
            vector<string> neighbors = getNeighbors(topWord, dict);

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

vector<string> getNeighbors(string &word, set<string> &dict){
    vector<string> neighbors;

    for(int letterI = 0; letterI < word.length(); letterI++){
        for(int alphaI = 0; alphaI < ALPHABET.length(); alphaI++){
            string newWord = word;
            newWord[letterI] = ALPHABET[alphaI];

            if(dict.find(newWord) != dict.end()){
                neighbors.insert(neighbors.end(), newWord);
            }
        }
    }

    return neighbors;
}

void printStack(stack<string> &words){
    cout << "Chain from data back to code:" << endl;

    while(!words.empty()){
        cout << words.top() << " ";
        words.pop();
    }

    cout << endl;
}

set<string> getDictionary(){
    ifstream input;
    input.open(FILENAME);
    string word;

    set<string> dict;

    while(input >> word){
        dict.insert(word);
    }
    input.close();
    return dict;

}


