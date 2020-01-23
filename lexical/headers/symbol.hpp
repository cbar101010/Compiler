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
class Symbol{
public:
    
    int symID;

    std::string scope;
    std::string lexeme;
    std::string kind;
    
    std::unordered_map<std::string, std::string> data;
    
    Symbol(std::string scope, std::string lexeme, std::string kind, std::unordered_map<std::string, std::string> data);
};

#endif /* symbol_hpp */
