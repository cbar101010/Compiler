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

Symbol::Symbol(){
    
}

std::string Symbol::getData(std::string val){
    if(this->kind == "class" && val == "type"){
        return this->lexeme;
    }
    if(this->kind == "method" && val == "type"){
        return this->data["returnType"];
    }
    return this->data[val];
    return "Data Not In Symbol";
}

bool Symbol::isFunction(){
    if(this->symID[0] == 'M')
        return true;
    return false;
}

bool Symbol::isArray(){
    if(this->data["type"][0] == '@')
        return true;
    return false;
}
