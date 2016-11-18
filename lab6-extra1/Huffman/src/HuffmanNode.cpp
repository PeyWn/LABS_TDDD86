/*
 * TDDD86 Huffman Encoding
 * This file implements the members of the HuffmanNode structure that you will
 * use in your Huffman encoding tree.  See HuffmanNode.h for documentation of
 * each member.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 */

#include <cctype>
#include "HuffmanNode.h"
#include "huffmanutil.h"

static void printHuffmanNode(ostream& out, const HuffmanNode& node, bool showAddress = false);

HuffmanNode::HuffmanNode(int character, int count, HuffmanNode* zero, HuffmanNode* one) {
    this->character = character;
    this->count = count;
    this->zero = zero;
    this->one = one;
}

bool HuffmanNode::isLeaf() const {
    return zero == nullptr && one == nullptr;
}

string HuffmanNode::toString() const {
    ostringstream out;
    out << *this;
    return out.str();
}

bool HuffmanNode::operator <(const HuffmanNode &rhs) const {
    return this->count > rhs.count;
}

void printSideways(HuffmanNode* node, bool showAddresses, string indent) {
    if (node != nullptr) {
        printSideways(node->one, showAddresses, indent + "  ");
        cout << indent << *node << endl;
        printSideways(node->zero, showAddresses, indent + "  ");
    }
}

ostream& operator <<(ostream& out, const HuffmanNode& node) {
    printHuffmanNode(out, node, false);
    return out;
}

static void printHuffmanNode(ostream& out, const HuffmanNode& node, bool showAddress) {
    if (showAddress) {
        out << "@" << &node;
    }
    out << "{";

    if (node.character == NOT_A_CHAR) {
        out << "NOT, ";
    } else {
        out << toPrintableChar(node.character)
            << " (" << node.character << "), ";
    }
    out << "count=" << node.count;

    if (showAddress) {
        if (node.zero == nullptr) {
            out << ", zero=nullptr";
        } else {
            out << ", zero=" << node.zero;
        }
        if (node.one == nullptr) {
            out << ", one=nullptr";
        } else {
            out << ", one=" << node.one;
        }
    }
    out << "}";
}
