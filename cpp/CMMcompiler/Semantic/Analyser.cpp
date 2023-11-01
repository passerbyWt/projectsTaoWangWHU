#include "SymbolTable.h"
#include "InterCode.h"
#include "TreeNode.h"

class SemAnalyser
{
public:
    SemAnalyser()
    {
        root = nullptr;
        table = new SymbolTable();
        interCodes = new std::vector<InterCode>();
        error = new std::vector<std::string>();
        no = 0;
        level = 0;
    }


    std::pair<std::vector<InterCode>, std::vector<std::string>> SemAnalyser::run(TreeNode *root)
    {
        this->root = root;
        this->table = new SymbolTable();
        this->InterCodes = {};
        this->error = {};
        this->no = 0;
        this->level = 0;

        for (auto stmt : this->root->childs)
        {
            auto stmtResult = this->handleStmt(stmt);
            this->InterCodes.insert(this->InterCodes.end(), stmtResult.begin(), stmtResult.end());
        }

        return {this->InterCodes, this->error};
    }

    std::vector<InterCode> SemAnalyser::handleStmt(TreeNode *node)
    {
        std::vector<InterCode> result;
        TreeNode *SpeStmt = node->childs[0];
        std::string tag = SpeStmt->content;
        if (tag == "if_stmt")
        {
            // pass
            auto result_if = this->handleIf(SpeStmt);
            result.insert(result.end(), result_if.begin(), result_if.end());
        }
        else if (tag == "while_stmt")
        {
            //  pass
            auto result_while = this->handleWhile(SpeStmt);
            result.insert(result.end(), result_while.begin(), result_while.end());
        }
        else if (tag == "read_stmt")
        {
            // pass
            auto result_read = this->handleRead(SpeStmt);
            result.insert(result.end(), result_read.begin(), result_read.end());
        }
        else if (tag == "write_stmt")
        {
            // pass
            auto result_write = this->handleWrite(SpeStmt);
            result.insert(result.end(), result_write.begin(), result_write.end());
        }
        else if (tag == "declare_stmt")
        {
            auto result_declare = this->handleDeclare(SpeStmt);
            result.insert(result.end(), result_declare.begin(), result_declare.end());
        }
        else if (tag == "stmt_block")
        {
            // pass
            auto result_block = this->handleBlock(SpeStmt);
            result.insert(result.end(), result_block.begin(), result_block.end());
        }
        else if (tag == "assign_stmt")
        {
            // pass
            auto result_assign = this->handleAssign(SpeStmt);
            result.insert(result.end(), result_assign.begin(), result_assign.end());
        }

        return result;
    }
    std::vector<InterCode> SemAnalyser::handleIf(TreeNode *node)
    {
        std::vector<InterCode> result;
        InterCode jumpcode;
        int jumpno = this->no;
        std::string one = jumpcode.COMP;
        std::string two = this->handleExp(node->childs[2]);
        std::string three = "null";
        int unfour = 0;
        this->no += 1;

        std::vector<InterCode> block1 = this->handleBlock(node->childs[4]);
        unfour = this->no;
        jumpcode.SetCode(jumpno, one, two, three, unfour);
        result.push_back(jumpcode);
        result.insert(result.end(), block1.begin(), block1.end());
        if (node->childs.size() > 5)
        {
            std::vector<InterCode> block2 = this->handleBlock(node->childs[6]);
            result.insert(result.end(), block2.begin(), block2.end());
        }
        return result;
    }

    std::vector<InterCode> SemAnalyser::handleWhile(TreeNode *node)
    {
        std::vector<InterCode> result;
        InterCode jumpcode;
        int jumpno = this->no;
        std::string one = jumpcode.COMP;
        std::string two = this->handleExp(node->childs[2]);
        std::string three = "null";
        int unfour = 0;
        this->no += 1;

        std::vector<InterCode> block1 = this->handleBlock(node->childs[4]);

        InterCode recode;
        std::string reone = recode.JMP;
        std::string retwo = "null";
        std::string rethree = "null";

        int reno = this->no;
        this->no += 1;

        unfour = this->no;
        jumpcode.SetCode(jumpno, one, two, three, unfour);
        result.push_back(jumpcode);
        result.insert(result.end(), block1.begin(), block1.end());
        int refour = jumpno;
        recode.SetCode(reno, reone, retwo, rethree, refour);
        result.push_back(recode);
        return result;
    }

    std::vector<InterCode> SemAnalyser::handleRead(TreeNode *node)
    {
        std::vector<InterCode> result;
        InterCode code;

        std::string one = code.READ;
        std::string two = "null";
        std::string three = "null";

        std::string four = this->handleExp(node->childs[1]);
        code.SetCode(this->no, one, two, three, four);
        this->no += 1;
        result.push_back(code);
        return result;
    }
    std::vector<InterCode> SemAnalyser::handleWrite(TreeNode *node)
    {
        std::vector<InterCode> result;
        InterCode code;

        std::string one = code.WRITE;
        std::string two = "null";
        std::string three = "null";

        std::string four = this->handleExp(node->childs[1]);
        code.SetCode(this->no, one, two, three, four);
        this->no += 1;
        result.push_back(code);
        return result;
    }

    std::vector<InterCode> SemAnalyser::handleDeclare(TreeNode *node)
    {
        std::vector<InterCode> result;
        InterCode code;
        std::string vtype = node->childs[0]->content;
        std::string one;
        if (vtype == "int")
        {
            one = code.INT;
        }
        else if (vtype == "real")
        {
            one = code.REAL;
        }

        std::string four = node->childs[1]->content;

        Symbol symbol;
        symbol.level = this->level;
        symbol.type = one;
        symbol.name = four;
        symbol.value = "null";
        std::string two = "null";
        std::string three = "null";
        if (node->childs[2]->content == "=")
        {
            three = "null";
            two = this->handleExp(node->childs[3]);
            symbol.value = two;
        }
        else if (node->childs[2]->content == "[")
        {
            if (vtype == "int")
            {
                symbol.type = "INTARRAY";
            }
            else if (vtype == "real")
            {
                symbol.type = "REALARRAY";
            }
            three = this->handleExp(node->childs[3]);
            symbol.length = std::stoi(three);
            two = "null";
        }

        this->table.AppendSymbol(symbol);
        code.SetCode(this->no, one, two, three, four);
        this->no += 1;
        result.push_back(code);
        return result;
    }

    std::string SemAnalyser::handleExp(TreeNode *node)
    {
        std::string origin = this->GetOrigin(node);
        std::vector<std::string> varlist = this->GetVar(origin);
        for (auto var : varlist)
        {
            int result = this->table.SearchSymbolByName(var);
            if (result == -1)
            {
                std::string message = std::to_string(this->no) + " 未声明的变量：" + var;
                this->error.push_back(message);
            }
            else if (result.value == "null")
            {
                std::string message = std::to_string(this->no) + " 未赋值的变量：" + var;
                this->error.push_back(message);
            }
        }
        return origin;
    }

    std::vector<InterCode> SemAnalyser::handleAssign(TreeNode *node)
    {
        std::vector<InterCode> result;
        InterCode code;
        std::string one = code.ASSIGN;

        std::string two = this->handleVariable(node->childs[0]);
        std::string n = two;
        int r = this->table.SearchSymbolByName(n);
        if (r != -1 && r.type in["INTARRAY", "REALARRAY"])
        {
            std::string message = std::to_string(this->no) + " 直接赋值数组变量";
            this->error.push_back(message);
        }

        std::string three = "null";

        std::string four = this->handleExp(node->childs[2]);
        code.SetCode(this->no, one, two, three, four);
        this->no += 1;
        result.push_back(code);
        return result;
    }
    std::string SemAnalyser::handleVariable(TreeNode *node)
    {
        std::string name = node->childs[0]->content;
        int temp = this->table.SearchSymbolByName(name);
        if (temp == -1)
        {
            std::string message = std::to_string(this->no) + " 未声明的变量：" + name;
            this->error.push_back(message);
        }
        if (node->childs.size() > 1 && node->childs[1]->content == "[")
        {
            TreeNode *exp = node->childs[2];
            std::string index = this->handleExp(exp);
            int num;
            try
            {
                num = std::stoi(index);
            }
            catch (std::invalid_argument &e)
            {
                num = -1;
            }
            if (num != -1)
            {
                if (temp.type != "INTARRAY" && temp.type != "REALARRAY")
                {
                    std::string message = std::to_string(this->no) + " 类型错误";
                    this->error.push_back(message);
                }
                else if (num > temp.length)
                {
                    std::string message = std::to_string(this->no) + " 数组越界";
                    this->error.push_back(message);
                }
            }
        }
        return this->GetOrigin(node);
    }

    std::vector<InterCode> SemAnalyser::handleBlock(TreeNode *node)
    {
        this->level += 1;
        std::vector<InterCode> result;
        InterCode code;
        std::string one = code.IN;
        std::string two = "null";
        std::string three = "null";
        std::string four = "null";
        code.SetCode(this->no, one, two, three, four);
        this->no += 1;
        result.push_back(code);

        for (size_t i = 1; i < node->childs.size() - 1; i++)
        {
            result.insert(result.end(), this->handleStmt(node->childs[i]).begin(), this->handleStmt(node->childs[i]).end());
        }

        code = InterCode();
        one = code.OUT;
        two = "null";
        three = "null";
        four = "null";
        code.SetCode(this->no, one, two, three, four);
        this->no += 1;
        result.push_back(code);
        this->table.DeleteSymbolByLevel(this->level);
        this->level -= 1;
        return result;
    }

    std::string SemAnalyser::GetOrigin(TreeNode *node)
    {
        if (node->childs.empty())
        {
            return node->content;
        }
        std::string result = "";
        for (auto child : node->childs)
        {
            result += this->GetOrigin(child);
        }

        return result;
    }

    std::vector<std::string> SemAnalyser::GetVar(std::string origin)
    {
        std::vector<std::string> result;
        std::string temp = "";
        for (char &c : origin)
        {
            if (!isalpha(c))
            {
                if (!temp.empty())
                {
                    result.push_back(temp);
                    temp = "";
                }
            }
            else
            {
                temp += c;
            }
        }
        return result;
    }
}