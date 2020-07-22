//
//  tCode.hpp
//  lexical
//
//  Created by Colby Barton on 3/11/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#ifndef tCode_hpp
#define tCode_hpp

#include <stdio.h>
#include "symbol_table.hpp"
#include "global.h"
#include <iostream>
#include <fstream>

class tCode{
public:
    SymbolTable symbolTable;
    std::ofstream file;
    std::vector<quadStruct> quad;
    tCode(std::vector<quadStruct> quad, SymbolTable symbolTable);
    void tCodeComplete();
    std::string getLocation(std::string symId);
    std::string getValue(std::string symID, bool use_old_frame = false);
    void globalize();
    bool isGlobal(Symbol symbol);
    bool checkStack(Symbol symbol);
    bool checkHeap(Symbol symbol);
    bool checkInstruction(Symbol symbol);
    void frame();
    void overUnder();
    void four();
    void call();
    void trap();
    void func();
    void move();
    void add();
    void mul();
    void div();
    void sub();
    void write();
    void rtn();
    void push();
    void eq();
    void lt();
    void gt();
    void ne();
    void le();
    void ge();
    void bf();
    void andd();
    void orr();
    void jmp();
    void peek();
    void newi();
    void ref();
    void neww();
    void aef();
    void returnn();
    void read();
    void neededValues();
    std::string getLabel();
    void print(std::string statement, bool newLine = true);
    int useReg = 5;
    void resetReg();
    int labelNum = -1;
    quadStruct curQuad;
};


#endif /* tCode_hpp */
