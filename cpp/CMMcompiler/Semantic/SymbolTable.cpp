#include <vector>
#include <string>

class Symbol
{
public:
    int level;
    std::string type;
    std::string name;
    std::string value;
    Symbol *ChildSymbol;
    int length;

    int GetLength()
    {
        if (type == "IntArray" || type == "FloatArray")
        {
            return value.length();
        }
        return -1;
    }

    Symbol *GetLastChild()
    {
        if (ChildSymbol == nullptr)
        {
            return this;
        }
        Symbol *now = ChildSymbol;
        while (now->ChildSymbol != nullptr)
        {
            now = now->ChildSymbol;
        }
        return now;
    }
};

class SymbolTable
{
public:
    std::vector<Symbol> symbols;

    void AppendSymbol(Symbol symbol)
    {
        int flag = 0;
        for (auto &sym : symbols)
        {
            if (sym.name == symbol.name)
            {
                flag = 1;
                if (sym.level < symbol.level)
                {
                    sym.ChildSymbol = &symbol;
                }
                else if (sym.level == symbol.level)
                {
                    symbols.erase(std::remove(symbols.begin(), symbols.end(), sym), symbols.end());
                    symbols.push_back(symbol);
                }
                break;
            }
        }

        if (flag == 0)
        {
            symbols.push_back(symbol);
        }
    }

    void DeleteSymbolByLevel(int level)
    {
        for (auto it = symbols.begin(); it != symbols.end();)
        {
            if (it->level == level)
            {
                it = symbols.erase(it);
            }
            else
            {
                DeleteChildBylevel(*it, level);
                ++it;
            }
        }
    }

    void DeleteChildBylevel(Symbol &sym, int level)
    {
        if (sym.ChildSymbol == nullptr)
        {
            return;
        }
        if (sym.ChildSymbol->level == level)
        {
            sym.ChildSymbol = nullptr;
        }
        else
        {
            DeleteChildBylevel(*sym.ChildSymbol, level);
        }
    }

    Symbol *SearchSymbolByName(std::string name)
    {
        Symbol *result = nullptr;
        if (name.find('[') != std::string::npos)
        {
            name = name.substr(0, name.find('['));
        }
        for (auto &sym : symbols)
        {
            if (sym.name == name)
            {
                result = sym.GetLastChild();
                break;
            }
        }
        return result;
    }

    std::vector<Symbol *> GetNowSymbol()
    {
        std::vector<Symbol *> result;
        for (auto &symbol : symbols)
        {
            if (symbol.name.find(symbol.name) == result.end())
            {
                result.push_back(&symbol);
            }
            else
            {
                if (symbol.level > result.back()->level)
                {
                    result.back() = &symbol;
                }
            }
        }
        return result;
    }
};
