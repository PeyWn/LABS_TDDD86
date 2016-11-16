#include "encoding.h"
#include <queue>
#include <functional>

map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;

    char curChar;

    while(input.get(curChar)){
        if(freqTable.find(curChar) != freqTable.end()){
            freqTable[curChar]++;
        }
        else{
            freqTable.insert(pair<int, int>(curChar, 1));
        }
    }

    freqTable.insert(pair<int, int>(PSEUDO_EOF, 1));

    return freqTable;
}

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable){
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, function<bool(HuffmanNode*, HuffmanNode*)>> que(comparePointers);

    for(auto character: freqTable){
        HuffmanNode* newNode = new HuffmanNode(character.first, character.second);

        que.push(newNode);
    }

    while(que.size() > 1){

        HuffmanNode* leftChild = que.top();
        que.pop();
        HuffmanNode* rightChild = que.top();
        que.pop();

        HuffmanNode* newRoot = new HuffmanNode(NOT_A_CHAR, (leftChild->count + rightChild->count), leftChild, rightChild);
        que.push(newRoot);
    }

    return que.top();
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    map<int, string> encodingMap;

    if(encodingTree->isLeaf()){
        encodingMap.insert(pair<int, string>(encodingTree->character, ""));

        return encodingMap;
    }

    map<int, string> zeroMap = buildEncodingMap(encodingTree->zero);
    map<int, string> oneMap = buildEncodingMap(encodingTree->one);

    for(auto code : zeroMap){
        encodingMap.insert(pair<int, string>(code.first, "0" + code.second));
    }

    for(auto code : oneMap){
        encodingMap.insert(pair<int, string>(code.first, "1" + code.second));
    }

    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {

    char charmander;

    while(input.get(charmander)){
        string bitString = encodingMap.at((int)charmander);

        for(char c : bitString){
           output.writeBit((c == '1'));
        }
    }

    for(char c : encodingMap.at(PSEUDO_EOF)){
       output.writeBit((c == '1'));
    }
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* curPointer = encodingTree;

    int curBit;

    while(curPointer->character != PSEUDO_EOF){
        if(curPointer->isLeaf()){
            output.put(curPointer->character);

            curPointer = encodingTree;
        }

        curBit = input.readBit();

        cout << curBit << endl;

        if(curBit == 1){
            curPointer = curPointer->one;
        }
        else{
            curPointer = curPointer->zero;
        }

        cout << curPointer->character << endl;
    }
}

void compress(istream& input, obitstream& output) {
    map<int, int> freq = buildFrequencyTable(input);
    auto tree = buildEncodingTree(freq);
    auto table = buildEncodingMap(tree);

    writeHeader(freq, output);

    encodeData(input, table, output);
    freeTree(tree);
}

void decompress(ibitstream& input, ostream& output) {
    HuffmanNode* tree = readHeader(input);

    decodeData(input, tree, output);
    freeTree(tree);
}

void freeTree(HuffmanNode* node){
    if(node->character != NOT_A_CHAR){
        delete node;
        return;
    }

    freeTree(node->one);
    freeTree(node->zero);

    delete node;
}

void writeHeader(map<int, int> &freqTable, ostream& output){
   output << "{";

   int first = true;
   for(auto freq: freqTable){
       if(!first){
           output << ", ";
       }

       first = false;

       output << freq.first << ":" << freq.second;
   }

   output << "}";
}

HuffmanNode* readHeader(ibitstream& input){
    map<int, int> freqMap;

    char curChar;
    input >> curChar;

    while(curChar != '}'){
        int ascii;
        int frequency;

        input >> ascii;
        input.get();
        input >> frequency;

        freqMap.insert(pair<int, int>(ascii, frequency));

        input >> curChar;
    }

    HuffmanNode* tree = buildEncodingTree(freqMap);

    return tree;
}

//Utility function to be able to compare HUffmmanNode-pointers using comparison from HuffmanNode
bool comparePointers(HuffmanNode* n1, HuffmanNode* n2){
    return (*n1 < *n2);
}
