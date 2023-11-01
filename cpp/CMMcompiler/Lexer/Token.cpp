#include <iostream>
#include <string>

class Token {
public:
    std::string type; // 类型
    std::string value; // 内容
    int ColNum; // 列号
    int LineNum; // 行号

    void Print() {
        // 输出token
        std::cout << "行号：" << LineNum << "\t列号：" << ColNum << "\t\t类型:" << type << "\t\t值：" << value << std::endl;
    }

    std::string GetOutput() {
        std::string result = "行号：" + std::to_string(LineNum) + "\t列号：" + std::to_string(ColNum) + "\t\t类型:" + type + "\t\t值：" + value + "\n";
        return result;
    }
};
