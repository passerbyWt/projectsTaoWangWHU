#include <iostream>
#include <vector>
#include <string>
#include "TreeNode.h"
class Token
{
public:
    std::string type;
    std::string value;

    TreeNode ParserTokens(std::vector<Token> tokens)
    {
        this->tokens = tokens;
        this->index = 0;
        this->root = TreeNode();
        this->root.type = "nonterminal";
        this->root.content = "program";
        Token NowToken = GetNowToken();
        while (NowToken.type != "eof")
        {
            this->root.childs.push_back(parseStmt());
            NowToken = GetNowToken();
        }
        return this->root;
    }
    TreeNode parseStmt()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "stmt";

        if (token.value == "if")
        {
            result.childs.push_back(parseIfStmt());
        }
        else if (token.value == "while")
        {
            result.childs.push_back(parseWhileStmt());
        }
        else if (token.value == "read")
        {
            result.childs.push_back(parseReadStmt());
        }
        else if (token.value == "write")
        {
            result.childs.push_back(parseWriteStmt());
        }
        else if (token.value == "int" || token.value == "real")
        {
            result.childs.push_back(parseDeclareStmt());
        }
        else if (token.value == "{")
        {
            result.childs.push_back(parseStmtBlock());
        }
        else if (token.type == "标识符")
        {
            result.childs.push_back(parseAssignStmt());
        }
        else
        {
            TreeNode temp;
            temp.type = "error";
            temp.content = "出错！不应出现的Token";
            result.childs.push_back(temp);
            Next();
        }

        return result;
    }
    TreeNode parseIfStmt()
    {
        Token token = GetNowToken();
        TreeNode result;
        try
        {
            result.type = "nonterminal";
            result.content = "if_stmt";
            result.childs.push_back(parseTerminalByValue({"if"}));
            result.childs.push_back(parseTerminalByValue({"("}));
            result.childs.push_back(parseExp());
            result.childs.push_back(parseTerminalByValue({")"}));
            result.childs.push_back(parseStmtBlock_unexcept({"else"}));
            token = GetNowToken();

            if (token.value == "else")
            {
                result.childs.push_back(parseTerminalByValue({"else"}));
                result.childs.push_back(parseStmtBlock());
            }
        }
        catch (...)
        {
            result.type = "error";
            result.content = "if语句出错！";
            Next();
        }

        return result;
    }
    TreeNode parseWhileStmt()
    {
        Token token = GetNowToken();
        TreeNode result;
        try
        {
            result.type = "nonterminal";
            result.content = "while_stmt";
            result.childs.push_back(parseTerminalByValue({"while"}));
            result.childs.push_back(parseTerminalByValue({"("}));
            result.childs.push_back(parseExp());
            result.childs.push_back(parseTerminalByValue({")"}));

            result.childs.push_back(parseStmtBlock_unexcept({"else"}));
        }
        catch (...)
        {
            result.type = "error";
            result.content = "while语句出错！";
            Next();
        }
        return result;
    }
    TreeNode parseReadStmt()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "read_stmt";
        result.childs.push_back(parseTerminalByValue({"read"}));
        result.childs.push_back(parseExp());
        result.childs.push_back(parseTerminalByValue({";"}));
        return result;
    }

    TreeNode parseWriteStmt()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "write_stmt";
        result.childs.push_back(parseTerminalByValue({"write"}));
        result.childs.push_back(parseExp());
        result.childs.push_back(parseTerminalByValue({";"}));
        return result;
    }

    TreeNode parseDeclareStmt()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "declare_stmt";
        result.childs.push_back(parseTerminalByValue({"int", "real"}));
        result.childs.push_back(parseTerminalByType({"标识符"}));
        token = GetNowToken();
        if (token.value == "=")
        {
            result.childs.push_back(parseTerminalByValue({"="}));
            result.childs.push_back(parseExp());
        }
        else if (token.value == "[")
        {
            result.childs.push_back(parseTerminalByValue({"["}));
            result.childs.push_back(parseExp());
            result.childs.push_back(parseTerminalByValue({"]"}));
        }
        result.childs.push_back(parseTerminalByValue({";"}));
        return result;
    }
    TreeNode parseStmtBlock()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "stmt_block";
        result.childs.push_back(parseTerminalByValue({"{"}));
        token = GetNowToken();
        while (token.value != "}")
        {
            result.childs.push_back(parseStmt());
            token = GetNowToken();
        }
        result.childs.push_back(parseTerminalByValue({"}"}));
        return result;
    }

    TreeNode parseStmtBlock_unexcept(vector<string> unexcepts)
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "stmt_block";
        result.childs.push_back(parseTerminalByValue({"{"}));
        token = GetNowToken();
        int flag = 0;
        TreeNode errorNode;
        while (token.value != "}")
        {
            if (find(unexcepts.begin(), unexcepts.end(), token.value) != unexcepts.end())
            {
                flag = 1;
                errorNode = parseError("右界符缺失");
                break;
            }

            result.childs.push_back(parseStmt());
            token = GetNowToken();
        }
        if (flag == 1)
        {
            result.childs.push_back(errorNode);
        }
        else
        {
            result.childs.push_back(parseTerminalByValue({"}"}));
        }
        return result;
    }

    TreeNode parseAssignStmt()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "assign_stmt";
        result.childs.push_back(parseVariable());
        result.childs.push_back(parseTerminalByValue({"="}));
        result.childs.push_back(parseExp());
        result.childs.push_back(parseTerminalByValue({";"}));
        return result;
    }

    TreeNode parseVariable()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "variable";
        result.childs.push_back(parseTerminalByType({"标识符"}));
        token = GetNowToken();
        if (token.value == "[")
        {
            result.childs.push_back(parseTerminalByValue({"["}));
            result.childs.push_back(parseExp());
            result.childs.push_back(parseTerminalByValue({"]"}));
        }
        return result;
    }
    TreeNode parseExp()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "exp";
        result.childs.push_back(parseAddtive_exp());
        token = GetNowToken();
        if (find({"<", ">", ">=", "<=", "<>", "=="}.begin(), {"<", ">", ">=", "<=", "<>", "=="}.end(), token.value) != {"<", ">", ">=", "<=", "<>", "=="}.end())
        {
            result.childs.push_back(parseLogical_op());
            result.childs.push_back(parseAddtive_exp());
        }
        return result;
    }

    TreeNode parseAddtive_exp()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "addtive-exp";
        result.childs.push_back(parseTerm());
        token = GetNowToken();
        if (find({"+", "-"}.begin(), {"+", "-"}.end(), token.value) != {"+", "-"}.end())
        {
            result.childs.push_back(parseAdd_op());
            result.childs.push_back(parseAddtive_exp());
        }
        return result;
    }

    TreeNode parseTerm()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "term";

        result.childs.push_back(parseFactor());
        token = GetNowToken();
        if (find({"*", "/"}.begin(), {"*", "/"}.end(), token.value) != {"*", "/"}.end())
        {
            result.childs.push_back(parseMul_op());
            result.childs.push_back(parseTerm());
        }
        return result;
    }

    TreeNode parseFactor()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "factor";
        if (find({"整形", "浮点数"}.begin(), {"整形", "浮点数"}.end(), token.type) != {"整形", "浮点数"}.end())
        {
            result.childs.push_back(parseTerminalByType({"整形", "浮点数"}));
        }
        else if (token.type == "标识符")
        {
            result.childs.push_back(parseTerminalByType({"标识符"}));
        }
        else if (find({"+", "-"}.begin(), {"+", "-"}.end(), token.value) != {"+", "-"}.end())
        {
            result.childs.push_back(parseAdd_op());
        }
        return result;
    }
    TreeNode parseLogical_op()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "logical-op";
        result.childs.push_back(parseTerminalByValue({"<", ">", ">=", "<=", "<>", "=="}));

        return result;
    }

    TreeNode parseAdd_op()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "add-op";
        result.childs.push_back(parseTerminalByValue({"+", "-"}));

        return result;
    }

    TreeNode parseMul_op()
    {
        Token token = GetNowToken();
        TreeNode result;
        result.type = "nonterminal";
        result.content = "mul-op";
        result.childs.push_back(parseTerminalByValue({"*", "/"}));

        return result;
    }

    TreeNode parseError(string message)
    {
        Token token = GetNowToken();
        TreeNode result;

        result.type = "error";
        result.content = message;
        Next();

        return result;
    }
    TreeNode parseTerminalByValue(vector<string> accepts)
    {
        Token token = GetNowToken();
        TreeNode result;
        if (std::find(accepts.begin(), accepts.end(), token.value) == accepts.end())
        {
            result.type = "error";
            result.content = "ERROR!Token should be in " + token.value + "!";
        }
        else
        {
            result.type = "terminal";
            result.content = token.value;
            Next();
        }
        // result.print(0)
        return result;
    }

    TreeNode parseTerminalByType(vector<string> accepts)
    {
        Token token = GetNowToken();
        TreeNode result;
        if (std::find(accepts.begin(), accepts.end(), token.type) == accepts.end())
        {
            result.type = "error";
            result.content = "出错！Token类型应取自" + token.type + "中";
        }
        else
        {
            result.type = "terminal";
            result.content = token.value;
            Next();
        }
        // result.print(0)
        return result;
    }

    Token GetNowToken()
    {
        if (index >= tokens.size())
        {
            return Token();
        }
        Token result = tokens[index];
        // index+=1;
        return result;
    }

    void Next()
    {
        index += 1;
    }

    void RollBack()
    {
        index -= 1;
    }
};