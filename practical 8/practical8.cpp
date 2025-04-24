#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<string, vector<string>> grammar;
map<string, set<string>> firstSets, followSets;
map<string, map<string, string>> parsingTable;
string startSymbol;

// Function to compute FIRST sets
void computeFirst() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto it = grammar.begin(); it != grammar.end(); ++it) {
            string non_terminal = it->first;
            vector<string> productions = it->second;

            for (const string& production : productions) {
                string firstSymbol = string(1, production[0]);

                if (!isupper(firstSymbol[0])) { // Terminal
                    if (firstSets[non_terminal].insert(firstSymbol).second)
                        changed = true;
                } else { // Non-terminal
                    for (string f : firstSets[firstSymbol]) {
                        if (firstSets[non_terminal].insert(f).second)
                            changed = true;
                    }
                }
            }
        }
    }
}

// Function to compute FOLLOW sets
void computeFollow() {
    followSets[startSymbol].insert("$"); // End marker

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto it = grammar.begin(); it != grammar.end(); ++it) {
            string non_terminal = it->first;
            vector<string> productions = it->second;

            for (const string& production : productions) {
                for (size_t i = 0; i < production.size(); ++i) {
                    string symbol = string(1, production[i]);

                    if (isupper(symbol[0])) { // Non-terminal
                        set<string> firstOfNext;
                        if (i + 1 < production.size()) {
                            string nextSymbol = string(1, production[i + 1]);
                            if (!isupper(nextSymbol[0])) // Terminal
                                firstOfNext.insert(nextSymbol);
                            else
                                firstOfNext = firstSets[nextSymbol];
                        } else {
                            firstOfNext = followSets[non_terminal];
                        }

                        for (const string& f : firstOfNext) {
                            if (followSets[symbol].insert(f).second)
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}

// Function to construct the parsing table
bool constructParsingTable() {
    for (auto it = grammar.begin(); it != grammar.end(); ++it) {
        string non_terminal = it->first;
        vector<string> productions = it->second;

        for (const string& production : productions) {
            string firstSymbol = string(1, production[0]);

            if (!isupper(firstSymbol[0])) {
                parsingTable[non_terminal][firstSymbol] = production;
            } else {
                for (const string& f : firstSets[firstSymbol]) {
                    parsingTable[non_terminal][f] = production;
                }
            }
        }
    }
    return true;
}

// Function to print sets
void printSets(const map<string, set<string>>& sets, const string& setName) {
    cout << setName << " Sets:\n";
    for (auto it = sets.begin(); it != sets.end(); ++it) {
        cout << it->first << " : { ";
        for (const string& s : it->second) {
            cout << s << " ";
        }
        cout << "}\n";
    }
}

int main() {
    int numRules;
    cout << "Enter the number of grammar rules: ";
    cin >> numRules;
    cin.ignore();

    cout << "Enter grammar rules (e.g., S->AB, A->a):\n";
    for (int i = 0; i < numRules; i++) {
        string rule;
        getline(cin, rule);
        string non_terminal = string(1, rule[0]);
        string production = rule.substr(3);
        grammar[non_terminal].push_back(production);
    }

    cout << "Enter the start symbol: ";
    cin >> startSymbol;

    computeFirst();
    computeFollow();
    constructParsingTable();

    printSets(firstSets, "FIRST");
    printSets(followSets, "FOLLOW");

    cout << "\nParsing Table:\n";
    for (auto it = parsingTable.begin(); it != parsingTable.end(); ++it) {
        string non_terminal = it->first;
        cout << non_terminal << ":\n";
        for (auto jt = it->second.begin(); jt != it->second.end(); ++jt) {
            cout << "  " << jt->first << " -> " << jt->second << "\n";
        }
    }

    return 0;
}
