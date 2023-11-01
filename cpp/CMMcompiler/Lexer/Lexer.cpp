#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.h"
#include "FSM.h"

class Lexer
{
private:
    std::vector<std::string> NowFile;
    int NowLineIndex;
    int NowColIndex;
    int linesCount;
    std::vector<std::string> keywords;
    std::unordered_map<std::string, int> ops;
    std::unordered_map<std::string, std::vector<std::string>> table;
    FSM fsm;

public:
    Lexer()
    {
        NowFile = std::vector<std::string>();
        NowLineIndex = 0;
        NowColIndex = 0;
        linesCount = 0;
        keywords = {"if", "else", "while", "read", "write", "int", "real"};
        ops = {{"letter", 0}, {"digit", 1}, {"+-", 2}, {"=", 3}, {"*", 4}, {"/", 5}, {"<>", 6}, {"_", 7}, {".", 8}, {"delimiter", 9}, {" ", 10}, {"\n", 11}, {"illegal", 12}};
        // TODO: read this table from file
        std::unordered_map<std::string, std::vector<std::string>> table = {
            {"start", {"keyorid", "int", "+-", "equal", "star", "slash", "<>", "keyorid", "error", "delimiter", "start", "start", "error"}},
            {"keyorid", {"keyorid", "keyorid", "success", "success", "success", "success", "success", "keyorid", "error", "success", "success", "success", "error"}},
            {"int", {"error", "int", "success", "success", "success", "success", "success", "error", "real", "success", "success", "success", "error"}},
            {"real", {"error", "real", "success", "success", "success", "success", "success", "error", "error", "success", "success", "success", "error"}},
            {"AorM", {"success", "success", "error", "error", "error", "error", "error", "success", "error", "success", "success", "success", "error"}},
            {"equal", {"success", "success", "success", "Dequal", "error", "error", "error", "success", "error", "success", "success", "success", "error"}},
            {"Dequal", {"success", "success", "success", "error", "error", "error", "error", "success", "error", "success", "success", "success", "error"}},
            {"star", {"success", "success", "success", "error", "error", "error", "error", "success", "error", "success", "success", "success", "error"}},
            {"slash", {"success", "success", "success", "error", "slash *", "ss", "error", "success", "error", "success", "success", "success", "error"}},
            {"slash *", {"slash *", "slash *", "slash *", "slash *", "s * *", "slash *", "slash *", "slash *", "slash *", "slash *", "slash *", "slash *", "slash *"}},
            {"s * *", {"slash *", "slash *", "slash *", "slash *", "s * *", "s**s", "slash *", "slash *", "slash *", "slash *", "slash *", "slash *", "slash *"}},
            {"s**s", {"success", "success", "success", "success", "success", "success", "success", "success", "success", "success", "success", "success", "error"}},
            {"ss", {"ss", "ss", "ss", "ss", "ss", "ss", "ss", "ss", "ss", "ss", "ss", "ss done", "ss"}},
            {"ss done", {"success", "success", "success", "success", "success", "success", "success", "success", "success", "success", "success", "success", "error"}},
            {"BS", {"success", "success", "success", "<>=", "error", "error", "error", "success", "error", "success", "success", "success", "error"}},
            {"BSE", {"success", "success", "success", "error", "error", "error", "error", "success", "error", "success", "success", "success", "error"}},
            {"delimiter", {"success", "success", "success", "success", "error", "success", "error", "success", "error", "success", "success", "success", "error"}}};
        fsm = FSM(ops, table);
    }

    void OpenFile(std::string path)
    {
        std::ifstream file(path);
        if (file.is_open())
        {
            std::string line;
            while (std::getline(file, line))
            {
                NowFile.push_back(line);
            }
            NowLineIndex = 0;
            NowColIndex = 0;
            linesCount = NowFile.size();
            file.close();
        }
        else
        {
            std::cout << "Unable to open file" << std::endl;
        }
    }

    void ImportData(std::vector<std::string> data)
    {
        NowFile = data;
        NowLineIndex = 0;
        NowColIndex = 0;
        linesCount = NowFile.size();
    }

    std::vector<Token> GetAllTokens()
    {
        std::vector<Token> result;
        Token token = GetToken();
        while (token.GetValue() != "-1")
        {
            result.push_back(token);
            token = GetToken();
        }
        for (auto it = result.begin(); it != result.end();)
        {
            if (it->GetType() == "注释")
            {
                it = result.erase(it);
            }
            else
            {
                ++it;
            }
        }
        return result;
    }

    Token GetToken()
    {
        std::string temp = ""; // Temporary storage for the read characters
        Token result;
        fsm.restart();            // Reset the state machine
        char nowchar = GetChar(); // Get the next character
        if (nowchar == -1)
        { // Check if it is EOF, i.e., -1
            rollback();
            result.SetValue("-1");
            return result;
        }
        while (nowchar != -1)
        {
            std::string move = GetType(nowchar);           // Get the operator type
            std::string nextstate = fsm.MoveByInput(move); // State machine changes state
            if (nextstate == "success")
            { // State is successful, generate token
                std::string success_state = fsm.NowState;
                std::string nowtype = GetTokenType(success_state, temp);
                std::string id = temp;

                // Warning handling
                if (nowtype == "Identifier" && id.back() == '_')
                {
                    result.SetType("error");
                    result.SetValue("Identifier ends with underscore");
                    rollback();
                    auto pos = rollbackbynum(temp.size());
                    int line = pos.first;
                    int col = pos.second;
                    int bias = GetBias(temp);
                    if (bias > 0)
                    {
                        result.SetColNum(1);
                    }
                    else
                    {
                        result.SetColNum(col + 1);
                    }
                    result.SetLineNum(line + 1 + bias);
                }
                else
                {
                    result.SetType(nowtype);
                    result.SetValue(temp);
                    rollback();
                    auto pos = rollbackbynum(temp.size());
                    int line = pos.first;
                    int col = pos.second;
                    int bias = GetBias(temp);
                    if (bias > 0)
                    {
                        result.SetColNum(1);
                    }
                    else
                    {
                        result.SetColNum(col + 1);
                    }
                    result.SetLineNum(line + 1 + bias);
                }
                return result;
            }
            else if (nextstate == "error")
            { // State is error, refine error type
                if (move == "illegal")
                {
                    result.SetValue("Illegal character");
                }
                else if (fsm.NowState == "keyorid")
                {
                    result.SetValue("Illegal identifier");
                }
                else
                {
                    result.SetValue("error");
                }
                result.SetType("error");
                auto pos = rollbackbynum(temp.size() + 1);
                int line = pos.first;
                int col = pos.second;
                int bias = GetBias(temp);
                if (bias > 0)
                {
                    result.SetColNum(1);
                }
                else
                {
                    result.SetColNum(col + 1);
                }
                result.SetLineNum(line + 1 + bias);
                return result;
            }
            else
            { // State is neither successful nor in error, continue to read characters
                fsm.ChangeState(nextstate);
                temp += nowchar;
                nowchar = GetChar();
            }
        }
        std::string success_state = fsm.NowState;
        if (success_state == "s * *" || success_state == "slash *")
        {
            result.SetType("error");
            result.SetValue("Unclosed comment block");
            auto pos = rollbackbynum(temp.size());
            int line = pos.first;
            int col = pos.second;
            int bias = GetBias(temp);
            if (bias > 0)
            {
                result.SetColNum(1);
            }
            else
            {
                result.SetColNum(col + 1);
            }
            result.SetLineNum(line + 1 + bias);
        }
        else
        {
            std::string nowtype = GetTokenType(success_state, temp);
            result.SetType(nowtype);
            result.SetValue(temp);
            auto pos = rollbackbynum(temp.size());
            int line = pos.first;
            int col = pos.second;
            int bias = GetBias(temp);
            if (bias > 0)
            {
                result.SetColNum(1);
            }
            else
            {
                result.SetColNum(col + 1);
            }
            result.SetLineNum(line + 1 + bias);
        }
        return result;
    }

    std::string GetTokenType(std::string state, std::string value)
    {
        // Based on the state machine, determine the token type
        std::string result = "";
        if (state == "keyorid")
        {
            if (value.find_first_not_of(" \t\n") == std::string::npos)
            {
                result = "Identifier";
            }
            else if (std::find(keywords.begin(), keywords.end(), value) != keywords.end())
            {
                result = "Keyword";
            }
            else
            {
                result = "Identifier";
            }
        }
        else if (state == "int")
        {
            result = "Integer";
        }
        else if (state == "real")
        {
            result = "Real";
        }
        else if (state == "+-" || state == "equal" || state == "star" || state == "slash")
        {
            result = "Arithmetic Operator";
        }
        else if (state == "s**s" || state == "ss done")
        {
            result = "Comment";
        }
        else if (state == "Dequal" || state == "<>" || state == "<>=")
        {
            result = "Comparison Operator";
        }
        else if (state == "delimiter")
        {
            result = "Delimiter";
        }
        return result;
    }

    std::string GetType(char character)
    {
        // Determine the type of the character
        if (isalpha(character))
        {
            return "letter";
        }
        else if (isdigit(character))
        {
            return "digit";
        }
        else if (character == '+' || character == '-')
        {
            return "+-";
        }
        else if (character == '=')
        {
            return "=";
        }
        else if (character == '*')
        {
            return "*";
        }
        else if (character == '/')
        {
            return "/";
        }
        else if (character == '<' || character == '>')
        {
            return "<>";
        }
        else if (character == '.')
        {
            return ".";
        }
        else if (character == '_')
        {
            return "_";
        }
        else if (character == ' ' || character == '\t')
        {
            return " ";
        }
        else if (character == '\n')
        {
            return "\n";
        }
        else if (character == '(' || character == ')' || character == '{' || character == '}' ||
                 character == '[' || character == ']' || character == ';')
        {
            return "delimiter";
        }
        else
        {
            return "illegal";
        }
    }

    char GetChar()
    {
        // Read a character from the file and move the file pointer
        if (NowLineIndex > linesCount - 1)
        {
            return -1;
        }
        std::string nowLine = NowFile[NowLineIndex];
        size_t nowlength = nowLine.length();
        char result = nowLine[NowColIndex];

        NowColIndex += 1;

        if (NowColIndex > nowlength - 1)
        {
            NowColIndex = 0;
            NowLineIndex += 1;
        }
        return result;
    }

    void rollback()
    {
        // Move the file pointer back by one character
        NowColIndex -= 1;
        if (NowColIndex < 0)
        {
            NowLineIndex -= 1;
            NowColIndex = NowFile[NowLineIndex].length() - 1;
        }
    }

    std::pair<int, int> GetRollBack(int lineindex, int colindex)
    {
        int nowCol = colindex;
        int nowLine = lineindex;
        nowCol -= 1;
        if (nowCol < 0)
        {
            nowLine -= 1;
            nowCol = NowFile[nowLine].length() - 1;
        }
        return std::make_pair(nowLine, nowCol);
    }

    std::pair<int, int> rollbackbynum(int num)
    {
        int line = NowLineIndex;
        int col = NowColIndex;
        for (int i = 0; i < num; i++)
        {
            std::pair<int, int> rollbackResult = GetRollBack(line, col);
            line = rollbackResult.first;
            col = rollbackResult.second;
        }
        return std::make_pair(line, col);
    }

    int GetBias(std::string aStr)
    {
        int result = 0;
        for (char &character : aStr)
        {
            if (character == '\n')
            {
                result++;
            }
            else
            {
                break;
            }
        }
        return result;
    }
};
