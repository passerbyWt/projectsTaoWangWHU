#include <iostream>
#include <string>
#include <unordered_map>

class FSM {
private:
    std::string NowState;
    std::unordered_map<std::string, std::unordered_map<char, std::string>> StatesTable;
    std::string ops;

public:
    FSM(std::string ops, std::unordered_map<std::string, std::unordered_map<char, std::string>> table) {
        //start
        NowState = "start";
        StatesTable = table;
        this->ops = ops;
    }

    std::string MoveByInput(char Input) {
        //get the next input
        std::string NextState = StatesTable[NowState][Input];
        return NextState;
    }

    void ChangeState(std::string state) {
        NowState = state;
    }

    void restart() {
        NowState = "start";
    }
};

