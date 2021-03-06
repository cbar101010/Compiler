//
//  tokenizer.hpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#ifndef tokenizer_hpp
#define tokenizer_hpp
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <iterator>
#include <vector>
#include <algorithm>
#include <map>
#include <ctype.h>
#include <stdio.h>
#include "token.hpp"

class Tokenizer{
public:
    std::vector<std::string> matches;
    int matchCount;
    Token currentToken;
    Token nextToken;
    Token thirdToken;
    std::map<int, std::string> lines;
    std::ifstream myfile;
    std::string fileName;
    int lineNumber;
    int lastLineNum;
    Tokenizer(std::string fileName);
    Tokenizer();
    std::string getLine();
    
    void tokenize(std::string line);
    Token retrieve(int x);
    Token getToken();
    std::string lexeme();
    std::string type();
    int lineNum();
    Token peekToken();
    Token peek2Token();
    std::string getCurLine(int x);
    void next();
    
    static std::string assignToken(std::string token);
    bool firstLine = true;
    bool isCharacterLiteral(int x = 0);
    bool isExpressionZ(int x = 0);
    bool isSemiColon(int x = 0);
    bool isIdentifier(int x = 0);
    bool isNumericLiteral();
    bool isExpression(int x = 0);
    bool isType(int x = 0);
    bool isStatement(int x = 0);
    void setCurLexeme(std::string newLex);

};




#endif /* tokenizer_hpp */
