#ifndef INTERCODE_H
#define INTERCODE_H

#include <string>

class InterCode {
public:
    int No;
    std::string ONE;
    std::string TWO;
    std::string THREE;
    std::string FOUR;

    std::string JMP = "jmp";
    std::string READ = "read";
    std::string WRITE = "write";
    std::string IN = "in";
    std::string OUT = "out";
    std::string INT = "int";
    std::string REAL = "real";
    std::string ASSIGN = "assign";
    std::string PLUS = "+";
    std::string MINUS = "-";
    std::string MUL = "*";
    std::string DIV = "/";
    std::string GT = ">";
    std::string LT = "<";
    std::string GET = ">=";
    std::string LET = "<=";
    std::string EQ = "==";
    std::string NEQ = "<>";
    std::string COMP = "comp";

    void SetCode(int no, std::string one, std::string two, std::string three, std::string four);
    std::string toString();
};

#endif

#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>

class Symbol {
public:
    int level;
    std::string type;
    std::string name;
    std::string value;
    Symbol *ChildSymbol;
    int length;

    int GetLength();
    Symbol* GetLastChild();
};

#endif

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <vector>
#include "Symbol.h"

class SymbolTable {
public:
    std::vector<Symbol> symbols;

    void AppendSymbol(Symbol symbol);
    void DeleteSymbolByLevel(int level);
    void DeleteChildBylevel(Symbol &sym, int level);
    Symbol* SearchSymbolByName(std::string name);
    std::vector<Symbol*> GetNowSymbol();
};

#endif
