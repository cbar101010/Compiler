//
//  symbol.cpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#include "symbol.hpp"

Symbol::Symbol(std::string scope, std::string lexeme, std::string kind, std::unordered_map<std::string, std::string> data){
    
    this->scope = scope;
    this->lexeme = lexeme;
    this->kind = kind;
    this->data = data;
    
}
