#include <iostream>
#include <vector>
#include "SymbolTable.h"
#include "Symbol.h"
class Exer
{
private:
    std::vector<InterCode> codes;
    int pc;
    SymbolTable table;
    int level;

public:
    Exer()
    {
        this->pc = 0;
        this->level = 0;
    }

    void exe(std::vector<InterCode> interdcodes)
    {
        this->codes = interdcodes;
        int length = this->codes.size();
        while (this->pc < length)
        {
            exeCode();
        }
    }

    void exeCode()
    {
        InterCode code = this->codes[this->pc];
        if (code.ONE == "jmp")
        {
            exeJMP();
        }
        else if (code.ONE == "read")
        {
            exeREAD();
        }
        else if (code.ONE == "write")
        {
            exeWRITE();
        }
        else if (code.ONE == "in")
        {
            exeIN();
        }
        else if (code.ONE == "out")
        {
            exeOUT();
        }
        else if (code.ONE == "int")
        {
            exeINT();
        }
        else if (code.ONE == "real")
        {
            exeREAL();
        }
        else if (code.ONE == "assign")
        {
            exeASSIGN();
        }
        else if (code.ONE == "comp")
        {
            exeCOMP();
        }
    }

    void exeJMP()
    {
        InterCode code = this->codes[this->pc];
        this->pc = std::stoi(code.FOUR);
    }

    void exeREAD()
    {
        InterCode code = this->codes[this->pc];
        std::string x;
        std::cout << "please input:";
        std::cin >> x;
        std::string val = code.FOUR;
        Symbol symbol = this->table.SearchSymbolByName(val);
        if (symbol.type == "int")
        {
            symbol.value = std::stoi(x);
        }
        else if (symbol.type == "real")
        {
            symbol.value = std::stof(x);
        }
        this->pc += 1;
    }

    void exeWRITE()
    {
        InterCode code = this->codes[this->pc];
        std::string val;
        try
        {
            val = replaceSymbol(code.FOUR);
        }
        catch (...)
        {
            val = "err";
        }
        std::cout << val << std::endl;
        this->pc += 1;
    }

    void exeIN()
    {
        this->level += 1;
        this->pc += 1;
    }

    void exeOUT()
    {
        this->table.DeleteSymbolByLevel(this->level);
        this->level -= 1;
        this->pc += 1;
    }

    void exeINT()
    {
        InterCode code = this->codes[this->pc];
        Symbol symbol;
        symbol.level = this->level;
        if (code.THREE == "null")
        {
            symbol.type = "int";
        }
        else
        {
            symbol.type = "INTARRAY";
            symbol.length = std::stoi(code.THREE);
        }
        symbol.name = code.FOUR;
        float val;
        if (code.TWO == "null")
        {
            val = NULL;
        }
        else
        {
            val = std::stof(replaceSymbol(code.TWO));
        }
        symbol.value = val;
        this->table.AppendSymbol(symbol);
        this->pc += 1;
    }

    void exeREAL()
    {
        InterCode code = this->codes[this->pc];
        Symbol symbol;
        symbol.level = this->level;
        if (code.THREE == "null")
        {
            symbol.type = "real";
        }
        else
        {
            symbol.type = "REALARRAY";
            symbol.length = std::stoi(code.THREE);
        }
        symbol.name = code.FOUR;
        symbol.value = std::stof(code.TWO);
        this->table.AppendSymbol(symbol);
        this->pc += 1;
    }

    void exeASSIGN()
    {
        InterCode code = this->codes[this->pc];
        Symbol symbol = this->table.SearchSymbolByName(code.TWO);
        float val = std::stof(replaceSymbol(code.FOUR));
        symbol.value = val;
        this->pc += 1;
    }

    void exeCOMP()
    {
        InterCode code = this->codes[this->pc];
        bool val = std::stoi(replaceSymbol(code.TWO));
        if (val)
        {
            this->pc += 1;
        }
        else
        {
            this->pc = std::stoi(code.FOUR);
        }
    }

    std::string replaceSymbol(std::string str)
    {
        std::string result = str;
        std::vector<Symbol> nowsymbols = this->table.GetNowSymbol();
        for (Symbol symbol : nowsymbols)
        {
            if (symbol.level <= this->level)
            {
                if (result.find(symbol.name) != std::string::npos)
                {
                    result = replaceAll(result, symbol.name, std::to_string(symbol.value));
                }
            }
        }
        return result;
    }

    std::string replaceAll(std::string str, const std::string &from, const std::string &to)
    {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }
};
