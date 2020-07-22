//
//  sar.cpp
//  lexical
//
//  Created by Colby Barton on 2/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#include "sar.hpp"
bool sar::Empty(){
    if(this->token.lexeme == ""){
        return true;
    }
    else return false;
}
sar::~sar(){
    
}

sar::sar(){
    
}
idSar::idSar(){
    type = "identifier";
}
lSar::lSar(){
    type = "lLit";
}

refSar::refSar(){
    type = "ref";
}
vSar::vSar(){
    type = "vSar";
}

balSar::balSar(){
    type = "bal";
}
alSar::alSar(){
    type = "al";
}

tSar::tSar(){
    type = "temp";
}
typeSar::typeSar(){
    type = "type";
}
funcSar::funcSar(){
    type = "func";
}
arrSar::arrSar(){
    type = "arr";
}
newSar::newSar(){
    type = "new";
}
