#include "token.hpp"
#include "tokenizer.hpp"
#include "symbol_table.hpp"
#include "symbol.hpp"
#include "sar.hpp"
#include "global.h"
#include "tcode.hpp"

Tokenizer tokenizer;
SymbolTable symbolTable = SymbolTable();
Symbol symbol;
Symbol paramSymbol;
sar curSar;

//Symantec stuff
std::string curLabel;
std::string backTrack(std::string newLabel);
void squadToQuad();
bool sp = false;
void eoe();
void printQuad();
void iExist();
void iExist2();
void iExistError(sar thisSar);
void iPush();
void oPush(std::string op);
void leftValError(sar thisSar);
bool allowPush(std::string op);
void lPush();
void typepush();
void tExist();
void rExist();
void vPush(Token t = Token());
void dup(std::string lex, std::string type);
void bal();
void poundComma();
void eal();
void closingParen();
void func();
void iff();
void arr();
void whilee();
void returnn();
void coutt();
void cin();
void newObj();
void cd();
void newArray();
void createPushQuads(std::vector<sar> argList);
//Syntax stuff (mostly).
void analyze();
void statement();
void expression();
void expressionz();
void syntaxErrorLexeme(std::string expected);
void syntaxErrorType(std::string expected);
void assignmentExpression();
void numbericLiteral();
void characterLiteral();
void number(bool isNeg);
void caseBlock();
void caseLabel();
void literal();
void character();
void nonPrintableASCII();
void printableASCII();
void fnArrMember();
void argumentList();
void memberRefz();
void newDeclaration();
void classMemberDeclaration();
void constructorDeclaration();
void fieldDeclaration();
void methodBody();
void parameter();
void variableDeclaration();
void parameterList();
void compiliationUnit();
void classDeclaration();
void endBrakcet();
void type();
void identifier();
void runRutine();
void printStack(int x);
int getOpVal();
bool isValidL();
bool isMath(std::string op);
bool isBool(std::string op);
std::string getLabel(std::string type = "");
std::string classSymID2;

bool misMdifier();
//Read definition for why I got rid of this
//bool isType(int x = 0);
bool isClassName(int x = 0);

std::string localVariable = "lvar";
std::vector<std::string> numberVector = {"4"};
std::vector<std::string> characterVector;
std::stack<std::string> operatorStack;
std::stack<sar> SAS;
std::map<std::string, int> presMap = {{"(", 15}, {")", 0}, {"[", 15}, {"]", 0}, {"*", 13}, {"/", 13}, {"%", 13}, {"+", 11}, {"-", 11}, {"<", 9}, {">", 9}, {"<=", 9}, {">=", 9}, {"==", 7}, {"!=", 7}, {"and", 5}, {"or", 3}, {"=", 1}};



void quadPush(quadStruct newQuad);

std::vector<quadStruct> squad;
std::vector<quadStruct> quad = {};
void printToken(std::string token, int lineNumber, std::string type) {
    std::cout << lineNumber << ": token = \"" << token << "\"" << " type = " << type << std::endl;
}

void printQuad(){
    for(quadStruct i : quad){
        i.printStruct();
    }
}

int getOpVal(std::string op){
    std::map<std::string, int>::iterator p;
    p = presMap.find(op);
    if(p != presMap.end())
        return p->second;
    else
        std::cout << "I broke in OpVal, operator is not valid" << std::endl;
    exit(EXIT_FAILURE);
}

void squadToQuad(){
    for(quadStruct x: squad){
        quad.push_back(x);
    }
    squad.clear();
}

void newArray(){
    sar temp = SAS.top();
    SAS.pop();
    Symbol expression = symbolTable.getSymbol(temp.symID);
    if(expression.data["type"] != "int"){
        std::cout << temp.token.lineNumber << ": " << "Array requres int index got " <<  expression.data["type"] << std::endl;
        exit(EXIT_FAILURE);
    }
    Symbol tempSym = Symbol();
    sar typeSar = SAS.top();
    SAS.pop();
    if(!(symbolTable.isClassName(typeSar.token.lexeme) || typeSar.token.lexeme == "int" || typeSar.token.lexeme == "char" || typeSar.token.lexeme == "bool" || typeSar.token.lexeme == "void")){
        std::cout << temp.token.lineNumber << ": Type " << typeSar.token.lexeme << " not defined" << std::endl;
        exit(EXIT_FAILURE);
    }
    typeSar.type = "newArr";
    tempSym.kind = "temp";
    std::string tempString = "@:";
    tempSym.data["type"] = tempString.append(typeSar.token.lexeme);
    typeSar.symID = symbolTable.push(tempSym);
    SAS.push(typeSar);
    
    quadStruct mulQuad;
    Symbol mulTempSym;
    mulTempSym.kind = "temp";
    mulTempSym.data["accessMod"] = "public";
    mulTempSym.data["type"] = "int";
    std::string mulTempSymId = symbolTable.push(mulTempSym);
    mulQuad.lineStatement = tokenizer.getCurLine(temp.token.lineNumber);
    if(typeSar.token.lexeme != "char"){
        mulQuad.arg1 = symbolTable.pointerSymId;
    }
    else{
        mulQuad.arg1 = symbolTable.charSizeSymId;

    }
    mulQuad.arg2 = expression.symID;
    mulQuad.op = "MUL";
    mulQuad.arg3 = mulTempSymId;
    quadPush(mulQuad);
    
    quadStruct newQuad;
    newQuad.op = "NEW";
    newQuad.arg1 = mulTempSymId;
    newQuad.arg2 = typeSar.symID;
    quadPush(newQuad);
}
void quadPush(quadStruct newQuad){

    if(curLabel != ""){
        newQuad.label = getLabel();
    }
    if(symbolTable.inFunction || symbolTable.inMain){
        quad.push_back(newQuad);
    }
    else if(symbolTable.inClass){
        squad.push_back(newQuad);
    }
}

void cd(){
    std::string name = tokenizer.lexeme();
    std::string className = symbolTable.scopes.at(symbolTable.scopes.size() -1);
    if(className != name){
        if(tokenizer.peekToken().lexeme != "("){
            std::cout << tokenizer.lineNum() << ": Found " << name << " expecting modifier " << std::endl;
            exit(EXIT_FAILURE);
        }
        else{
            std::cout << tokenizer.lineNum() << ": Counstructor " << name << " must match class" << className << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void newObj(){
    quadStruct newQuad;
    sar allSar = SAS.top();
    allSar.type = "al";
    SAS.pop();
    sar typeSar = SAS.top();
    typeSar.type = "type";
    SAS.pop();
    if(symbolTable.constructor(allSar, typeSar) == "NF"){
        std::cout << tokenizer.lineNum() << ": " << "Constructor " << typeSar.token.lexeme << "(";
        for(int i = 0; i < allSar.argumentList.size(); i++){
            std::cout << symbolTable.getSymbol(allSar.argumentList.at(i).symID).data["type"] << ", ";
        }
        std::cout << ") not defined" << std::endl;
        exit(EXIT_FAILURE);
    }
    //I did this because of the Icode Cat k = new Cat(); (see icode documentation).
    Symbol newEmptySym;
    newEmptySym.kind = "temp";
    newEmptySym.symID = symbolTable.push(newEmptySym);

    
    
    Symbol tempSym = Symbol();
    newSar newS =  newSar();
    tempSym.kind = "temp";
    tempSym.data["type"] = typeSar.token.lexeme;
    newS.symID = symbolTable.push(tempSym);
    SAS.push(newS);
    
    std::string myClass = symbolTable.getClassSize(typeSar);
    Symbol classSymbol = symbolTable.getSymbol(myClass);
    
    newQuad.op = "NEWI";
    newQuad.arg1 = std::to_string(classSymbol.size);
    newQuad.arg2 = newEmptySym.symID;
    newQuad.lineStatement = tokenizer.getCurLine(typeSar.token.lineNumber);
    quadPush(newQuad);
    
    quadStruct frameQuad;
    frameQuad.op = "FRAME";
    frameQuad.arg1 = symbolTable.getConstructorSymID(classSymbol.lexeme);
    frameQuad.arg2 = newEmptySym.symID;
    quadPush(frameQuad);
    
    createPushQuads(allSar.argumentList);
    
    quadStruct callClass;
    callClass.op = "CALL";
    callClass.arg1 = symbolTable.getConstructorSymID(classSymbol.lexeme);
    quadPush(callClass);
  
 //TODO: ask what this is all about;
    quadStruct peekQuad;
    peekQuad.op = "PEEK";
    peekQuad.arg1 = newS.symID;
    quadPush(peekQuad);
}

void coutt(){
    if(!operatorStack.empty()){
        while(!operatorStack.empty()){
            runRutine();
        }
    }
    sar top = SAS.top();
    SAS.pop();
    Symbol topSym = symbolTable.getSymbol(top.symID);
    if(topSym.data["type"] != "int" && topSym.data["type"] != "char"){
        std::cout << top.token.lineNumber << ": " << "cout not defined for " <<  topSym.data["type"] << std::endl;
        exit(EXIT_FAILURE);
    }
}

void cin(){
    if(!operatorStack.empty()){
        while(!operatorStack.empty()){
            runRutine();
        }
    }
    sar top = SAS.top();
    SAS.pop();
    Symbol topSym = symbolTable.getSymbol(top.symID);
    if(topSym.data["type"] != "int" && topSym.data["type"] != "char"){
        std::cout << top.token.lineNumber << ": " << "cout not defined for " <<  topSym.data["type"] << std::endl;
        exit(EXIT_FAILURE);
    }
}

void getFuncTypeForSAS(){
    while (!SAS.empty()) {
        if(SAS.top().symID[0] == 'M'){
            break;
        }
        SAS.pop();
    }
}

void returnn(){
    quadStruct newQuad;
    if(!operatorStack.empty()){
        while(!operatorStack.empty()){
            runRutine();
        }
    }
    sar top = SAS.top();
    std::string lineNumber;
    if(top.token.lineNumber < 1){
        lineNumber = std::to_string(tokenizer.lineNum() -1);
    }
    else{
        lineNumber = std::to_string(top.token.lineNumber);
    }
    Symbol topSym = symbolTable.getSymbol(top.symID);
    if(top.symID[0] != 'M'){
        newQuad.op = "RETURN";
        newQuad.arg1 = top.symID;
        SAS.pop();
        getFuncTypeForSAS();

        sar bottom = SAS.top();
        Symbol bottomSym = symbolTable.getSymbol(bottom.symID);
        
        if(bottomSym.getData("returnType") != topSym.getData("type")){
            if(topSym.getData("type") != "null" || !symbolTable.isClassName(bottomSym.getData("returnType"))){
                std::cout << lineNumber << ": " << " Function requires " << bottomSym.data["returnType"]  << " Returned " << topSym.data["type"] << std::endl ;
                exit(EXIT_FAILURE);
            }
        }
    }
    else if(topSym.getData("returnType") != "void"){
        std::cout << lineNumber << ": " << " Function requires " << topSym.data["returnType"]  << " Returned void" << std::endl ;
        exit(EXIT_FAILURE);
    }
    else{
        newQuad.op = "RTN";
    }
    quadPush(newQuad);
}

void iff(){
    quadStruct newQuad;
    sar ifSar = SAS.top();
    Symbol temp = symbolTable.getSymbol(ifSar.symID);
    std::string type;
    type = temp.data["type"];
    if(type == ""){
        type = "assignment expression";
    }
    if(type != "bool"){
        std::cout << tokenizer.lineNum() << ": " << "If requires bool got " << type << std::endl;
        exit(EXIT_FAILURE);
    }
    SAS.pop();
    newQuad.op = "BF";
    newQuad.arg1 = temp.symID;
    newQuad.arg2 = symbolTable.genLabel("if");
    newQuad.lineStatement = tokenizer.getCurLine(ifSar.token.lineNumber);
    quadPush(newQuad);
}

void whilee(){
    quadStruct newQuad;
    sar ifSar = SAS.top();
    Symbol temp = symbolTable.getSymbol(ifSar.symID);
    std::string type;
    type = temp.data["type"];
    std::string lineNumber = std::to_string(ifSar.token.lineNumber);
    if(ifSar.token.lineNumber < 1){
        lineNumber = std::to_string(tokenizer.lineNum());
    }
    else{
        lineNumber = std::to_string(ifSar.token.lineNumber);
    }
    if(type != "bool"){
        std::cout << ifSar.token.lineNumber << ": " << "While requires bool got " << type << std::endl;
        exit(EXIT_FAILURE);
    }
    
    newQuad.op = "BF";
    newQuad.arg1 = temp.symID;
    newQuad.arg2 = symbolTable.genLabel("endWhile");
    newQuad.lineStatement = tokenizer.getCurLine(ifSar.token.lineNumber);
    quadPush(newQuad);
    
    SAS.pop();}

void closingParen(){
    while(operatorStack.top() != "("){
        runRutine();
    }
    operatorStack.pop();
}
void endBracket(){
    while(operatorStack.top() != "["){
        runRutine();
    }
    operatorStack.pop();
}

void iPush(){
    curSar = idSar();
    curSar.token = tokenizer.getToken();
    SAS.push(curSar);
}

void arr(){
    sar temp = SAS.top();
    SAS.pop();
    Symbol expression = symbolTable.getSymbol(temp.symID);
    if(expression.data["type"] != "int"){
        std::cout << temp.token.lineNumber << ": " << "Found " << expression.data["type"] << " expecting int" << std::endl;
        exit(EXIT_FAILURE);
    }
    sar idSar = SAS.top();
    SAS.pop();
    arrSar ar = arrSar();
    ar.token = idSar.token;
    ar.indexSymId = temp.symID;
    ar.arrayIndex = expression.symID;
    SAS.push(ar);
}

void func(){
    quadStruct newQuad;
    sar argumentSar = SAS.top();
    std::vector<sar> symIdVec = argumentSar.argumentList;
    SAS.pop();
    sar identifierSar = SAS.top();
    std::string methodName = identifierSar.token.lexeme;
    SAS.pop();
    funcSar funcsar = funcSar();
    funcsar.token.lexeme = methodName;
    funcsar.argumentList = symIdVec;
    funcsar.methodName = methodName;
    SAS.push(funcsar);
}

// Maybe works
void poundComma(){
    while(operatorStack.top() != "("){
        runRutine();
    }
}

void eal(){
    alSar newSar = alSar();
    sar top = SAS.top();
    while(top.type != "bal"){
        newSar.argumentList.push_back(top);
        SAS.pop();
        top = SAS.top();
    }
    SAS.pop();
    std::reverse(newSar.argumentList.begin(), newSar.argumentList.end());
    SAS.push(newSar);
}

void bal(){
    balSar b = balSar();
    SAS.push(b);
}

void vPush(Token t){
    std::string symID = symbolTable.inScope(t.lexeme);
    curSar = vSar();
    curSar.symID = symID;
    curSar.token = t;
    SAS.push(curSar);
}

void typePush(){
    curSar = typeSar();
    curSar.token = tokenizer.getToken();
    SAS.push(curSar);
}

//Fix ERROR Messages
void dup(std::string lex, std::string type){
    std::string symID = symbolTable.inScopeWithScopeNieve(lex, symbolTable.getScope());
    if(symID != "NF"){
        Symbol sym = symbolTable.getSymbol(symID);
        if(sym.kind == "method" || sym.kind == "constructor"){
            std::cout << tokenizer.lineNum() << ": Duplicate function " << lex << std::endl;
        }
        else if(sym.kind == "class"){
            std::cout << tokenizer.lineNum() << ": Duplicate class " << lex << std::endl;
        }
        else{
            std::cout << tokenizer.lineNum() << ": Duplicate variable " << lex << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    
}

void lPush(){
    curSar = lSar();
    curSar.token = tokenizer.getToken();
    SAS.push(curSar);
    iExist();
}

void tExist(){
    sar temp = SAS.top();
    SAS.pop();
    std::string tok = temp.token.lexeme;
    if(!(symbolTable.isClassName(tok) || tok == "int" || tok == "bool" || tok == "char" || tok == "void" || tok == "sym")){
        std::cout << tokenizer.lineNum() << ": Type " << tok <<  " not defined" << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool isMath(std::string op){
    if(op == "+" || op == "-" || op == "/" || op == "*"){
        return true;
    }
    return false;
}
bool isBool(std::string op){
    if(op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!="){
        return true;
    }
    return false;
}

bool allowPush(std::string op){
    if(operatorStack.empty()){
        return true;
    }
    std::string top = operatorStack.top();
    int topVal = getOpVal(top);
    int newVal = getOpVal(op);
    if(op == "(" || top == "(" || op == "[" || top == "["){
        return true;
    }
    if(newVal > topVal){
        return true;
    }
    return false;
}

bool isValidL(sar s){
    Symbol sym = symbolTable.getSymbol(s.symID);
    if(sym.kind == "this"){
        return false;
    }
    if(s.type == "identifier" || s.type == "ref" || s.type == "arr" || s.type == "vSar"){
        return true;
    }
    return false;
}

void runRutine(){
    quadStruct newQuad;
    if(operatorStack.empty()){
        std::cout << "yo something happened in runRutine your operatorStack is empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string op = operatorStack.top();
    operatorStack.pop();
    if(SAS.empty()){
        std::cout << "yo something happened in runRutine your SAS0 is empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    sar top = SAS.top();
    SAS.pop();
    if(SAS.empty()){
        std::cout << "yo something happened in runRutine your SAS1 is empty" << std::endl;
        exit(EXIT_FAILURE);
    }
    sar bottom = SAS.top();
    SAS.pop();
    if(op == "="){
        if(!symbolTable.typeMatch(bottom, top) || !isValidL(bottom)){
            std::cout << tokenizer.lineNum() << ": Invalid Operation " << symbolTable.getSymbol(bottom.symID).getData("type") << " ";
            if(bottom.token.lexeme == ""){
                std::cout << bottom.symID;
            }
            else{
                std::cout << bottom.token.lexeme;
            }
            std::cout << " " <<  op << " " << symbolTable.getSymbol(top.symID).getData("type") << " ";
            if(top.token.lexeme == ""){
                std::cout << top.symID << std::endl;
            }
            else{
                std::cout << top.token.lexeme << std::endl;
            }
            exit(EXIT_FAILURE);
        }
        newQuad.op = "MOV";
        newQuad.arg1 = top.symID;
        newQuad.arg2 = bottom.symID;
    }
    
    if(isMath(op) || isBool(op) || op == "and" || op == "or"){
        if(!symbolTable.typeMatch(top, bottom, op)){
            Symbol topSym = symbolTable.getSymbol(top.symID);
            Symbol bottomSym = symbolTable.getSymbol(bottom.symID);
            if(bottom.token.lineNumber > 0){
                std::cout << bottom.token.lineNumber;
            }
            else{
                std::cout << tokenizer.lineNum();
            }
            std::cout <<  ": Invalid Operation " << bottomSym.getData("type") << " ";
            if(bottomSym.lexeme == ""){
                std::cout << bottomSym.symID;
            }
            else{
                std::cout << bottomSym.lexeme;
            }
             std::cout << " " << op << " " << topSym.getData("type") << " ";
            if(topSym.lexeme == ""){
                std::cout << topSym.symID;
            }
            else{
                std::cout << topSym.lexeme;
            }

            std::cout << std::endl;
            exit(EXIT_FAILURE);
        }
        Symbol tempSym = Symbol();
        tempSym.data["accessMod"] = "public";
        if(isMath(op)){
            tempSym.data["type"] = "int";
        }
        if(isBool(op) || op == "and" || op == "or"){
            tempSym.data["type"] = "bool";
        }
        tempSym.kind = "temp";
        std::string symID = symbolTable.push(tempSym);
        tSar tempSar = tSar();
        tempSar.symID = symID;
        tempSar.op = op;
        tempSar.bottom = bottom.symID;
        tempSar.top  = top.symID;
        SAS.push(tempSar);
        std::string tcodeOperator;
        if(op == "+"){
            tcodeOperator = "ADD";
        }
        else if(op == "-"){
            tcodeOperator = "SUB";
        }
        else if(op == "*"){
            tcodeOperator = "MUL";
        }
        else if(op == "/"){
            tcodeOperator = "DIV";
        }
        else if(op == "and"){
            tcodeOperator = "AND";
        }
        else if(op == "or"){
            tcodeOperator = "OR";
        }
        else if(op == "=="){
            tcodeOperator = "EQ";
        }
        else if(op == "!="){
            tcodeOperator = "NE";
        }
        else if(op == "<"){
            tcodeOperator = "LT";
        }
        else if(op == ">"){
            tcodeOperator = "GT";
        }
        else if(op == "<="){
            tcodeOperator = "LE";
        }
        else if(op == ">="){
            tcodeOperator = "GE";
        }

        newQuad.op = tcodeOperator;
        newQuad.arg1 = bottom.symID;
        newQuad.arg2 = top.symID;
        newQuad.arg3 = symID;
    }
    if(bottom.token.lineNumber > 0){
        std::string line = tokenizer.getCurLine(bottom.token.lineNumber);
        newQuad.lineStatement = line.erase(0, line.find_first_not_of(" "));
    }
    quadPush(newQuad);
}

void oPush(std::string op){
    while(!allowPush(op)){
        runRutine();
    }
    operatorStack.push(op);
}

void leftValError(sar thisSar){
    std::cout << "Sar " << thisSar.token.lexeme << " does not have a valid l value";
}

void createPushQuads(std::vector<sar> argList){
    for(int i = 0; i < argList.size(); i ++){
        quadStruct newQuad;
        newQuad.op = "PUSH";
        newQuad.arg1 = argList.at(i).symID;
        quadPush(newQuad);
    }
}

void rExist(){
    quadStruct newQuad;
    sar member = SAS.top();
    SAS.pop();
    sar classs = SAS.top();
    SAS.pop();
    if(!symbolTable.isMemberOf(classs.symID, member)){
        if(member.type == "func"){
            std::cout << tokenizer.lineNum() << ": Function " << member.token.lexeme << "(";
            for(int i = 0; i < member.argumentList.size(); i++){
                std::cout << symbolTable.getSymbol(member.argumentList.at(i).symID).data["type"];
                if(i < member.argumentList.size() - 1){
                    std::cout << ", ";
                }
            }
            std::cout << ") not defined/public in class " << symbolTable.getSymbol(classs.symID).data["type"] << std::endl;
            exit(EXIT_FAILURE);
        }
        else if(member.type == "arr"){
            std::cout << tokenizer.lineNum() << ": Array " << member.token.lexeme << " not defined/public in class " << symbolTable.getSymbol(classs.symID).data["type"] << std::endl;
        }
        else{
            std::cout << tokenizer.lineNum() << ": Variable " << member.token.lexeme <<  " not defined/public in class " << symbolTable.getSymbol(classs.symID).data["type"] << std::endl;
        }
        exit(EXIT_FAILURE);
    }
    Symbol tempSym = Symbol();
    Symbol memberSym = symbolTable.getMemberOf(classs.symID, member);
    Symbol arrayElementSym;
    tempSym.data["accessMod"] = "public";
    if(member.type == "arr"){
        tempSym.data["type"] = memberSym.data["type"].erase(0,2);
        // Bro who knows if this is correct I just created this variable because of the a.c[i] = y; example in xcode, this never goes on the SAS.
        arrayElementSym.data["type"] = memberSym.data["type"];
        arrayElementSym.kind = "rtemp";
        arrayElementSym.scope = symbolTable.getScope();
        arrayElementSym.data["accessMod"] = "public";
        arrayElementSym.symID = symbolTable.push(arrayElementSym);
    }
    else if(member.type == "func"){
        tempSym.data["type"] = memberSym.data["returnType"];
        quadStruct frameQuad;
        frameQuad.lineStatement = tokenizer.getCurLine(classs.token.lineNumber);
        frameQuad.op = "FRAME";
        frameQuad.arg1 = memberSym.symID;
        if(symbolTable.getSymbol(classs.symID).kind == "this"){
            frameQuad.arg2 = "this";
        }
        else{
            frameQuad.arg2 = classs.symID;
        }
        quadPush(frameQuad);
        createPushQuads(member.argumentList);
        quadStruct callFunctionQuad;
        callFunctionQuad.op = "CALL";
        callFunctionQuad.arg1 = memberSym.symID;
        quadPush(callFunctionQuad);

    }
    else{
        if(symbolTable.getSymbol(memberSym.symID).isFunction()){
            std::cout << tokenizer.lineNum() << ": Variable " << member.token.lexeme <<  " not defined/public in class " << symbolTable.getSymbol(classs.symID).data["type"] << std::endl;
            exit(EXIT_FAILURE);
        }
        tempSym.data["type"] = memberSym.data["type"];
    }
    member.symID = memberSym.symID;
    if(member.type != "func"){
        tempSym.kind = "rtemp";
    }
    else{
        tempSym.kind = "temp";
    }
    std::string symID = symbolTable.push(tempSym);
    refSar s = refSar();
    s.symID = symID;
    s.classs = classs.symID;
    s.mem = member.symID;
    SAS.push(s);
    if(member.type == "func" &&memberSym.data["returnType"] != "void"){
        quadStruct peekStruct;
        peekStruct.op = "PEEK";
        peekStruct.arg1 = s.symID;
        quadPush(peekStruct);
    }
    if(member.type == "arr"){
        newQuad.op = "REF";
        if(symbolTable.getSymbol(classs.symID).lexeme == "this"){
            newQuad.arg1 = "this";
        }
        else{
            newQuad.arg1 = classs.symID;
        }
        newQuad.arg2 = member.symID;
        newQuad.arg3 = arrayElementSym.symID;
        newQuad.lineStatement = tokenizer.getCurLine(classs.token.lineNumber);
        quadPush(newQuad);
    }
    else if(member.type != "func"){
        newQuad.op = "REF";
        if(symbolTable.getSymbol(classs.symID).lexeme == "this"){
            newQuad.arg1 = "this";
        }
        else{
            newQuad.arg1 = classs.symID;
        }        newQuad.arg2 = member.symID;
        newQuad.arg3 = symID;
        newQuad.lineStatement = tokenizer.getCurLine(classs.token.lineNumber);
        quadPush(newQuad);
    }
    if(member.type == "arr"){
        quadStruct arrayStruct;
        arrayStruct.op = "AEF";
        arrayStruct.arg1 = arrayElementSym.symID;
        arrayStruct.arg2 = member.arrayIndex;
        arrayStruct.arg3 = symID;
        quadPush(arrayStruct);
    }
}

void iExist(){
    quadStruct newQuad;
    sar thisSar = SAS.top();
    SAS.pop();
    Symbol newSymbol = Symbol();
    newSymbol.kind = "temp";
    newSymbol.data["accessMod"] = "public";
    std::string symID;
    bool isFunc = false;
    if(thisSar.type == "func"){
        isFunc = true;
        thisSar.type = "identifier";
        symID = symbolTable.getMethod(thisSar);
        if(symID == "NF"){
            std::cout << tokenizer.lineNum() << ": Function " << thisSar.token.lexeme << "(";
            for(int i = 0; i < thisSar.argumentList.size(); i++){
                std::cout << symbolTable.getSymbol(thisSar.argumentList.at(i).symID).data["type"];
                if(i < thisSar.argumentList.size() - 1){
                    std::cout << ", ";
                }
            }
            std::cout << ") not defined" << std::endl;
            exit(EXIT_FAILURE);
        }
        newSymbol.data["type"] = symbolTable.getSymbol(symID).data["returnType"];
        thisSar.symID = symbolTable.push(newSymbol);
        newQuad.op = "FRAME";
        if(symbolTable.getSymbol(symID).kind == "this"){
            newQuad.arg1 = "this";
        }
        else{
            newQuad.arg1 = symID;
        }
        newQuad.arg2 = "this";
        newQuad.lineStatement = tokenizer.getCurLine(tokenizer.lineNum());
        quadPush(newQuad);
        createPushQuads(thisSar.argumentList);
        quadStruct callFunctionQuad;
        callFunctionQuad.op = "CALL";
        callFunctionQuad.arg1 = symID;
        quadPush(callFunctionQuad);
        

    }
    else if(thisSar.type == "arr"){
        thisSar.type = "identifier";
        symID = symbolTable.inScopeArray(thisSar.token.lexeme);
        if(symID == "NF"){
            std::cout << tokenizer.lineNum() << ": Array " << thisSar.token.lexeme << " not defined" << std::endl;
            exit(EXIT_FAILURE);
        }
        newSymbol.kind = "rtemp";
        newSymbol.data["type"] = symbolTable.getSymbol(symID).data["type"].erase(0,2);
        thisSar.symID = symbolTable.push(newSymbol);
        newQuad.op = "AEF";
        newQuad.arg1 = symID;
        newQuad.arg2 = thisSar.arrayIndex;
        newQuad.arg3 = thisSar.symID;
        newQuad.lineStatement = tokenizer.getCurLine(thisSar.token.lineNumber);
        quadPush(newQuad);
    }
    else{
        symID = symbolTable.inScope(thisSar.token.lexeme);
        if(symID == "NF" || symbolTable.getSymbol(symID).isFunction() || symbolTable.isClassName(symbolTable.getSymbol(symID).lexeme)){
            std::cout << tokenizer.lineNum() << ": Variable " << thisSar.token.lexeme << " not defined" << std::endl;
            exit(EXIT_FAILURE);
        }
        thisSar.symID = symID;
    }
    SAS.push(thisSar);
    if(symbolTable.getSymbol(symID).data["returnType"] != "void" && isFunc){
        quadStruct peekStruct;
        peekStruct.op = "PEEK";
        peekStruct.arg1 = thisSar.symID;
        quadPush(peekStruct);
    }
}

void printAllTokens(std::string fileName) {
    Token a = tokenizer.getToken();
    a.print();
    while (a.type != "EOF") {
        tokenizer.next();
        a = tokenizer.getToken();
        a.print();
    }
}

void eoe(){
    if(!operatorStack.empty()){
        while(!operatorStack.empty()){
            runRutine();
        }
    }
    if(!SAS.empty()){
        while(SAS.top().symID[0] != 'M'){
            SAS.pop();
            if(SAS.empty()){
                break;
            }
        }
    }
}

void identifier(){
    if(tokenizer.isIdentifier()){
        tokenizer.next();
    }
    else{
        syntaxErrorLexeme("identifier");
    }
}

void className(){
    if(tokenizer.isIdentifier()){
        tokenizer.next();
    }
    else{
        syntaxErrorLexeme("className");
    }
}

void type(){
    if(sp){
        typePush();
    }
    if(tokenizer.isIdentifier()){
        className();
    }
    else if(tokenizer.isType()){
        tokenizer.next();
    }
    else{
        syntaxErrorLexeme("type");
    }
}

void syntaxErrorLexeme(std::string expected) {
    std::cout << std::to_string(tokenizer.lineNum()) << ": Found " << tokenizer.lexeme() << " expecting " << expected << std::endl;
    exit(EXIT_FAILURE);
}

void syntaxErrorType(std::string expected) {
    std::cout << std::to_string(tokenizer.lineNum()) << ": Found " << tokenizer.lexeme() << " expecting " << expected << std::endl;
    exit(EXIT_FAILURE);
}

void analyze() {
    compiliationUnit();
}

bool isModifier() {
    if (tokenizer.lexeme() == "public" || tokenizer.lexeme() == "private") {
        return true;
    }
    return false;
}

void modifier(){
    if(isModifier()){
        tokenizer.next();
    }
    else{
        syntaxErrorLexeme("modifier");
    }
}

void classMemberDeclaration() {
    symbol = Symbol();
    symbol.kind = "ivar";
    if (!isClassName() && !isModifier()) {
        syntaxErrorType("Class Name, Modifier");
    }
    if (isModifier()) {
        symbol.data["accessMod"] = tokenizer.lexeme();
        modifier();
        if (!tokenizer.isType()) {
            syntaxErrorType("Type");
        }
        
        if (tokenizer.peekToken().lexeme == "[") {
            std::string symtype = "@:";
            symtype.append(tokenizer.lexeme());
            symbol.data["type"] = symtype;
            type();
            if(sp){
                tExist();
            }
            tokenizer.next();
            if (tokenizer.lexeme() != "]") {
                syntaxErrorLexeme("]");
            }
            tokenizer.next();
        }
        else {
            symbol.data["type"] = tokenizer.lexeme();
            type();
            if(sp){
                tExist();
            }
        }
        symbol.lexeme = tokenizer.lexeme();
        Token tempToken = tokenizer.getToken();
        identifier();
        if(!sp){
            if(tokenizer.lexeme() == "("){
                dup(tempToken.lexeme, "function");
            }
            else{
                dup(tempToken.lexeme, "variable");
            }
        }
        else{
            vPush(tempToken);
        }
        fieldDeclaration();
    }
    else {
        constructorDeclaration();
    }
}
void fieldDeclaration() {
    if (tokenizer.lexeme() == ";") {
        tokenizer.next();
        if(sp){
            eoe();
            while(!SAS.empty()){
                SAS.pop();
            }
        }
        if(!sp){symbolTable.push(symbol);}
    }
    else if (tokenizer.lexeme() == "=") {
        if(sp){
            oPush(tokenizer.lexeme());
        }
        tokenizer.next();
        if(!sp){symbolTable.push(symbol);}
        assignmentExpression();
        if (tokenizer.lexeme() != ";") {
            syntaxErrorLexeme(";");
        }
        tokenizer.next();
        if(sp){
            eoe();
        }
    }
    else {
        std::string lexeme = symbol.lexeme;
        symbolTable.pushScope(symbol.lexeme);
        symbolTable.inFunction = true;
        if (tokenizer.lexeme() != "(") {
            syntaxErrorType("field_declaration");
        }
        symbol.data["returnType"] = symbol.data["type"];
        symbol.data.erase("type");
        symbol.kind = "method";
        tokenizer.next();
        if (tokenizer.lexeme() != ")") {
            parameterList();
        }
        if (tokenizer.lexeme() != ")") {
            syntaxErrorLexeme(") or ',' ");
        }
        tokenizer.next();
        symbolTable.popScope();
        if(!sp){
            symbolTable.push(symbol);
        }
        symbolTable.pushScope(symbol.lexeme);
        if(sp){
            quadStruct newQuad;
            if(SAS.top().token.lineNumber > 0){
                newQuad.lineStatement = tokenizer.getCurLine(SAS.top().token.lineNumber);
            }
            newQuad.arg1 = SAS.top().symID;
            newQuad.op = "FUNC";
            if(curLabel == ""){
                newQuad.label = SAS.top().symID;
            }
            else{
                newQuad.label = backTrack(SAS.top().symID);
                curLabel = "";
            }
            quadPush(newQuad);
        }
        methodBody();
        symbolTable.popScope();
        
        symbolTable.assignFuncSize(symbolTable.inScope(lexeme));
        
        symbolTable.inFunction = false;
    }
}

void parameterList() {
    parameter();
    while (tokenizer.lexeme() == ",") {
        tokenizer.next();
        parameter();
    }
}

void variableDeclaration() {
    symbol = Symbol();
    symbol.data["accessMod"] = "private";
    symbol.kind = localVariable;
    symbol.data["type"] = tokenizer.lexeme();
    type();
    if(sp){
        tExist();
    }
    if (tokenizer.lexeme() == "[") {
        std::string temp = "@:";
        symbol.data["type"] = temp.append(symbol.data["type"]);
        tokenizer.next();
        if (tokenizer.lexeme() != "]") {
            syntaxErrorLexeme("]");
        }
        tokenizer.next();
    }
    symbol.lexeme = tokenizer.lexeme();
    Token temp = tokenizer.getToken();
    identifier();
    if(!sp){
        dup(symbol.lexeme, "variable");
    }
    if(!sp){symbolTable.push(symbol);}
    symbol = Symbol();
    if(sp){
        vPush(temp);
    }
    if (tokenizer.lexeme() == "=") {
        if(sp){
            oPush(tokenizer.lexeme());
        }
        tokenizer.next();
        assignmentExpression();
    }
    if (tokenizer.lexeme() != ";") {
        syntaxErrorLexeme("; or =");
    }
    if(sp){
        eoe();
    }
    tokenizer.next();
    
}

void methodBody() {
    if(!sp){
        symbolTable.thisDeclarer();
    }
    if (tokenizer.lexeme() != "{") {
        syntaxErrorLexeme("{");
    }
    tokenizer.next();
    while (tokenizer.isType() && tokenizer.peekToken().lexeme != "=" && tokenizer.peekToken().lexeme != "." && tokenizer.peekToken().lexeme != "(") {
        //isclassnametag
        if (tokenizer.isIdentifier() && tokenizer.peekToken().lexeme == "[" && tokenizer.peek2Token().lexeme != "]") {
            break;
        }
        variableDeclaration();
    }
    while (tokenizer.isStatement()) {
        statement();
    }
    if (tokenizer.lexeme() != "}") {
        syntaxErrorLexeme("}, type, or expression");
    }
    if(sp){
        if(symbolTable.inConstructor){
            quadStruct endFunctionQuad;
            endFunctionQuad.op = "RETURN";
            endFunctionQuad.arg1 = "this";
            endFunctionQuad.lineStatement = tokenizer.getCurLine(tokenizer.lineNum());
            quadPush(endFunctionQuad);
        }
        else {
            quadStruct endFunctionQuad;
            endFunctionQuad.op = "RTN";
            endFunctionQuad.lineStatement = tokenizer.getCurLine(tokenizer.lineNum());
            quadPush(endFunctionQuad);
        }

    }
    tokenizer.next();
    if(sp){
        while(!SAS.empty()){
            SAS.pop();
        }
    }
}

void constructorDeclaration() {
    symbolTable.funcSize = -12;
    symbol.kind = "constructor";
    symbol.data["accessMod"] = "public";
    if (!tokenizer.isIdentifier()) {
        syntaxErrorType("class name");
    }
    symbol.scope = symbolTable.getScope();
    if(!sp){
        dup(tokenizer.lexeme(), "function");
        //cd();
    }
    if(sp){
        cd();
    }
    symbolTable.inFunction = true;
    symbolTable.pushScope(tokenizer.lexeme());
    symbolTable.inConstructor = true;
    symbol.lexeme = tokenizer.lexeme();
    symbol.data["returnType"] = tokenizer.lexeme();
    std::string ClassName = tokenizer.lexeme();
    className();
    if (tokenizer.lexeme() != "(") {
        syntaxErrorLexeme("(");
    }
    tokenizer.next();
    if (tokenizer.lexeme() != ")") {
        parameterList();
    }
    if (tokenizer.lexeme() != ")") {
        syntaxErrorLexeme(") or ','");
    }
    tokenizer.next();
    symbolTable.popScope();
    
    if(!sp){
        symbolTable.push(symbol);
    }
    if(sp){
        std::string actualClassName = symbolTable.getConstructorSymID(ClassName);
        if(actualClassName == "NF"){
            std::cout << "YO CHECK THIS ERROR MESSAGE OUT, CONSTRUCTOR NOT FOUND" << std::endl;
            exit(EXIT_FAILURE);
        }
        ClassName.append("staticFunction");
        quadStruct FuncQuad;
        FuncQuad.op = "FUNC";
        FuncQuad.arg1 = actualClassName;
        FuncQuad.label = actualClassName;
        quadPush(FuncQuad);
        
        quadStruct FrameQuad;
        FrameQuad.op = "FRAME";
        FrameQuad.arg1 = ClassName;
        FrameQuad.arg2 = "this";
        quadPush(FrameQuad);
        
        quadStruct callQuad;
        callQuad.op = "CALL";
        callQuad.arg1 = ClassName;
        quadPush(callQuad);
    }
    symbolTable.pushScope(symbol.lexeme);
    std::string symId = symbolTable.getConstructorSymIdFromClassName(symbol.lexeme);
    methodBody();
    symbolTable.inFunction = false;
    symbolTable.assignFuncSize(symId);
    symbolTable.popScope();
    symbolTable.inConstructor = false;
}

bool isClassName(int x) {
    return tokenizer.isIdentifier(x);
}

void compiliationUnit() {
    if(sp){
        quadStruct mainCaller;
        mainCaller.op = "FRAME";
        mainCaller.arg1 = "main";
        mainCaller.arg2 = "null";
        
        quadStruct callQuad;
        callQuad.op = "CALL";
        callQuad.arg1 = "main";
        quad.push_back(mainCaller);
        quad.push_back(callQuad);
        
        quadStruct kill;
        kill.op = "TRP";
        kill.arg1 = "0";
        quad.push_back(kill);
    }
    while (tokenizer.lexeme() == "class") {
        classDeclaration();
    }
    if (tokenizer.lexeme() != "void")
        syntaxErrorLexeme("void or class");
    tokenizer.next();
    if (tokenizer.lexeme() != "kxi2020")
        syntaxErrorLexeme("kxi2020");
    tokenizer.next();
    if (tokenizer.lexeme() != "main")
        syntaxErrorLexeme("main");
    Symbol sym;
    sym.lexeme = "main";
    sym.kind = "main";
    sym.data["returnType"] = "void";
    if(!sp){
        std::string symID = symbolTable.push(sym);
    }
    tokenizer.next();
    if (tokenizer.lexeme() != "(")
        syntaxErrorLexeme("(");
    tokenizer.next();
    if (tokenizer.lexeme() != ")")
        syntaxErrorLexeme(")");
    tokenizer.next();
    symbolTable.pushScope("main");
    symbolTable.inMain = true;
    if(sp){
        quadStruct mainQuad;
        mainQuad.op = "FUNC";
        mainQuad.arg1 = "main";
        mainQuad.label = "main";
        quad.push_back(mainQuad);
    }
    methodBody();
    symbolTable.assignFuncSize("main");
    
    symbolTable.inMain = false;
    symbolTable.popScope();
    
    if (tokenizer.lexeme() != "EOF") {
        syntaxErrorType("EOF");
    }
}

void classDeclaration() {
    symbol = Symbol();
    symbol.kind = "class";
    symbolTable.inClass = true;
    if (tokenizer.lexeme() != "class")
        syntaxErrorLexeme("class");
    tokenizer.next();
    if (!tokenizer.isIdentifier()) {
        syntaxErrorType("class_name");
    }
    std::string myClassName = tokenizer.lexeme();
    if(sp){
        quadStruct staticFunctionQuad;
        staticFunctionQuad.op = "FUNC";
        staticFunctionQuad.label = myClassName.append("staticFunction");
        staticFunctionQuad.arg1 = myClassName;
        quadPush(staticFunctionQuad);
    }
    symbol.lexeme = tokenizer.lexeme();
    if(!sp){
        symbolTable.classNames.push_back(tokenizer.lexeme());
    }
    std::string symId;
    if(!sp){
       symId = symbolTable.push(symbol);
    }
    symbolTable.pushScope(tokenizer.lexeme());
    if(!sp){
        dup(tokenizer.lexeme(), "class");
    }
    className();
    if (tokenizer.lexeme() != "{")
        syntaxErrorLexeme("{");
    tokenizer.next();
    
    while ((isModifier() && tokenizer.isType(1)) || isClassName(0)) {
        classMemberDeclaration();
    }
    if (tokenizer.lexeme() != "}"){
        std::cout << std::to_string(tokenizer.lineNum()) << ": Found " << tokenizer.lexeme() << " expecting " << "}, modifier, or constructor" << std::endl;
        exit(EXIT_FAILURE);
    }
    symbolTable.popScope();

    tokenizer.next();
    symbolTable.assignClassSize(symId);
    if(sp){
        quadStruct returnQuad;
        returnQuad.op = "RTN";
        quadPush(returnQuad);
        squadToQuad();
    }
    symbolTable.inClass = false;

}
std::string backTrack(std::string newLabel){
    for(int i = 0; i < quad.size(); i++){
        if(quad.at(i).label == curLabel){
            quad.at(i).label = newLabel;
        }
        else if(quad.at(i).arg1 == curLabel){
            quad.at(i).arg1 = newLabel;
        }
        else if(quad.at(i).arg2 == curLabel){
            quad.at(i).arg2 = newLabel;
        }
        else if(quad.at(i).arg3 == curLabel){
            quad.at(i).arg3 = newLabel;
        }
    }
    return newLabel;
}
std::string getLabel(std::string type){
        std::string label = curLabel;
        curLabel = "";
        return label;
}

void statement() {
    if(sp){
        //SUUUPER HACKY
        while(!SAS.empty() && SAS.top().type == "vSar" && SAS.top().symID[0] != 'M'){
            SAS.pop();
        }
    }
    std::string topLex = tokenizer.lexeme();
    if (topLex == "{") {
        tokenizer.next();
        while (tokenizer.isStatement()) {
            statement();
        }
        if (tokenizer.lexeme() != "}") {
            syntaxErrorLexeme("}");
        }
        
        tokenizer.next();
        
    }
    else if (topLex == "if") {
        tokenizer.next();
        if (tokenizer.lexeme() != "(") {
            syntaxErrorLexeme("(");
        }
        if(sp){
            oPush(tokenizer.lexeme());
        }
        tokenizer.next();
        expression();
        if (tokenizer.lexeme() != ")") {
            syntaxErrorLexeme(") or expression");
        }
        tokenizer.next();
        if(sp){
            closingParen();
            iff();
        }
        statement();
        if(sp && tokenizer.lexeme() != "else"){
            if(curLabel == ""){
                curLabel = symbolTable.ifStack.top();
            }
            else{
                curLabel = backTrack(symbolTable.ifStack.top());
            }
            symbolTable.ifStack.pop();
        }
        if (tokenizer.lexeme() == "else") {
            quadStruct newQuad;
            tokenizer.next();
            if(sp){
                newQuad.op = "JMP";
                newQuad.arg1 = symbolTable.genLabel("else");
                quadPush(newQuad);
            }
            if(sp){
                if(curLabel == ""){
                    curLabel = symbolTable.ifStack.top();
                }
                else{
                    curLabel = backTrack(symbolTable.ifStack.top());
                }
                symbolTable.ifStack.pop();
            }
            statement();
            if(sp){
                if(curLabel == ""){
                    curLabel = symbolTable.elseStack.top();
                }
                else{
                    curLabel = backTrack(symbolTable.elseStack.top());
                }
                symbolTable.elseStack.pop();
            }
        }
    }
    else if (topLex == "while") {
        tokenizer.next();
        if(sp){
            if(curLabel == ""){
                curLabel = symbolTable.genLabel("while");
            }
            else{
                curLabel = backTrack(symbolTable.genLabel("while"));
            }
        }
 
        if (tokenizer.lexeme() != "(") {
            syntaxErrorLexeme("(");
        }
        if(sp){
            oPush(tokenizer.lexeme());
        }
        tokenizer.next();
        expression();
        if (tokenizer.lexeme() != ")") {
            syntaxErrorLexeme(")");
        }
        if(sp){
            closingParen();
            whilee();
        }
        tokenizer.next();

        statement();
        quadStruct newQuad;

        if(sp){
            newQuad.op = "JMP";
            newQuad.arg1 = symbolTable.whileStack.top();
            symbolTable.whileStack.pop();
            quadPush(newQuad);
            if(curLabel == ""){
                curLabel = symbolTable.endWhileStack.top();
                symbolTable.endWhileStack.pop();
            }
            else{
                curLabel = backTrack(symbolTable.endWhileStack.top());
                symbolTable.endWhileStack.pop();
            }
        }
    }
    else if (topLex == "return") {
        tokenizer.next();
        if (tokenizer.isExpression()) {
            expression();
        }
        if (tokenizer.lexeme() != ";")
        {
            syntaxErrorLexeme(";");
        }
        tokenizer.next();
        if(sp){
            returnn();
        }
    }
    else if (topLex == "cout") {
        tokenizer.next();
        if (tokenizer.lexeme() != "<<") {
            syntaxErrorLexeme("<<");
        }
        tokenizer.next();
        expression();
        if (tokenizer.lexeme() != ";") {
            syntaxErrorLexeme(";");
        }
        tokenizer.next();
        if(sp){
            quadStruct newQuad;
            newQuad.op = "WRITE";
            newQuad.arg1 = SAS.top().symID;
            if(SAS.top().token.lineNumber > 0){
                newQuad.lineStatement = tokenizer.getCurLine(SAS.top().token.lineNumber);
            }
            quadPush(newQuad);
            coutt();
            
        }
    }
    else if (topLex == "cin") {
        tokenizer.next();
        if (tokenizer.lexeme() != ">>") {
            syntaxErrorLexeme(">>");
        }
        tokenizer.next();
        expression();
        if (tokenizer.lexeme() != ";") {
            syntaxErrorLexeme(";");
        }
        tokenizer.next();
        if(sp){
            quadStruct newQuad;
            newQuad.op = "READ";
            newQuad.arg1 = SAS.top().symID;
            newQuad.lineStatement = tokenizer.getCurLine(SAS.top().token.lineNumber);
            quadPush(newQuad);
        }

        if(sp){
            cin();
        }
    }
    else if (topLex == "switch") {
        tokenizer.next();
        if (tokenizer.lexeme() != "(") {
            syntaxErrorLexeme("(");
        }
        tokenizer.next();
        expression();
        if (tokenizer.lexeme() != ")") {
            syntaxErrorLexeme(")");
        }
        tokenizer.next();
        caseBlock();
    }
    else if (topLex == "break") {
        tokenizer.next();
        if (tokenizer.lexeme() != ";") {
            syntaxErrorLexeme(";");
        }
        tokenizer.next();
    }
    else if (tokenizer.isExpression()) {
        expression();
        if (!tokenizer.isSemiColon()) {
            syntaxErrorLexeme("; or expression");
        }
        else {
            if(sp){
                eoe();
            }
            tokenizer.next();
        }
    }
    else {
        syntaxErrorType("Expression");
    }
}

void expression() {
    std::string topLexeme = tokenizer.lexeme();
    if (tokenizer.isIdentifier()) {
        if(sp){
            iPush();
        }
        identifier();
        if (tokenizer.lexeme() == "[" || tokenizer.lexeme() == "(") {
            fnArrMember();
        }
        if(sp){
            iExist();
        }
        if (tokenizer.lexeme() == ".") {
            memberRefz();
        }
        if (tokenizer.isExpressionZ()) {
            expressionz();
        }
    }
    else if (topLexeme == "(") {
        if(sp){
            oPush(topLexeme);
        }
        tokenizer.next();
        if (tokenizer.lexeme() == ")") {
            closingParen();
            tokenizer.next();
        }
        else {
            expression();
            if (tokenizer.lexeme() != ")") {
                syntaxErrorLexeme(")");
            }
            else {
                if(sp){
                    closingParen();
                }
                tokenizer.next();
                if (tokenizer.isExpressionZ()) {
                    expressionz();
                }
            }
        }
    }
    else if (topLexeme == "true" || topLexeme == "false" || topLexeme == "null") {
        if(sp){
            lPush();
        }
        tokenizer.next();
        if (tokenizer.isExpressionZ()) {
            expressionz();
        }
    }
    else if (topLexeme == "this") {
        if(sp){
            iPush();
            iExist();
        }
        tokenizer.next();
        if (tokenizer.lexeme() == ".") {
            memberRefz();
        }
        if (tokenizer.isExpressionZ()) {
            expressionz();
        }
    }
    else if (tokenizer.isNumericLiteral()) {
        numbericLiteral();
        if (tokenizer.isExpressionZ()) {
            expressionz();
        }
    }
    else if (tokenizer.isCharacterLiteral()) {
        characterLiteral();
        
        if (tokenizer.isExpressionZ()) {
            expressionz();
        }
        
    }
    else {
        syntaxErrorType("expression");
    }
    
}

void expressionz() {
    if (!tokenizer.isExpressionZ())
        syntaxErrorType("ExpressionZ");
    else {
        if(sp){
            oPush(tokenizer.lexeme());
        }
        if (tokenizer.lexeme() == "=") {
            tokenizer.next();
            assignmentExpression();
        }
        else {
            tokenizer.next();
            expression();
        }
    }
}

void assignmentExpression() {
    if (tokenizer.lexeme() == "new") {
        tokenizer.next();
        type();
        newDeclaration();
    }
    else if (tokenizer.isExpression()) {
        expression();
    }
    else {
        syntaxErrorType("assignment expression");
    }
}

void newDeclaration() {
    if (tokenizer.lexeme() == "(") {
        if(sp){
            oPush(tokenizer.lexeme());
            bal();
        }
        tokenizer.next();
        if (tokenizer.lexeme() != ")") {
            argumentList();
        }
        if (tokenizer.lexeme() != ")") {
            syntaxErrorLexeme(")");
        }
        tokenizer.next();
        
        if(sp){
            closingParen();
            eal();
            newObj();
        }
    }
    else {
        if (tokenizer.lexeme() != "[") {
            syntaxErrorLexeme("[, (");
        }
        if(sp){
            oPush(tokenizer.lexeme());
        }
        tokenizer.next();
        expression();
        if (tokenizer.lexeme() != "]") {
            syntaxErrorLexeme("]");
        }
        if(sp){
            endBracket();
            newArray();
        }
        tokenizer.next();
        
    }
    
}

void fnArrMember() {
    if (tokenizer.lexeme() == "(") {
        if(sp){
            oPush(tokenizer.lexeme());
            bal();
        }
        tokenizer.next();
        if (tokenizer.lexeme() != ")")
        {
            argumentList();
            if (tokenizer.lexeme() != ")") {
                syntaxErrorLexeme(")");
            }
            tokenizer.next();
        }
        else{
            tokenizer.next();
        }
        if(sp){
            closingParen();
            eal();
            func();
        }
    }
    else {
        if (tokenizer.lexeme() != "[") {
            syntaxErrorLexeme("(, [");
        }
        if(sp){
            oPush(tokenizer.lexeme());
        }
        tokenizer.next();
        expression();
        if (tokenizer.lexeme() != "]") {
            syntaxErrorLexeme("] or expression");
        }
        if(sp){
            endBracket();
            arr();
        }
        tokenizer.next();
    }
}

void numbericLiteral() {
    if (!tokenizer.isNumericLiteral()) {
        syntaxErrorType("numberic_literal");
    }
    else {
        if (tokenizer.lexeme() == "+" || tokenizer.lexeme() == "-") {
            if(tokenizer.lexeme() == "-"){
                tokenizer.next();
                number(true);
            }
            else{
                tokenizer.next();
                number(false);
            }
        }
        else {
            number(false);
        }
    }
}

void argumentList() {
    expression();
    while (tokenizer.lexeme() == ",") {
        if(sp){
            poundComma();
        }
        tokenizer.next();
        expression();
    }
}

void memberRefz() {
    if (tokenizer.lexeme() != ".") {
        syntaxErrorLexeme(".");
    }
    tokenizer.next();
    if(sp){
        iPush();
    }
    identifier();
    if (tokenizer.lexeme() == "[" || tokenizer.lexeme() == "(") {
        fnArrMember();
    }
    if(sp){
        rExist();
    }
    if (tokenizer.lexeme() == ".") {
        memberRefz();
    }
}

void number(bool isNeg) {
    symbol = Symbol();
    symbol.lexeme = "";
    if(isNeg){
        symbol.lexeme.append("-").append(tokenizer.lexeme());
    }
    else{
        symbol.lexeme.append(tokenizer.lexeme());
    }
    if (tokenizer.type() != "number") {
        syntaxErrorType("number");
    }
    if (!std::count(numberVector.begin(), numberVector.end(), symbol.lexeme)) {
        symbol.kind = "ilit";
        symbol.data["type"] = "int";
        symbol.data["accrssMod"] = "public";
        if(!sp){symbolTable.push(symbol);}
        numberVector.push_back(symbol.lexeme);
    }
    if(isNeg){
        tokenizer.setCurLexeme(symbol.lexeme);
    }
    if(sp){
        lPush();
    }
    tokenizer.next();
}

void caseBlock() {
    if (tokenizer.lexeme() != "{") {
        syntaxErrorLexeme("{");
    }
    tokenizer.next();
    while (tokenizer.lexeme() == "case") {
        caseLabel();
    }
    if (tokenizer.lexeme() != "default") {
        syntaxErrorLexeme("default");
    }
    tokenizer.next();
    if (tokenizer.lexeme() != ":") {
        syntaxErrorLexeme(":");
    }
    tokenizer.next();
    statement();
    if (tokenizer.lexeme() != "}") {
        syntaxErrorType("}");
    }
    tokenizer.next();
}

void caseLabel() {
    if (tokenizer.lexeme() != "case") {
        syntaxErrorLexeme("case");
    }
    tokenizer.next();
    literal();
    if (tokenizer.lexeme() != ":") {
        syntaxErrorLexeme(":");
    }
    tokenizer.next();
    statement();
}

void literal() {
    if (tokenizer.isCharacterLiteral()) {
        characterLiteral();
    }
    else if (tokenizer.isNumericLiteral()) {
        numbericLiteral();
    }
    else {
        syntaxErrorType("literal");
    }
}

void characterLiteral() {
    if (tokenizer.type() == "character")
        character();
    else if(tokenizer.type() == "nonprintable_ascii")
        tokenizer.next();
    else {
        syntaxErrorType("characterLiteral");
    }
}

void character() {
    symbol = Symbol();
    if (tokenizer.type() == "character") {
        if (!std::count(characterVector.begin(), characterVector.end(), tokenizer.lexeme())) {
            symbol.lexeme = tokenizer.lexeme();
            symbol.kind = "clit";
            symbol.data["type"] = "char";
            symbol.data["accrssMod"] = "public";
            if(!sp){symbolTable.push(symbol);}
            characterVector.push_back(tokenizer.lexeme());
        }
        if(sp){
            lPush();
        }
        tokenizer.next();
    }
    else
        syntaxErrorType("character");
    
}

void parameter() {
    paramSymbol = Symbol();
    paramSymbol.kind = "param";
    paramSymbol.data["accessMod"] = "private";
    std::string symtype = tokenizer.lexeme();
    paramSymbol.data["type"] = symtype;
    type();
    if(sp){
        tExist();
    }
    if (tokenizer.lexeme() == "[") {
        std::string temp = "@:";
        symtype = temp.append(symtype);
        paramSymbol.data["type"] = symtype;
        tokenizer.next();
        if (tokenizer.lexeme() != "]") {
            syntaxErrorLexeme("]");
        }
        tokenizer.next();
    }
    paramSymbol.lexeme = tokenizer.lexeme();
    if(!sp){
        dup(paramSymbol.lexeme, "variable");
    }
    if(!sp){
        symbol.param.push_back(symbolTable.push(paramSymbol));
    }
    
    identifier();
}
void tCodeDude(){
    tCode x = tCode(quad, symbolTable);
}

int main(int argc, char* argv[]) {
    std::string fileName = "Resource/test.kxi";
    if (argv[1]) {
        fileName = argv[1];
    }
    tokenizer = Tokenizer(fileName);
    //printAllTokens(fileName);
    analyze();
    sp = true;
    tokenizer = Tokenizer(fileName);
    analyze();
    symbolTable.print();
    printQuad();
    tCodeDude();
    return 0;
}
