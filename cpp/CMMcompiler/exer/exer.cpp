#include "Analyser.h"
#include <iostream>
#include <vector>
#include <string>

class SymbolTable {
    // Implement the SymbolTable class
};

class Symbol {
    // Implement the Symbol class
};

class Exer {
private:
    std::vector<Code> codes;
    int pc;
    SymbolTable table;
    int level;

public:
    Exer() {
        codes = {};
        pc = 0;
        level = 0;
    }

    void exe(std::vector<Code> interdcodes) {
        table = SymbolTable();
        level = 0;
        codes = interdcodes;
        int length = codes.size();
        pc = 0;
        while (pc < length) {
            exeCode();
        }
    }

    void exeCode() {
        Code code = codes[pc];
        if (code.ONE == code.JMP) {
            exeJMP();
        } else if (code.ONE == code.READ) {
            exeREAD();
        }
        // Add other conditions for different instructions
    }

    void exeJMP() {
        Code code = codes[pc];
        pc = stoi(code.FOUR);
    }

    void exeREAD() {
        Code code = codes[pc];
        std::string x;
        std::cout << "please input:";
        std::cin >> x;
        std::string val = code.FOUR;
        Symbol symbol = table.SearchSymbolByName(val);
        if (symbol.type == "int") {
            symbol.value = stoi(x);
        } else if (symbol.type == "real") {
            symbol.value = stof(x);
        }
        pc += 1;
    }
    // Implement other functions similarly
};

