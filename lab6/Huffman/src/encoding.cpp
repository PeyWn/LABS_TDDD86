// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>
// TODO: include any other headers you need

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

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {

    priority_queue<HuffmanNode, vector<HuffmanNode>> que;

    for(auto character: freqTable){
        que.push(HuffmanNode(character.first, character.second));
    }

    while(que.size() > 1){

        HuffmanNode* leftChild = new HuffmanNode(que.top().character, que.top().count, que.top().zero, que.top().one);
        que.pop();
        HuffmanNode* rightChild = new HuffmanNode(que.top().character, que.top().count, que.top().zero, que.top().one);
        que.pop();

        que.push(HuffmanNode(NOT_A_CHAR, (leftChild->count + rightChild->count), leftChild, rightChild));
    }
    HuffmanNode* rootNode = new HuffmanNode(que.top().character, que.top().count, que.top().zero, que.top().one);

    return rootNode;
}

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    map<int, string> encodingMap;

    if(encodingTree->character != NOT_A_CHAR){
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
        if(curPointer->character != NOT_A_CHAR){
            output.put(curPointer->character);

            curPointer = encodingTree;
        }

        curBit = input.readBit();

        if(curBit == 1){
            curPointer = curPointer->one;
        }
        else{
            curPointer = curPointer->zero;
        }
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

    cout << tree->toString() << endl;

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

        cout << "ascii: " << ascii << " freq: " << frequency << endl;

        freqMap.insert(pair<int, int>(ascii, frequency));

        input >> curChar;
    }

    for(auto pair : freqMap){
        cout << pair.first << " " << pair.second << endl;
    }

    HuffmanNode* tree = buildEncodingTree(freqMap);
    cout << tree->toString() << endl;

    return tree;
}
