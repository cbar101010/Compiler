//
//  symbol_table.hpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#ifndef symbol_table_hpp
#define symbol_table_hpp

#include <stdio.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "symbol.hpp"
#include "tokenizer.hpp"

class SymbolTable{
public:
    std::string scope;
    std::unordered_map<int, Symbol> symbols;
    SymbolTable();
};

#endif /* symbol_table_hpp */
