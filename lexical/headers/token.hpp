//
//  token.hpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#ifndef token_hpp
#define token_hpp

#include <stdio.h>
#include <string>
#include <iostream>

class Token{
public:
    std::string lexeme;
    std::string type;
    int lineNumber;
    Token();
    Token (std::string lexeme, std::string type, int lineNumber);
    void print();
};


#endif /* token_hpp */
