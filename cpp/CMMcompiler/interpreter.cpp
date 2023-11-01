#include "lexer.h"
#include "parser.h"
#include "sem_analyser.h"
#include "exer.h"

class Interpreter {
public:
    Interpreter() {
        lexer = new Lexer();
        parser = new Parser();
        analyser = new SemAnalyser();
        exer = new Exer();
    }

    void run(std::string data) {
        lexer->ImportData(data);
        auto tokens = lexer->GetAllTokens();
        auto root = parser->ParserTokens(tokens);
        auto codes = analyser->run(root);
        exer->exe(codes);
    }

private:
    Lexer* lexer;
    Parser* parser;
    SemAnalyser* analyser;
    Exer* exer;
};
