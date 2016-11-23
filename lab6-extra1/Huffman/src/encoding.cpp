#include "encoding.h"
#include <queue>
#include <functional>
#include <bitset>

/*
 * Builds a table with char and occurance
 */
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

/*
 * Creates a Huffman Tree from a frequency table
 */
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

/*
 *  Translates a Huffman Tree to an enconding map
 */
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

/*
 *  Huffman encodes data using the given map
 */
void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {

    char charmander; //Got to catch em' all :P

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

/*
 *  Decodes data using the given Tree
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* curPointer = encodingTree;

    int curBit;

    while(curPointer->character != PSEUDO_EOF){
        if(curPointer->isLeaf()){
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

/*
 * Compresses data using the Huffman encoding
 */
void compress(istream& input, obitstream& output) {
    map<int, int> freq = buildFrequencyTable(input);
    auto tree = buildEncodingTree(freq);
    auto table = buildEncodingMap(tree);

    writeHeader(tree, output);

    input.clear();
    input.seekg(0, ios::beg);

    encodeData(input, table, output);
    freeTree(tree);
}

/*
 * Decompress data using the Huffman decoding
*/
void decompress(ibitstream& input, ostream& output) {
    HuffmanNode* tree = readHeader(input);

    decodeData(input, tree, output);
    freeTree(tree);
}

/*
 * Frees the memory for given Huffman Tree
 */
void freeTree(HuffmanNode* node){
    if(node->isLeaf()){
        delete node;
        return;
    }

    freeTree(node->one);
    freeTree(node->zero);

    delete node;
}


/**
 * Writes a header for the Huffman encoding the given output stream.
 */
void writeHeader(HuffmanNode* root, obitstream& output){
    bool zeroFound = false;
    int EOFplace = 0; //0 = first, 1 = second (placement of End Of File)

    writeHeaderRec(root, output, zeroFound, EOFplace);

    //Write EOF placement at end of header
    output.writeBit(EOFplace);
}

/**
 * Reads a header for a huffman encoding from the input stream.
 * Uses the header to construct and return an encoding tree.
 */
HuffmanNode* readHeader(ibitstream& input){
    HuffmanNode* zero1 = nullptr;
    HuffmanNode* zero2 = nullptr;

    HuffmanNode* root = readHeaderRec(input, zero1, zero2);

    int EOFplace = input.readBit();

    if(EOFplace == 0){
        zero1->character = PSEUDO_EOF;
    }
    else{
        zero2->character = PSEUDO_EOF;
    }

    return root;
}

/*
 * Recursive help function for writing the header
 */
void writeHeaderRec(HuffmanNode* root, obitstream& output, bool& zeroFound, int& EOFplace){
    if(root->isLeaf()){
        output.writeBit(0); //Leaf

        if(root->character == 0){
            zeroFound = true;

            writeNext8bits(0, output);
        }
        else if(root->character == PSEUDO_EOF){
            EOFplace = zeroFound;

            writeNext8bits(0, output);
        }
        else{
            writeNext8bits(root->character, output);
        }

        return;
    }

    output.writeBit(1); //Internal node
    writeHeaderRec(root->zero, output, zeroFound, EOFplace);
    writeHeaderRec(root->one, output, zeroFound, EOFplace);
}

/*
 * Recursive help function for reading the header
 */
HuffmanNode* readHeaderRec(ibitstream& input, HuffmanNode* &zero1, HuffmanNode* &zero2){
    int bit = input.readBit();

    if(bit == 0){
        int ascii = readNext8Bits(input);

        HuffmanNode* newNode = new HuffmanNode(ascii);

        if(ascii == 0){
            if(zero1 == nullptr){
                zero1 = newNode;
            }
            else{
                zero2 = newNode;
            }
        }

        return newNode;
    }

    return new HuffmanNode(NOT_A_CHAR, 0, readHeaderRec(input, zero1, zero2), readHeaderRec(input, zero1, zero2));
}

/*
 * Writes the given int (has to be < 256) as 8 bits to the output stream;
 */
void writeNext8bits(const int ascii, obitstream& output){

     string binary = bitset<8>(ascii).to_string(); //Source http://stackoverflow.com/questions/22746429/c-decimal-to-binary-converting
     for(char c : binary){
         output.writeBit( c == '1' );
     }
}

/*
 * Reads 8 bits and returns corresponding int.
 */
int readNext8Bits(ibitstream& input){

    string binary;

    for(int i = 0; i < 8; i++){
        if(input.readBit() == 1){
            binary += "1";
        }
        else{
            binary += "0";
        }
    }

    return bitset<8>(binary).to_ulong();
}

/*
 * Utility function to be able to compare HUffmmanNode-pointers using comparison from HuffmanNode
 */
bool comparePointers(HuffmanNode* n1, HuffmanNode* n2){
    return (*n1 < *n2);
}
