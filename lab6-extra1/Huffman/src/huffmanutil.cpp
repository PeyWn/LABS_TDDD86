/*
 * TDDD86 Huffman Encoding
 * This file defines various utility functions used by the main client program.
 * See huffmanutil.h for documentation of each function.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 */

#include "huffmanutil.h"
#include "bitstream.h"
#include "strlib.h"
#include "simpio.h"

string bitsToBytes(string text) {
    istringbitstream input(text);
    ostringstream out;
    while (true) {
        int bit = input.readBit();
        if (input.fail()) {
            break;
        }
        out.put(bit == 1 ? '1' : '0');
    }
    return out.str();
}

string bytesToBits(string text) {
    ostringbitstream out;
    for (int i = 0; i < (int) text.length(); i++) {
        out.writeBit(text[i] == '1' ? 1 : 0);
    }
    return out.str();
}

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#else
    // assume POSIX
    #include <sys/stat.h>
#endif

bool confirmOverwrite(string filename) {
#if defined(_WIN32) || defined(_WIN64)
    bool fileExists = (GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES);
#else
    // assume POSIX
    struct stat fileInfo;
    bool fileExists = (stat(filename.c_str(), &fileInfo) == 0);
#endif
    if (!fileExists) {
        return true;
    } else {
        return yesOrNo(filename + " already exists.  Overwrite? (y/n) ");
    }
}

int fileSize(string filename) {
    ifstream input;
    input.open(filename.c_str(), ifstream::binary);
    input.seekg(0, ifstream::end);
    return (int) input.tellg();
}

void printBits(string text) {
    istringbitstream input(text);
    int i = 0;
    while (true) {
        i++;
        int bit = input.readBit();
        if (input.fail()) break;
        cout << bit;
        if (i > 0 && i % 8 == 0) {
            cout << " ";
        }
        if (i > 0 && i % 64 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

string promptForExistingFileName(string prompt) {
    while (true) {
        string filename = getLine(prompt);
#if defined(_WIN32) || defined(_WIN64)
        bool fileExists = (GetFileAttributesA(filename.c_str()) != INVALID_FILE_ATTRIBUTES);
#else
        // assume POSIX
        struct stat fileInfo;
        bool fileExists =  (stat(filename.c_str(), &fileInfo) == 0);
#endif
        if (fileExists) {
            return filename;
        } else {
            cout << "That file does not exist; please try again." << endl;
        }
    }
    return "";
}

string readEntireFileText(string filename) {
    ifstream input;
    input.open(filename.c_str());
    return readEntireFileText(input);
}

string readEntireFileText(istream& input) {
    ostringstream out;
    while (true) {
        int ch = input.get();
        if (input.fail()) {
            break;
        }
        out << (char) ch;
    }
    return out.str();
}

string stringReplace(string s, char oldChar, char newChar) {
    for (int i = (int) s.length() - 1; i >= 0; i--) {
        if (s[i] == oldChar) {
            s[i] = newChar;
        }
    }
    return s;
}

string stringReplace(string s, char oldChar, string newStr) {
    for (int i = (int) s.length() - 1; i >= 0; i--) {
        if (s[i] == oldChar) {
            s.erase(i, 1);
            if (newStr.length() > 0) {
                s.insert(i, newStr);
            }
        }
    }
    return s;
}

string stringReplace(string s, string oldStr, string newStr) {
    int l2 = oldStr.length();
    for (int i = (int) (s.length() - l2); i >= 0; i--) {
        if (s.substr(i, l2) == oldStr) {
            s.replace(i, l2, newStr);
        }
    }
    return s;
}

/*
 * Returns a printable string for the given character.  See bitstream.h.
 */
string toPrintableChar(int ch) {
    if (ch == '\n') {
        return "'\\n'";
    } else if (ch == '\t') {
        return "'\\t'";
    } else if (ch == '\r') {
        return "'\\r'";
    } else if (ch == '\f') {
        return "'\\f'";
    } else if (ch == '\b') {
        return "'\\b'";
    } else if (ch == '\0') {
        return "'\\0'";
    } else if (ch == ' ') {
        return "' '";
    } else if (ch == (int) PSEUDO_EOF) {
        return "EOF";
    } else if (ch == (int) NOT_A_CHAR) {
        return "NONE";
    } else if (!isgraph(ch)) {
        return "???";
    } else {
        return string("'") + (char) ch + string("'");
    }
}

bool yesOrNo(string prompt) {
    while (true) {
        string answer = trim(toLowerCase(getLine(prompt)));
        if (startsWith(answer, 'y')) {
            return true;
        } else if (startsWith(answer, 'n')) {
            return false;
        } else {
            cout << "Please type a word that begins with 'y' or 'n'." << endl;
        }
    }
}

string getRoot(string filename) {
   int dot = -1;
   int len = filename.length();
   for (int i = 0; i < len; i++) {
      char ch = filename[i];
      if (ch == '.') dot = i;
      if (ch == '/' || ch == '\\') dot = -1;
   }
   if (dot == -1) {
      return filename;
   } else {
      return filename.substr(0, dot);
   }
}
