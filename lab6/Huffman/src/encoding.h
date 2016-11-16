/*
 * TDDD86 Huffman Encoding
 * This file declares the functions that you will need to write in this
 * assignment for your Huffman Encoder in huffmanencoding.cpp.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 */

#ifndef _encoding_h
#define _encoding_h

#include <iostream>
#include <string>
#include <map>
#include "bitstream.h"
#include "HuffmanNode.h"
using namespace std;

/*
 * See huffmanencoding.cpp for documentation of these functions
 * (which you are supposed to write, based on the spec).
 */
map<int, int> buildFrequencyTable(istream& input);
HuffmanNode* buildEncodingTree(const map<int, int>& freqTable);
map<int, string> buildEncodingMap(HuffmanNode* encodingTree);
void encodeData(istream& input, const map<int, string>& encodingMap, obitstream& output);
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output);
void compress(istream& input, obitstream& output);
void decompress(ibitstream& input, ostream& output);
void freeTree(HuffmanNode* node);
void writeHeader(map<int, int> &freqTable, ostream& output);
HuffmanNode* readHeader(ibitstream& input);
bool comparePointers(HuffmanNode* n1, HuffmanNode* n2);
#endif
