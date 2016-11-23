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
 * Builds a tablen with char and occurance
 */
map<int, int> buildFrequencyTable(istream& input);
HuffmanNode* buildEncodingTree(const map<int, int>& freqTable);
map<int, string> buildEncodingMap(HuffmanNode* encodingTree);
void encodeData(istream& input, const map<int, string>& encodingMap, obitstream& output);
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output);
void compress(istream& input, obitstream& output);
void decompress(ibitstream& input, ostream& output);
void freeTree(HuffmanNode* node);

/**
 * Writes a header for the Huffman encoding the given output stream.
 */
void writeHeader(HuffmanNode* root, obitstream& output);

/**
 * Reads a header for a huffman encoding from the input stream.
 * Uses the header to construct and return an encoding tree.
 */
HuffmanNode* readHeader(ibitstream& input);

void writeHeaderRec(HuffmanNode* root, obitstream& output, bool& zeroFound, int& EOFplace);

HuffmanNode* readHeaderRec(ibitstream& input, HuffmanNode* &zero1, HuffmanNode* &zero2);

void writeNext8bits(const int ascii, obitstream& output);

int readNext8Bits(ibitstream& input);

/*
 * Utility function to be able to compare HUffmmanNode-pointers using comparison from HuffmanNode
 */
bool comparePointers(HuffmanNode* n1, HuffmanNode* n2);
#endif
