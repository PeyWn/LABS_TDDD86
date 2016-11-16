/*
 * TDDD86 Huffman Encoding
 *
 * This file declares the HuffmanNode structure that you will use in your
 * Huffman encoding tree.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 */

#ifndef _huffmannode_h
#define _huffmannode_h

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "bitstream.h"
using namespace std;

/* Type: HuffmanNode
 * A node inside a Huffman encoding tree.	 Each node stores four
 * values - the character stored here (or NOT_A_CHAR if the value
 * is not a character), pointers to the 0 and 1 subtrees, and the
 * character count (weight) of the tree.
 */
struct HuffmanNode {
    int character;       // character being represented by this node
    int count;           // number of occurrences of that character
    HuffmanNode* zero;   // 0 (left) subtree (nullptr if empty)
    HuffmanNode* one;    // 1 (right) subtree (nullptr if empty)

    /*
     * Constructs a new node to store the given character and its count,
     * along with the given child pointers.
     */
    HuffmanNode(int character = NOT_A_CHAR, int count = 0,
                HuffmanNode* zero = nullptr, HuffmanNode* one = nullptr);

    /*
     * Returns true if this node is a leaf (has NULL children).
     */
    bool isLeaf() const;

    /*
     * Returns a string representation of this node for debugging.
     */
    string toString() const;

    /*
     * Returns false if count of this node is strictly less than that of rhs,
     * true otherwise.
     */
    bool operator <(const HuffmanNode &rhs) const;
};

/*
 * Prints an indented horizontal view of the tree of HuffmanNodes with the given
 * node as its root.
 * Can optionally show the memory addresses of each node for debugging.
 */
void printSideways(HuffmanNode* node, bool showAddresses = false, string indent = "");

/*
 * Stream insertion operator so that a HuffmanNode can be printed for debugging.
 */
ostream& operator <<(ostream& out, const HuffmanNode& node);

#endif
