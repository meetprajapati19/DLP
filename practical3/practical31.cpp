#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_set>
#include <vector>

using namespace std;

unordered_set<string> keywords = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", 
                                   "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", 
                                   "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", 
                                   "union", "unsigned", "void", "volatile", "while"};

unordered_set<char> operators = {'+', '-', '*', '/', '=', '%', '>', '<', '!', '&', '|', '^'};

unordered_set<char> punctuation = {';', ',', '(', ')', '{', '}', '[', ']'};

bool isKeyword(const string& word) {
    return keywords.find(word) != keywords.end();
}

bool isIdentifier(const string& word) {
    if (word.empty()) return false;
    if (!isalpha(word[0]) && word[0] != '_') return false;
    for (size_t i = 1; i < word.size(); ++i) {
        if (!isalnum(word[i]) && word[i] != '_') return false;
    }
    return true;
}

bool isConstant(const string& word) {
    if (word.empty()) return false;
    for (char c : word) {
        if (!isdigit(c)) return false;
    }
    return true;
}

void lexicalAnalyzer(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return;
    }

    string token;
    char ch;
    vector<string> symbolTable;
    vector<string> errors;

    cout << "TOKENS\n";
    while (file.get(ch)) {

        if (isspace(ch)) continue;

        
        if (ch == '/') {
            char nextChar;
            if (file.get(nextChar)) {
                if (nextChar == '/') {
                    while (file.get(ch) && ch != '\n');
                    continue;
                } else if (nextChar == '*') {
                    while (file.get(ch)) {
                        if (ch == '*') {
                            if (file.get(nextChar) && nextChar == '/') break;
                        }
                    }
                    continue;
                } else {
                    file.unget();
                }
            }
        }

        
        if (punctuation.find(ch) != punctuation.end()) {
            cout << "Punctuation: " << ch << endl;
            continue;
        }

       
        if (operators.find(ch) != operators.end()) {
            cout << "Operator: " << ch << endl;
            continue;
        }

        
        if (isalnum(ch) || ch == '_') {
            token = ch;
            while (file.get(ch) && (isalnum(ch) || ch == '_')) {
                token += ch;
            }
            file.unget();

            if (isKeyword(token)) {
                cout << "Keyword: " << token << endl;
            } else if (isConstant(token)) {
                cout << "Constant: " << token << endl;
            } else if (isIdentifier(token)) {
                cout << "Identifier: " << token << endl;
                symbolTable.push_back(token);
            } else {
                errors.push_back(token + " invalid lexeme");
            }
            continue;
        }

      
        if (ch == '\'') {
            token = "";
            while (file.get(ch) && ch != '\'') {
                token += ch;
            }
            cout << "Character: '" << token << "'" << endl;
            continue;
        }
    }

    file.close();

    if (!errors.empty()) {
        cout << "\nLEXICAL ERRORS" << endl;
        for (const auto& err : errors) {
            cout << err << endl;
        }
    }

    if (!symbolTable.empty()) {
        cout << "\nSYMBOL TABLE ENTRIES" << endl;
        int index = 1;
        for (const auto& id : symbolTable) {
            cout << index++ << ") " << id << endl;
        }
    }
}

int main() {
    string filename = "practical3.txt"; 
    lexicalAnalyzer(filename);
    return 0;
}
