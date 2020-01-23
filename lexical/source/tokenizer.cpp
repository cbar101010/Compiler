//
//  tokenizer.cpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#include "tokenizer.hpp"


Tokenizer::Tokenizer(std::string fileName){
        this->fileName = fileName;
        this->matchCount = 0;
        this->lineNumber = 0;
        currentToken = Token();
        nextToken = Token();
        next();
    }
Tokenizer::Tokenizer(){}
    
std::string Tokenizer::getLine(){
        lineNumber++;
        std::string line;
    
        if(!myfile.is_open()){
            myfile.open(fileName);
            if(!myfile.is_open()){
                std::cout << "could not open file";
            }
        }
        if(std::getline(myfile, line)){
            return line;
        }
        return "EOF";
    }
    
void Tokenizer::tokenize(std::string line){
        while(line.empty()){
            line = getLine();
        }
        std::regex tokenizer("(\\d+|\\w+|\'\\\\n\'|==|!=|<=|>=|<<|>>|\'.\'|[\\S])");
        std::vector<std::vector<std::string>> currentVectors;
        std::smatch matches;
        std::sregex_iterator currentMatch(line.begin(), line.end(), tokenizer);
        std::sregex_iterator lastMatch;
        while (currentMatch != lastMatch) {
            std::smatch match = *currentMatch;
            this->matches.push_back(match.str());
            currentMatch++;
        }
    }
    
Token Tokenizer::getToken(){
        return this->currentToken;
    }
std::string Tokenizer::lexeme(){
    return this->currentToken.lexeme;
}
std::string Tokenizer::type(){
    return this->currentToken.type;
}
Token Tokenizer::peekToken(){
        return this->nextToken;
}
    
void Tokenizer::next(){
        std::string toke1;
        std::string toke2;
        
        if(currentToken.lineNumber < 1){
            tokenize(getLine());
            if(matches.size() == 1){
                toke1 = matches.at(0);
                currentToken = Token(toke1, assignToken(toke1), lineNumber);
                matches.clear();
                tokenize(getLine());
                toke2 = matches.at(0);
                nextToken = Token(toke2, assignToken(toke2), lineNumber);
            }
            else {
                toke1 = matches.at(0);
                currentToken = Token(toke1, assignToken(toke1), lineNumber);
                matches.erase(matches.begin());
                toke2 = matches.at(0);
                nextToken = Token(toke2, assignToken(toke2), lineNumber);
            }
        }
        else{
            currentToken = nextToken;
            //I don't think this would ever happen...
            if(matches.size() > 0){
                matches.erase(matches.begin());
            }
            
            while(matches.size() < 1){
                tokenize(getLine());
            }
            
            toke2 = matches.at(0);
            nextToken = Token(toke2, assignToken(toke2), lineNumber);
        }
    }

Token Tokenizer::retrieve(int x){
    if(x == 0){
        return currentToken;
    }
    else if(x == 1){
        return nextToken;
    }
    else{
        //throw error
    }
    return currentToken;
}

bool Tokenizer::isCharacterLiteral(int x){
    Token token = retrieve(x);
    if(token.type == "character"){
        return true;
    }
    return false;
}

bool Tokenizer::isExpressionZ(int x){
    Token token = retrieve(x);
    if(token.type == "relational" || token.type == "math" || token.type == "booleanExpression" || token.type == "logicalConnective" || token.lexeme == "="){
        return true;
    }
    return false;
}

bool Tokenizer::isSemiColon(int x){
    Token token = retrieve(x);
    if(token.lexeme == ";"){
        return true;
    }
    return false;
}

bool Tokenizer::isIdentifier(int x){
    Token token = retrieve(x);
    if(token.type == "identifier"){
        return true;
    }
    return false;
}

std::string Tokenizer::assignToken(std::string token){
    
        std::vector<std::string> protectedWords = { "atoi", "and", "bool", "block", "break", "case", "class",
            "char", "cin", "cout", "default", "else", "false", "if", "int",
            "itoa", "kxi2020", "lock", "main", "new", "null", "object", "or"
            ,"public", "private", "protected", "return", "release", "string"
            ,"spawn", "sym", "set", "switch", "this", "true", "thread",
            "unprotected", "unlock", "void", "while", "wait" };
        std::regex number("(\\d+)");
        std::regex word("(\\w+)");
        std::regex character("(\'.\')");
        std::regex newLineChar("(\'\\\\n\')");
        std::regex anythingelse("([\\S])");
        std::regex period("(\\.)");
        std::regex comma("(,)");
        std::regex semicolon("(;)");
        std::regex colon("(:)");
        std::regex mathOperator("([/+\\-*])");
        std::regex booleanExpression("(==|!=|<=|>=|and|or|>|<)");
        std::regex assignmentOperator("(=)");
        std::regex arrayBeginEnd("([\\[\\]])");
        std::regex blockBeginEnd("([{}])");
        std::regex openBlock("(\\{)");
        std::regex closeBlock("(\\})");
        std::regex arrayBegin("(\\[)");
        std::regex arrayEnd("(\\])");
        std::regex parenthesesOpenClose("([()])");
        std::regex parenopen("(\\()");
        std::regex parenclose("(\\))");
        std::regex ioOperators("(<<|>>)");
        if (std::regex_match(token, number)) {
            return("number");
        }
        else if (std::regex_match(token, word)) {
            if (std::find(protectedWords.begin(), protectedWords.end(), token) != protectedWords.end()) {
                if (std::regex_match(token, booleanExpression)) {
                    if(token == "and" || token == "or"){
                        return "logicalConnective";
                    }
                    return("booleanExpression");
                }
                return(token);
            }
            if(token == "EOF"){
                return "EOF";
            }
            return("identifier");
        }
        else if (std::regex_match(token, character)) {
            return("character");
        }
        else if (std::regex_match(token, period)) {
            return("period");
        }
        else if (std::regex_match(token, comma)) {
            return("comma");
        }
        else if (std::regex_match(token, semicolon)) {
            return("semicolon");
        }
        else if (std::regex_match(token, colon)) {
            return("colon");
        }
        else if (std::regex_match(token, mathOperator)) {
            return("math");
        }
        else if (std::regex_match(token, booleanExpression)) {
            return("relational");
        }
        else if (std::regex_match(token, newLineChar)) {
            return("nonprintable_ascii");
        }
        else if (std::regex_match(token, assignmentOperator)) {
            return("assignment_operator");
        }
        else if (std::regex_match(token, arrayBeginEnd)) {
            if (std::regex_match(token, arrayBegin)) {
                return("array_begin");
            }
            else if (std::regex_match(token, arrayEnd)) {
                return("array_end");
            }
        }
        else if (std::regex_match(token, blockBeginEnd)) {
            if (std::regex_match(token, openBlock)) {
                return("open_block");
            }
            else if (std::regex_match(token, closeBlock)) {
                return("close_block");
            }
        }
        else if (std::regex_match(token, parenthesesOpenClose)) {
            if (std::regex_match(token, parenopen)) {
                return("open_paren");
            }
            else if (std::regex_match(token, parenclose)) {
                return("close_paren");
            }
        }
        else if (std::regex_match(token, ioOperators)) {
            return("io_operator");
        }
        
        return "unknown";
    }

