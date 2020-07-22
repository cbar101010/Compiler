//
//  symbol_table.hpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright � 2020 Colby Barton. All rights reserved.
//

#ifndef symbol_table_hpp
#define symbol_table_hpp

#include <stdio.h>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <stack>
#include "symbol.hpp"
#include "tokenizer.hpp"
#include "sar.hpp"

class SymbolTable {
public:
    int mainCounter = 0;
    int varCounter = 0;
    std::vector<std::string> classNames;
    std::vector<std::string> scopes = { "g" };
    std::vector<std::pair <std::string, Symbol>> symbols;
    std::stack<std::string> ifStack;
    std::stack<std::string> whileStack;
    std::stack<std::string> elseStack;
    std::stack<std::string> endWhileStack;
    std::string getClassSymId(std::string name);
    std::string getConstructorSymIdFromClassName(std::string name);
    SymbolTable();
    int labelCounter = 0;
    std::string genLabel(std::string type);
    std::string push(Symbol symbol);
    std::string getSymId(std::string kind);
    std::string getScope(int x = -999);
    void print();
    void pushScope(std::string newScope);
    void popScope();
    void thisDeclarer();
    Symbol getSymbol(std::string symId);
    bool typeMatch(sar symIdOne, sar symIdTwo, std::string op = "noOP");
    std::string inScope(std::string curLexeme, std::string type = "NNNNOOOOTTYPPEEE");
    std::string inScopeArray(std::string curLexeme);
    std::string inScopeWithScope(sar curSar, std::string scope);
    std::string inScopeWithScopeNieve(std::string curSar, std::string scope);
    bool isClassName(std::string name);
    bool isMemberOf(std::string first, sar second);
    Symbol getMemberOf(std::string symId, sar memeber);
    std::string getMethod(sar curSar);
    std::string getArray(sar curSar);
    std::string constructor(sar argumentListSar, sar typeSar);
    bool isFunction(Symbol thisSymbol);
    bool isArray(Symbol thisSymbol);
    int classSize = 0;
    int funcSize = -12;
    bool inClass = false;
    bool inFunction = false;
    bool inMain = false;
    bool inConstructor = false;
    void assignFuncSize(std::string symId);
    void assignClassSize(std::string symId);
    std::string getClassSize(sar classSar);
    std::string pointerSymId;
    std::string getConstructorSymID(std::string lexeme);
    std::string currentFunction();
    std::string currentScopeSymId;
    std::string charSizeSymId;
    std::string currentClass();
    std::string getConstructorSymIdFromConstructorSymId(std::string symId);

};

#endif /* symbol_table_hpp */
