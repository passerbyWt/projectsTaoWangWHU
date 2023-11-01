#include <string>

class InterCode {
public:
    std::string No;
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

    void SetCode(std::string no, std::string one, std::string two, std::string three, std::string four) {
        No = no;
        ONE = one;
        TWO = two;
        THREE = three;
        FOUR = four;
    }

    std::string toString() {
        std::string result = No + ": ";
        result += "(" + ONE + ",";
        result += TWO + ",";
        result += THREE + ",";
        result += FOUR + ")";
        return result;
    }
};
