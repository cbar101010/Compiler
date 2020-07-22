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
        thirdToken = Token();
        next();
    }
Tokenizer::Tokenizer(){}
    
std::string Tokenizer::getLine(){
        std::string line;
        lineNumber++;

        if(!myfile.is_open()){
            myfile.open(fileName);
            if(!myfile.is_open()){
                std::cout << "could not open file";
            }
        }
        if(std::getline(myfile, line)){
            lines.insert(std::pair<int, std::string>(lineNumber, line));
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
//hack for neg numbers
void Tokenizer::setCurLexeme(std::string newLex){
    this->currentToken.lexeme = newLex;
}
std::string Tokenizer::type(){
    return this->currentToken.type;
}
int Tokenizer::lineNum(){
    return this->currentToken.lineNumber;
}
Token Tokenizer::peekToken(){
        return this->nextToken;
}

Token Tokenizer::peek2Token(){
    return this->thirdToken;
}
void Tokenizer::next(){
        std::string toke1;
        std::string toke2;
        std::string toke3;
        
        if(firstLine){
            firstLine = false;
            tokenize(getLine());
            while(matches.size() > 1 && matches.at(0) == "/" and matches.at(1) == "/"){
                matches.clear();
                while(matches.size() < 1){
                    tokenize(getLine());
                }
            }
            
            if(matches.size() == 1){
                toke1 = matches.at(0);
                currentToken = Token(toke1, assignToken(toke1), lineNumber);
                matches.clear();
                tokenize(getLine());
                if(matches.size() == 1){
                    toke2 = matches.at(0);
                    nextToken = Token(toke2, assignToken(toke2), lineNumber);
                    matches.clear();
                    tokenize(getLine());
                    toke3 = matches.at(0);
                    thirdToken = Token(toke3, assignToken(toke3), lineNumber);
                }
                else{
                    toke2 = matches.at(0);
                    nextToken = Token(toke2, assignToken(toke2), lineNumber);
                    matches.erase(matches.begin());
                    toke3 = matches.at(0);
                    thirdToken = Token(toke3, assignToken(toke3), lineNumber);
                }
                
            }
            else if(matches.size() == 2){
                toke1 = matches.at(0);
                currentToken = Token(toke1, assignToken(toke1), lineNumber);
                matches.erase(matches.begin());
                toke2 = matches.at(0);
                nextToken = Token(toke2, assignToken(toke2), lineNumber);
                matches.clear();
                tokenize(getLine());
                toke3 = matches.at(0);
                thirdToken = Token(toke3, assignToken(toke3), lineNumber);
            }
            else{
                toke1 = matches.at(0);
                currentToken = Token(toke1, assignToken(toke1), lineNumber);
                matches.erase(matches.begin());
                toke2 = matches.at(0);
                nextToken = Token(toke2, assignToken(toke2), lineNumber);
                matches.erase(matches.begin());
                toke3 = matches.at(0);
                thirdToken = Token(toke3, assignToken(toke3), lineNumber);
               // matches.erase(matches.begin());
            }
        }
        else{
            currentToken = nextToken;
            nextToken = thirdToken;

            if(matches.size() > 0){
                matches.erase(matches.begin());
            }
            
            while(matches.size() < 1){
                tokenize(getLine());
            }
            while(matches.size() > 1 && matches.at(0) == "/" and matches.at(1) == "/"){
                matches.clear();
                while(matches.size() < 1){
                    tokenize(getLine());
                }
            }
            
            while(matches.size() < 1){
                tokenize(getLine());
            }
            toke3 = matches.at(0);
            thirdToken = Token(toke3, assignToken(toke3), lineNumber);
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
    if(token.type == "character" || token.type == "nonprintable_ascii"){
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

bool Tokenizer::isExpression(int x){
    Token token = retrieve(x);
    std::string lex = token.lexeme;
    std::vector<std::string> possibilitites = {"(", "true", "false", "null", "this"};
    if (std::count(possibilitites.begin(), possibilitites.end(), lex)){
        return true;
    }
    else if(isNumericLiteral() || isCharacterLiteral() || token.type == "identifier"){
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

bool Tokenizer::isNumericLiteral(){
    Token token = retrieve(0);
    Token next = retrieve(1);
    return (((token.lexeme == "+" || token.lexeme == "-") && next.type == "number") || token.type == "number");
}

std::string Tokenizer::getCurLine(int x){
    if(lastLineNum < x){
        lastLineNum = x;
        std::string line = lines.at(x);
        line.erase(0, line.find_first_not_of(" "));
        std::string line2 = std::to_string(x).append(": ").append(line);
        return line2;
    }
    else{
        return "";
    }
    
}


bool Tokenizer::isType(int x){
    Token token = retrieve(x);
    std::vector<std::string> protectedWords = { "int", "char", "bool", "void", "sym"};
    //Remove isIdentifier if you don't like it.
     if (std::find(protectedWords.begin(), protectedWords.end(), token.lexeme) != protectedWords.end() || isIdentifier(x)) {
         return true;
     }
    return false;
}

bool Tokenizer::isStatement(int x){
    Token token = retrieve(x);
    std::vector<std::string> protectedWords = { "{", "if", "while", "return", "cout", "cin", "switch", "break"};
    if (std::find(protectedWords.begin(), protectedWords.end(), token.lexeme) != protectedWords.end())
        return true;
    else if(isExpression())
        return true;
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
            if(token == "\'\\n\'"){
                return "character";
            }
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

