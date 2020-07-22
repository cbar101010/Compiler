//
//  symbol.hpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#ifndef symbol_hpp
#define symbol_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
class Symbol{
public:
    
    std::string symID;
    std::string scope;
    std::string lexeme;
    std::string kind;
    std::vector<std::string> param;
    std::unordered_map<std::string, std::string> data;
    int size = 0;
    int offset;
    Symbol(std::string scope, std::string lexeme, std::string kind, std::unordered_map<std::string, std::string> data);
    Symbol();
    std::string getData(std::string val);
    bool isFunction();
    bool isArray();
    
};

#endif /* symbol_hpp */
