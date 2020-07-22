//
//  symbol_table.cpp
//  lexical
//
//  Created by Colby Barton on 1/20/20.
//  Copyright � 2020 Colby Barton. All rights reserved.
//

#include "symbol_table.hpp"

//SymbolTable::SymbolTable(std::string fileName){
//    this->tokenizer = Tokenizer(fileName);
//}


SymbolTable::SymbolTable() {
    Symbol tr = Symbol();
    tr.scope = "g.";
    tr.symID = "true";
    tr.lexeme = "true";
    tr.kind = "true";
    tr.data["type"] = "bool";
    tr.data["accessMod"] = "public";
    
    Symbol False = Symbol();
    False.scope = "g.";
    False.symID = "false";
    False.lexeme = "false";
    False.kind = "false";
    False.data["type"] = "bool";
    False.data["accessMod"] = "public";
    
    Symbol null = Symbol();
    null.scope = "g.";
    null.symID = "null";
    null.lexeme = "null";
    null.kind = "null";
    null.data["type"] = "null";
    null.data["accessMod"] = "public";
    
    Symbol pointerSize = Symbol();
    pointerSize.scope = "g.";
    pointerSize.lexeme = "4";
    pointerSize.kind = "ilit";
    pointerSize.data["type"] = "int";
    pointerSize.data["accessMod"] = "public";
    pointerSymId = getSymId(pointerSize.kind);
    pointerSize.symID = pointerSymId;
    
    Symbol charSize = Symbol();
    charSize.scope = "g.";
    charSize.lexeme = "1";
    charSize.kind = "ilit";
    charSize.data["type"] = "int";
    charSize.data["accessMod"] = "public";
    charSizeSymId = getSymId(charSize.kind);
    charSize.symID = charSizeSymId;

    
    
    this->symbols.push_back(std::make_pair(False.symID, False));
    this->symbols.push_back(std::make_pair(null.symID, null));
    this->symbols.push_back(std::make_pair(tr.symID, tr));
    this->symbols.push_back(std::make_pair(pointerSymId, pointerSize));
    this->symbols.push_back(std::make_pair(charSizeSymId, charSize));

}

std::string SymbolTable::currentClass(){
    std::string scope;
    for(int i = 0; i < scopes.size() - 1; i++){
        scope.append(scopes.at(i)).append(".");
    }
    for(auto x: symbols){
        if(x.second.scope == scope && scopes.back() == x.second.lexeme){
            return x.first;
        }
    }
    std::cout << "WHAT THE CHECK CURRENTClass FUNCTION IN SYMBOL TABLE";
    exit(EXIT_FAILURE);
    return "WHAT THE CHECK CURRENTFUNCTION FUNCTION IN SYMBOL TABLE";
}

std::string SymbolTable::currentFunction(){
    std::string scope;
    for(int i = 0; i < scopes.size() - 1; i++){
        scope.append(scopes.at(i)).append(".");
    }
    for(auto x: symbols){
        if(x.second.scope == scope && scopes.back() == x.second.lexeme){
            return x.first;
        }
    }
    std::cout << "WHAT THE CHECK CURRENTClass FUNCTION IN SYMBOL TABLE";
    exit(EXIT_FAILURE);
    return "WHAT THE H CHECK CURRENTFUNCTION FUNCTION IN SYMBOL TABLe";
}

std::string SymbolTable::push(Symbol symbol) {
    std::string symId = getSymId(symbol.kind);
    if(symId == "R283"){
        int x = 1;
    }
    symbol.symID = symId;
    bool isTemp = false;
    if(symId[0] == 'T' || symId[0] == 'R'){
        isTemp = true;
    }
    if(symbol.kind != "clit" && symbol.kind != "ilit")
        symbol.scope = getScope();
    else
        symbol.scope = "g.";
    if(inClass && !inFunction && symbol.symID[0] != 'M' && symbol.symID != "main" && symbol.symID[0] != 'X' && symbol.symID[0] != 'C' && symbol.symID[0] != 'H' && symbol.symID[0] != 'N' && symbol.kind != "this" && symbol.kind != "clit" && symbol.kind != "ilit"){
        if(isTemp){
            symbol.offset = getSymbol(currentClass()).size + funcSize;
        }
        else{
            symbol.offset = classSize;
        }
        classSize += 4;
    }
    else if(((inFunction || inMain) && symbol.symID[0] != 'M' && symbol.symID[0] != 'X' && symbol.kind != "this" && symbol.symID[0] != 'H' && symbol.symID[0] != 'N') && symbol.symID != "main"){
        if(isTemp){
            symbol.offset = getSymbol(currentFunction()).size + funcSize + 12;
        }
        else{
            symbol.offset = funcSize;
        }
        funcSize -= 4;
    }
    if((symbol.kind == "temp" || symbol.kind == "rtemp") && inClass){
        symbol.data["inClass"] = "true";
    }
    symbols.push_back(std::make_pair(symId, symbol));
    return symId;
}

void SymbolTable::assignFuncSize(std::string symId){
    for(auto & x : symbols){
        if(x.first == symId){
            if(x.second.size != 0){
                x.second.size += funcSize + 12;
            }
            else{
                x.second.size = funcSize;
            }
            funcSize = -12;
            break;
        }
    }
}

std::string SymbolTable::getConstructorSymIdFromConstructorSymId(std::string symId){
    return getClassSymId(getSymbol(symId).lexeme);
}

std::string SymbolTable::getConstructorSymIdFromClassName(std::string name){
    for(auto & x : symbols){
        if(x.second.lexeme == name && x.first[0] == 'X'){
            return x.first;
        }
    }
    return "";
}


std::string SymbolTable::getClassSymId(std::string name){
    for(auto & x : symbols){
        if(x.second.lexeme == name && x.first[0] == 'C'){
            return x.first;
        }
    }
    return "";
}

void SymbolTable::assignClassSize(std::string symId){
    for(auto & x : symbols){
        if(x.first == symId){
            x.second.size += classSize;
            classSize = 0;
            break;
        }
    }
}

void SymbolTable::thisDeclarer(){
    Symbol constructorSym = Symbol();
    constructorSym.lexeme = "this";
    constructorSym.kind = "this";
    std::string className;

    for(int i = 0; i < this->scopes.size() -1; i++){
        if (i == this->scopes.size() - 2){
            className = scopes.at(i);
        }
    }
    constructorSym.data["type"] = className;
    constructorSym.scope = getScope(-1);
    std::string tempString = "this";
    varCounter++;
    if(className != "g")
        this->push(constructorSym);
}



std::string SymbolTable::getSymId(std::string kind) {

    std::string prefix;
    std::string temp;
    if (kind == "lvar") {
        prefix = "L";
        temp = std::to_string(varCounter);
    }
    else if (kind == "ivar") {
        prefix = "V";
        temp = std::to_string(varCounter);
    }
    else if(kind == "this"){
        prefix = "this";
        temp = std::to_string(varCounter);
    }
    else if (kind == "method") {
        prefix = "M";
        temp = std::to_string(varCounter);
    }
    else if (kind == "param") {
        prefix = "P";
        temp = std::to_string(varCounter);
    }
    else if (kind == "constructor") {
        prefix = "X";
        temp = std::to_string(varCounter);
    }
    else if (kind == "main") {
        if (mainCounter > 0)
            return "ERROR 2 MAINS";
        mainCounter++;
        return "main";
    }
    else if (kind == "class") {
        prefix = "C";
        temp = std::to_string(varCounter);
    }
    else if (kind == "ilit") {
        prefix = "N";
        temp = std::to_string(varCounter);
    }
    else if (kind == "clit"){
        prefix = "H";
        temp = std::to_string(varCounter);
    }
    else if (kind == "temp"){
        prefix = "T";
        temp = std::to_string(varCounter);
    }
    else if (kind == "rtemp"){
        prefix = "R";
        temp = std::to_string(varCounter);
    }
    else {
        return "error";
    }
    varCounter++;
    return prefix.append(temp);
}

bool SymbolTable::isClassName(std::string name) {
    if (std::count(classNames.begin(), classNames.end(), name))
        return true;
    return false;
}

Symbol SymbolTable::getMemberOf(std::string symId, sar member){
        Symbol classSym = getSymbol(symId);
        std::string className = classSym.getData("type");
        std::string scope = "";
        bool isThis = classSym.kind == "this";
        if(isThis){
            for(int c = 0; c < scopes.size() - 1; c++){
                scope.append(scopes.at(c)).append(".");
            }

        }
        else{
            Symbol cl = getSymbol(inScope(className, "class"));
            scope = cl.scope;
            scope.append(className).append(".");
        }
        std::string memberSymbol = inScopeWithScope(member, scope);
        Symbol mem = getSymbol(memberSymbol);
        return mem;
}



bool SymbolTable::isMemberOf(std::string symId, sar member){
    Symbol classSym = getSymbol(symId);
    std::string className = classSym.getData("type");
    bool isThis = classSym.kind == "this";
    if(!isClassName(className) && !isThis){
        return false;
    }
    std::string scope = "";
    if(isThis){
        for(int c = 0; c < scopes.size() - 1; c++){
            scope.append(scopes.at(c)).append(".");
        }
    }
    else{
        Symbol cl = getSymbol(inScope(className, "class"));
        scope = cl.scope;
        scope.append(className).append(".");
    }
    std::string memberSymbol = inScopeWithScope(member, scope);
    Symbol mem = getSymbol(memberSymbol);
    if(mem.getData("accessMod") != "public" && !isThis){
        return false;
    }
    if(memberSymbol != "NF"){
        return true;
    }
    return false;
}

void SymbolTable::print() {
    std::for_each(symbols.begin(), symbols.end(), [](std::pair<std::string, Symbol> element) {
        std::cout << element.first << " --> ";
        std::cout << "   Scope: " << element.second.scope << std::endl;
        std::cout << "          SymbId: " << element.second.symID << std::endl;
        std::cout << "          Lexeme: " << element.second.lexeme << std::endl;
        std::cout << "          Kind: " << element.second.kind << std::endl;
        std::cout << "          Size: " << element.second.size << std::endl;
        std::cout << "          Offset: " << element.second.offset << std::endl;
        if (!element.second.param.empty()) {
            std::cout << "          Param: ";
            for (std::vector<std::string>::const_iterator i = element.second.param.begin(); i != element.second.param.end(); ++i)
                std::cout << *i << ", ";
            std::cout << std::endl;
        }
        std::cout << "          Data: " << std::endl;
        std::for_each(element.second.data.begin(), element.second.data.end(), [](std::pair<std::string, std::string> dataElement) {
            std::cout << "              " << dataElement.first << ": " << dataElement.second << std::endl;
        });
        std::cout << std::endl << std::endl << std::endl;
    });
}

std::string SymbolTable::getScope(int size) {
    std::string temp;
    //fullScope
    if(size == -999){
        for (std::string curScope : scopes) {
            temp.append(curScope).append(".");
        }
    }
    else if(size < 0){
        for(int i = 0; i < scopes.size() + size; i++){
            temp.append(scopes.at(i)).append(".");
        }
    }
    else{
        for(int i = 0; i < size; i++){
            temp.append(scopes.at(i)).append(".");
        }
    }
    return temp;
}

void SymbolTable::pushScope(std::string newScope) {
    if (scopes.empty()) {
        std::cout << "ERROR ERROR ERRROR: SCOPE IS EMPTY";
    }
    else {
        scopes.push_back(newScope);
    }
}

void SymbolTable::popScope() {
    scopes.pop_back();
}

Symbol SymbolTable::getSymbol(std::string symId){
    for(int i = 0; i < symbols.size(); ++i){
        if(symbols.at(i).first == symId){
            return symbols.at(i).second;
        }
    }
    if(symId == "this"){
        Symbol l = Symbol();
        l.lexeme = "this";
        return l;
    }
    return Symbol();
}

std::string SymbolTable::constructor(sar argumentListSar, sar typeSar){
    //WARNING THIS WILL PROBABLY CAUSE PROBLEMS;
    for(int i = 0; i < symbols.size(); ++i){
        if(symbols.at(i).second.kind == "constructor" && symbols.at(i).second.lexeme == typeSar.token.lexeme){
            if(argumentListSar.argumentList.size() == symbols.at(i).second.param.size()){
                if(argumentListSar.argumentList.size() == 0){
                    return symbols.at(i).second.symID;
                }
                for(int z = 0; z < argumentListSar.argumentList.size(); z++){
                    Symbol temp1 = getSymbol(argumentListSar.argumentList.at(z).symID);
                    Symbol temp2 = getSymbol(symbols.at(i).second.param.at(z));
                    if(temp1.getData("type") == temp2.getData("type")){
                        if(z == (argumentListSar.argumentList.size() - 1)){
                            return symbols.at(i).second.symID;
                        }
                    }
                    else{
                        break;
                    }
                }
            }
            
        }
    }
    return "NF";
}

std::string SymbolTable::getClassSize(sar classSar){
    for(int i = 0; i < symbols.size(); i++){
        if(symbols.at(i).second.kind == "class" && classSar.token.lexeme == symbols.at(i).second.lexeme){
            return symbols.at(i).first;
        }
    }
    return "NF";
}

bool SymbolTable::typeMatch(sar symIdOne, sar symIdTwo, std::string op){
    Symbol one = getSymbol(symIdOne.symID);
    Symbol two = getSymbol(symIdTwo.symID);
    std::string twoType;
    std::string oneType;
    oneType = one.getData("type");
    twoType = two.getData("type");
    if(symIdOne.type == "arr"){
        oneType.erase(0,2);
    }
    if(symIdTwo.type == "arr"){
        twoType.erase(0,2);
    }
    if(oneType == twoType && oneType == "Data Not In Symbol"){
        if(oneType == "Data Not In Symbol"){
            std::cout << "bro Data is not in the symbol";
            exit(EXIT_FAILURE);
        }
    }
    else{
        if(oneType != twoType){
            if((this->isClassName(oneType) && two.kind == "null") || (oneType[0] == '@' && two.kind == "null")){
                return true;
            }
            if(std::count(classNames.begin(), classNames.end(), twoType) && oneType == "null"){
                return true;
            }
            return false;
        }
        if((op == "+" || op == "-" || op == "/" || op == "*") && (oneType != "int" || twoType != "int")){
            return false;
        }
        else if((op == ">" || op == "<" || op == "<=" || op == ">=") && ((oneType != "int" || twoType != "int") && (oneType != "char" || twoType != "char"))){
            return false;
        }
        else if(op == "==" || op == "!="){
            if(oneType == twoType){
                return true;
            }
            return false;
        }
        else if((op == "and" || op == "or") && (oneType != "bool" || twoType != "bool"));
        else{
            return true;
        }
    }
    return false;
}
std::string SymbolTable::getConstructorSymID(std::string lexeme){
    for(auto x: symbols){
        if(x.second.lexeme == lexeme && x.second.symID[0] == 'X'){
            return x.first;
        }
    }
    return "NF";
}
std::string SymbolTable::inScope(std::string curLexeme, std::string type){
    std::string curScope = getScope();
    unsigned long scopeLength = scopes.size();
    while(curScope != ""){
        for(int i = 0; i < symbols.size(); ++i){
            if(symbols.at(i).second.scope == curScope){
                if(symbols.at(i).second.lexeme == curLexeme){
                    if(type ==  "NNNNOOOOTTYPPEEE" || type == symbols.at(i).second.kind){
                        return symbols.at(i).first;
                    }
                }
            }
        }
        scopeLength--;
        curScope = getScope(scopeLength);
    }
    return "NF";
}
std::string SymbolTable::genLabel(std::string type){
    std::string newLabel = "LABEL";
    newLabel.append(std::to_string(labelCounter));
    labelCounter++;
    if(type == "if"){
        ifStack.push(newLabel);
    }
    else if(type == "while"){
        whileStack.push(newLabel);
    }
    else if(type == "else"){
        elseStack.push(newLabel);
    }
    else if(type == "endWhile"){
        endWhileStack.push(newLabel);
    }
    return newLabel;
}

std::string SymbolTable::inScopeArray(std::string curLexeme){
    std::string curScope = getScope();
    unsigned long scopeLength = scopes.size();
    while(curScope != ""){
        for(int i = 0; i < symbols.size(); ++i){
            if(symbols.at(i).second.scope == curScope && symbols.at(i).second.lexeme == curLexeme && symbols.at(i).second.data["type"][0] == '@'){
                return symbols.at(i).first;
            }
        }
        scopeLength--;
        curScope = getScope(scopeLength);
    }
    return "NF";
}

//DON"T USE THIS (never made it do anything good just took getMothod and removed argumentList parts
std::string SymbolTable::getArray(sar curSar){
    std::string scope = getScope();
    unsigned long scopeLength = scopes.size();
    while(scope != ""){
        for(int i = 0; i < symbols.size(); ++i){
            if(symbols.at(i).second.scope == scope && symbols.at(i).second.lexeme == curSar.token.lexeme){
                        return symbols.at(i).first;
                }
            }
        }
        scopeLength--;
        scope = getScope(scopeLength);
    return "NF";
}

std::string SymbolTable::getMethod(sar curSar){
    //Probably shouldn't do this here either.
    std::string scope = getScope();
    unsigned long scopeLength = scopes.size();
    while(scope != ""){
        for(int i = 0; i < symbols.size(); ++i){
            if(symbols.at(i).second.scope == scope && symbols.at(i).second.lexeme == curSar.token.lexeme && symbols.at(i).first[0] == 'M'){
                if(curSar.argumentList.size() == symbols.at(i).second.param.size()){
                    if(curSar.argumentList.size() == 0){
                        return symbols.at(i).first;
                    }
                    for(int z = 0; z < curSar.argumentList.size(); z++){
                        Symbol temp1 = getSymbol(curSar.argumentList.at(z).symID);
                        Symbol temp2 = getSymbol(symbols.at(i).second.param.at(z));
                        std::string temp1Type = temp1.getData("type");
                        std::string temp2Type = temp2.getData("type");
                        if(curSar.argumentList.at(z).type == "arr"){
                            temp1Type.erase(0,2);
                        }
                        if(temp1Type == temp2Type){
                            if(z == (curSar.argumentList.size() - 1)){
                                return symbols.at(i).first;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
        }
        scopeLength--;
        scope = getScope(scopeLength);
    }
    return "NF";
}

std::string SymbolTable::inScopeWithScope(sar curSar, std::string scope){
    for(int i = 0; i < symbols.size(); ++i){
        if(symbols.at(i).second.scope == scope && symbols.at(i).second.lexeme == curSar.token.lexeme){
            if(curSar.type == "func"){
                if(curSar.argumentList.size() == symbols.at(i).second.param.size()){
                    if(curSar.argumentList.size() == 0){
                        return symbols.at(i).first;
                    }
                    for(int z = 0; z < curSar.argumentList.size(); z++){
                            Symbol temp1 = getSymbol(curSar.argumentList.at(z).symID);
                            Symbol temp2 = getSymbol(symbols.at(i).second.param.at(z));
                        if(temp1.getData("type") == temp2.getData("type")){
                            if(z == (curSar.argumentList.size() - 1)){
                                return symbols.at(i).first;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            else{
                    return symbols.at(i).first;
                }
            }
        }
    return "NF";
}

std::string SymbolTable::inScopeWithScopeNieve(std::string curLexeme, std::string scope){
    for(int i = 0; i < symbols.size(); ++i){
        if(symbols.at(i).second.scope == scope){
            if(symbols.at(i).second.lexeme == curLexeme){
                return symbols.at(i).first;
            }
        }
    }
    return "NF";
}
