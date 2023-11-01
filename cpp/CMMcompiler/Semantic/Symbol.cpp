#include <string>

class Symbol {
public:
    int level;
    std::string type;
    std::string name;
    std::string value;
    Symbol *ChildSymbol;
    int length;

    int GetLength() {
        if (type == "IntArray" || type == "FloatArray") {
            return value.length();
        }
        return -1;
    }

    Symbol* GetLastChild() {
        if (ChildSymbol == nullptr) {
            return this;
        }
        Symbol *now = ChildSymbol;
        while (now->ChildSymbol != nullptr) {
            now = now->ChildSymbol;
        }
        return now;
    }
};
