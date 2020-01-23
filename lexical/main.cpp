#include "token.hpp"
#include "tokenizer.hpp"
#include "symbol_table.hpp"

Tokenizer tokenizer;
SymbolTable symbolTable;
Token currentToken;
Token nextToken;
int cur = 0;
int next = 1;

void analyze();
void statement();
void expression();
void expressionz();
void syntaxError(std::string lexeme);
void assignmentExpression();

void printToken(std::string token, int lineNumber, std::string type) {
    std::cout << lineNumber << ": token = \"" << token << "\"" << " type = " << type << "\n";
}

void printAllTokens(std::string fileName){
    
    Token a = tokenizer.getToken();
    a.print();
    while(a.type != "EOF"){
        tokenizer.next();
        a = tokenizer.getToken();
        a.print();
    }
}

void syntaxError(std::string lexeme){
    std::cout << lexeme;
}

void analyze(){
    statement();
}

void statement(){
    if(false){
        
    }
    else{
        expression();
        if(!tokenizer.isSemiColon()){
            syntaxError(tokenizer.lexeme());
        }
    }
}

void expression(){
    if(tokenizer.isIdentifier()){
        tokenizer.next();
        if(tokenizer.isExpressionZ()){
            expressionz();
        }
    }
    else{
        
    }
}

void expressionz(){
    if(!tokenizer.isExpressionZ())
        syntaxError(tokenizer.lexeme());
    else{
        if(tokenizer.lexeme() == "="){
            tokenizer.next();
            assignmentExpression();
        }
        else{
            expression();
        }
    }
}

void assignmentExpression(){
    expression();
}

int main(int argc, char* argv[]) {
    std::string fileName = "Resource/test.kxi";
    if (argv[1]) {
        fileName = argv[1];
    }
    
    tokenizer = Tokenizer(fileName);
    symbolTable = symbolTable;
    //printAllTokens(fileName);
    analyze();
    return 0;
}
