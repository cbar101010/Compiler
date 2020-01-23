//
//  token.cpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#include "token.hpp"

Token::Token(){
        lexeme = "";
        type = "";
        lineNumber = -1;
    }

Token::Token (std::string lexeme, std::string type, int lineNumber){
        this->lexeme = lexeme;
        this->type = type;
        this->lineNumber = lineNumber;
    }
    
void Token::print(){
        std::cout << lineNumber << ": Lexme: " << lexeme << " Type: " << type << "\n";
}
