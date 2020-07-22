//
//  tCode.cpp
//  lexical
//
//  Created by Colby Barton on 3/11/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//


#include "tCode.hpp"

tCode::tCode(std::vector<quadStruct> quad, SymbolTable symbolTable){
    this->quad = quad;
    this->symbolTable = symbolTable;
    this->tCodeComplete();
}



void tCode::tCodeComplete(){
    file.open("test1.asm");
    globalize();
    neededValues();
    print("\n \n \n");
    bool first = true;
    for(auto x: quad){
        print("\n ;;;; ", false);
        std::string quadString = x.printStruct();
        file << quadString;
        curQuad = x;
        if(x.label != ""){
            print(x.label + " ", false);
        }
        if(first){
            first = false;
            print("START LDR R0, ZERO");
            print("LDR R1, ONE");
        }
        if(curQuad.op == "FRAME"){
            frame();
        }
        else if (curQuad.op == "CALL"){
            call();
        }
        else if(curQuad.op == "TRP"){
            trap();
        }
        else if(curQuad.op == "FUNC"){
            func();
        }
        else if(curQuad.op == "MOV"){
            move();
        }
        else if(curQuad.op == "ADD"){
            add();
        }
        else if(curQuad.op == "WRITE"){
            write();
        }
        else if(curQuad.op == "READ"){
            read();
        }
        else if(curQuad.op == "RTN"){
            rtn();
        }
        else if(curQuad.op == "MUL"){
            mul();
        }
        else if(curQuad.op == "DIV"){
            div();
        }
        else if(curQuad.op == "SUB"){
            sub();
        }
        else if(curQuad.op == "PUSH"){
            push();
        }
        else if(curQuad.op == "EQ"){
            eq();
        }
        else if(curQuad.op == "LT"){
            lt();
        }
        else if(curQuad.op == "GT"){
            gt();
        }
        else if(curQuad.op == "NE"){
            ne();
        }
        else if(curQuad.op == "LE"){
            le();
        }
        else if(curQuad.op == "GE"){
            ge();
        }
        else if(curQuad.op == "BF"){
            bf();
        }
        else if (curQuad.op == "AND"){
            andd();
        }
        else if(curQuad.op == "OR"){
            orr();
        }
        else if (curQuad.op == "JMP"){
            jmp();
        }
        else if(curQuad.op == "RETURN"){
            returnn();
        }
        else if(curQuad.op == "PEEK"){
            peek();
        }
        else if(curQuad.op == "NEWI"){
            newi();
        }
        else if(curQuad.op == "REF"){
            ref();
        }
        else if(curQuad.op == "NEW"){
            neww();
        }
        else if(curQuad.op == "AEF"){
            aef();
        }
        else{
            print("YOooOOOoOOOOOO NO command herhrerear agaklvjakljvksdjf;klasdjflaksjdflkasjfsakdj;asljf");
        }
        resetReg();
    }
    overUnder();

    file.close();
}

void tCode::read(){
    getLocation(curQuad.arg1);
    if(symbolTable.getSymbol(curQuad.arg1).getData("type") == "char"){
        print("TRP 2");
        print("STB R3, R5");
    }
    else{
        print("TRP 4");
        print("STR R3, R5");
    }
}

void tCode::aef(){
//    if(curQuad.arg1[0] != 'R'){
//        getLocation(curQuad.arg1);
//        print("LDR R5, R5");
//    }
//    else{
//        getLocation(curQuad.arg1);
//    }
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    if(symbolTable.getSymbol(curQuad.arg1).getData("type") != "@:char" && symbolTable.getSymbol(curQuad.arg1).getData("type") != "char"){
        print("LDR R3, N0");
        print("MUL R4, R3");
    }
    print("ADD R5, R4");
    print("MOV R3, FP");
    print("ADI R3, #" + std::to_string(symbolTable.getSymbol(curQuad.arg3).offset));
    print("STR R5, R3");
}

void tCode::neww(){
    print("MOV R4, SL");
    getValue(curQuad.arg1);
    print("ADD SL, R5");
    getLocation(curQuad.arg2);
    print("STR R4, R5");
}

void tCode::peek(){
    if(curQuad.arg1 == "T251"){
        int a = 1;
    }
    getLocation(curQuad.arg1);
    print("LDR R4, SP");
    print("STR R4, R5");
}

void tCode::neededValues(){
    print("ONE .INT 1");
    print("TWO .INT 2");
    print("ZERO .INT 0");
}

void tCode::ref(){
    getValue(curQuad.arg1);
    useReg = 4;
    print("ADI R5, #" + std::to_string(symbolTable.getSymbol(curQuad.arg2).offset));
    print("MOV R3, FP");
    print("ADI R3, #" + std::to_string(symbolTable.getSymbol(curQuad.arg3).offset));
    print("STR R5, R3");
}

void tCode::newi(){
    print("MOV R6, SL");
    print("ADI SL, #" + curQuad.arg1);
    print("MOV R5, FP");
    print("ADI R5, #" + std::to_string(symbolTable.getSymbol(curQuad.arg2).offset));
    print("STR R6, R5");
}

std::string tCode::getLabel(){
    labelNum++;
    return "compareLabel" + std::to_string(labelNum) + " ";
}

void tCode::eq(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("CMP R5, R4");
    std::string label = getLabel();
    std::string label2 = getLabel();
    getLocation(curQuad.arg3);
    print("BRZ R5, " + label);
    print("MOV R5, R0");
    print("JMP " + label2);
    print(label + " MOV R5, R1");
    print(label2 + " STR R5, R4");
}

void tCode::jmp(){
    print("JMP " + curQuad.arg1);
}

void tCode::andd(){
    std::string label = getLabel();
    std::string label2 = getLabel();
    
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    useReg = 3;
    getLocation(curQuad.arg3);
    
    print("CMP R5, R1");
    print("BNZ R5, " + label);
    print("CMP R4, R1");
    print("BNZ R5, " + label);
    print("MOV R5, R1");
    print("JMP " + label2);
    print(label + " MOV R5, R0");
    print(label2 + " STR R5, R3");
}

void tCode::orr(){
    std::string label = getLabel();
    std::string label2 = getLabel();
    
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    useReg = 3;
    getLocation(curQuad.arg3);
    print("CMP R5, R1");
    print("BRZ R5, " + label);
    print("CMP R4, R1");
    print("BRZ R4, " + label);
    print("MOV R5, R0");
    print("JMP " + label2);
    print(label + "MOV R5, R1");
    print(label2 + "STR R5, R3");
}

void tCode::ge(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("CMP R5, R4");
    std::string label = getLabel();
    std::string label2 = getLabel();
    getLocation(curQuad.arg3);
    print("BGT R5, " + label);
    print("BRZ R5, " + label);
    print("MOV R5, R0");
    print("JMP " + label2);
    print(label + " MOV R5, R1");
    print(label2 + " STR R5, R4");
}

void tCode::le(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("CMP R5, R4");
    std::string label = getLabel();
    std::string label2 = getLabel();
    getLocation(curQuad.arg3);
    print("BLT R5, " + label);
    print("BRZ R5, " + label);
    print("MOV R5, R0");
    print("JMP " + label2);
    print(label + " MOV R5, R1");
    print(label2 + " STR R5, R4");
}

void tCode::ne(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("CMP R5, R4");
    std::string label = getLabel();
    std::string label2 = getLabel();
    getLocation(curQuad.arg3);
    print("BNZ R5, " + label);
    print("MOV R5, R0");
    print("JMP " + label2);
    print(label + " MOV R5, R1");
    print(label2 + " STR R5, R4");
}
void tCode::gt(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("CMP R5, R4");
    std::string label = getLabel();
    std::string label2 = getLabel();
    getLocation(curQuad.arg3);
    print("BGT R5, " + label);
    print("MOV R5, R0");
    print("JMP " + label2);
    print(label + " MOV R5, R1");
    print(label2 + " STR R5, R4");
}
void tCode::lt(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("CMP R5, R4");
    std::string label = getLabel();
    std::string label2 = getLabel();
    getLocation(curQuad.arg3);
    print("BLT R5, " + label);
    print("MOV R5, R0");
    print("JMP " + label2);
    print(label + " MOV R5, R1");
    print(label2 + " STR R5, R4");
}

void tCode::bf(){
    getValue(curQuad.arg1);
    print("BRZ R5, " + curQuad.arg2);
}

void tCode::push(){
    if(curQuad.arg1 == "this120"){
        int x = 2;
    }
    getValue(curQuad.arg1, true);
    print("STR R5, SP");
    four();
}

void tCode::resetReg(){
    useReg = 5;
}

void tCode::move(){
    getLocation(curQuad.arg2);
    useReg = 4;
    getValue(curQuad.arg1);
    print("STR R4, R5");
}

void tCode::returnn(){
    useReg = 3;
    getValue(curQuad.arg1);
    //SP = FP
    print("MOV SP, FP ; SP = FP");
    //Test for underflow
    print(";TEST FOR UNDERFLOW");
    print("MOV R5, SP");
    print("CMP R5, SB");
    print("BGT R5, UNDERFLOW");
    //FP = PFP
    print(";FP = PFP");
    print("MOV R5, FP ; R5 will be the acutal FP = PFP"); //R5 will be the actual FP = PFP
    print("MOV R4, FP ; R4 will be the return addy"); // R4 WILL BE THE RETURN ADDRESS
    print("ADI R5, #-4");
    print("LDR FP, R5");
    //Retrieve return address from stack (R4 is grabbed above).
    print("LDR R4, R4 ;retrieve return addy from stack (R4 is grabbed above");
    //Put return value on the stack
    print("STR R3, SP ; Put return value on stack");
    //JMP to return address
    print("JMR R4 ; jump to return addy");
}

void tCode::rtn(){
    //SP = FP
    print("MOV SP, FP");

    //Test for underflow
    print("MOV R5, SP");
    print("CMP R5, SB");
    print("BGT R5, UNDERFLOW");
    //FP = PFP
    print("MOV R5, FP"); //R5 will be the actual FP = PFP
    print("MOV R4, FP"); // R4 WILL BE THE RETURN ADDRESS
    print("ADI R5, #-4");
    print("LDR FP, R5");
    //Retrieve return address from stack (R4 is grabbed above).
    print("LDR R4, R4");
    //JMP to return address
    print("JMR R4");
}

void tCode::write(){
    useReg = 3;
    getValue(curQuad.arg1);
    if(symbolTable.getSymbol(curQuad.arg1).getData("type") == "int"){
        print("TRP 1");
    }
    else{
        print("TRP 3");
    }
}

void tCode::add(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("ADD R5, R4");
    getLocation(curQuad.arg3); // this will use R4 as well.
    print("STR R5, R4");
}

void tCode::mul(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("MUL R5, R4");
    getLocation(curQuad.arg3); // this will use R4 as well.
    print("STR R5, R4");
}

void tCode::div(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("DIV R5, R4");
    getLocation(curQuad.arg3); // this will use R4 as well.
    print("STR R5, R4");
}

void tCode::sub(){
    getValue(curQuad.arg1);
    useReg = 4;
    getValue(curQuad.arg2);
    print("SUB R5, R4");
    getLocation(curQuad.arg3); // this will use R4 as well.
    print("STR R5, R4");
}

void tCode::func(){
    Symbol symbol = symbolTable.getSymbol(curQuad.arg1);
    std::string symId;
    int size;
    if(curQuad.arg1.length() > 10){
        std::string name = curQuad.arg1;
        name.resize(curQuad.arg1.length() - 14);
        symId = symbolTable.getClassSymId(name);
        size = (symbolTable.getSymbol(symId).size * -1) - 12;
    }
    else{
        symId = symbolTable.getSymbol(curQuad.arg1).symID;
        size = symbolTable.getSymbol(symId).size;
    }
    print("ADI SP, #" + std::to_string(size));
}

void tCode::call(){
    //Get PC
    print("MOV R6, PC");
    //Compute Return Address
    print("ADI R6, #36");
    //Store Return Addy
    print("STR R6, FP");
    //Jump baby
    print("JMP " + curQuad.arg1);
}

void tCode::frame(){
    Symbol temp = symbolTable.getSymbol(curQuad.arg1);
    int size = symbolTable.getSymbol(curQuad.arg1).size;
//    if(curQuad.arg1.length() > 12){
//        size = symbolTable.getSymbol(symbolTable.getConstructorSymIdFromConstructorSymId(curQuad.arg1)).size;
//    }
    useReg = 6;
    if(curQuad.arg2 != "this"){
        getLocation(curQuad.arg2);
    }
    else{
        print("MOV R6, FP");
        print("ADI R6, #-8");
    }
    print("LDR R6, R6");
    //Test overflow
    print("MOV R5, SP");
    print("ADI R5, #" + std::to_string(size));
    print("CMP R5, SL");
    print("BLT R5, OVERFLOW");
    //Save off current FP in a register, this will be the PFP
    print("MOV R2, FP ; Old Frame");
    //Point at current activation record (FP = SP)
    print("MOV FP, SP ; New Frame");
    //Adjust Stack Pointer for Return ADDY
    four();
    //Store PFP to top of the Stack
    print("STR R2, SP ; Set PFP");
    //Adjust Stack pointer for PFP
    four();
    
    //Store this pointer on top of stack
    print("STR R6, SP ; Set this on stack");
    //Adjust SP
    four();
}

void tCode::four(){
    print("ADI SP, #-4");
}

void tCode::trap(){
    if(curQuad.arg1 == "0"){
        print("TRP 0");
    }
    else if(curQuad.arg1 == "1"){
        print("TRP 1");
    }
    else if(curQuad.arg1 == "3"){
        print("TRP 3");
    }
    else if(curQuad.arg1 == "4"){
        print("TRP 4");
    }
}

void tCode::globalize(){
    print("number .int 10");
    for(auto x : symbolTable.symbols){
        if(x.second.scope == "g."){
            if(x.second.kind == "ilit"){
                print(x.first + " .INT " + x.second.lexeme + " \n");
            }
            else if(x.second.kind == "clit"){
                //newlines
                if(x.second.lexeme[1] == '\\'){
                    print(x.first + " .BYT " + "10");
                }
                else{
                    print(x.first + " .BYT " + std::to_string((int)x.second.lexeme[1]) + " \n");
                }
            }
            else if(x.second.kind == "true"){
                print(x.first + " .INT 1 \n");
            }
            else if(x.second.kind == "false" || x.second.kind == "null"){
                print(x.first + " .INT 0 \n");
            }
        }
    }
}

bool tCode::isGlobal(Symbol symbol){
    if(symbol.scope == "g." && (symbol.kind == "ilit" || symbol.kind == "clit" || symbol.kind == "true" || symbol.kind == "false" || symbol.kind == "null")){
        return true;
    }
    return false;
}

bool tCode::checkStack(Symbol symbol){
    if((symbol.kind == "lvar" || symbol.kind == "param" || symbol.kind == "temp" || symbol.kind == "rtemp") && symbol.offset < 1){
        return true;
    }
    return false;
}



bool tCode::checkHeap(Symbol symbol){
    if(symbol.kind == "ivar" || symbol.data["inClass"] == "true"){
        return true;
    }
    return false;
}

bool tCode::checkInstruction(Symbol symbol){
    return true;
}



void tCode::overUnder(){
    
    print("OVERFLOW LDR R3, ZERO");
    print("TRP 1");
    print("LDR R3, ONE");
    print("TRP 1");
    print("LDR R3, ZERO");
    print("TRP 1");
    print("LDR R3, ONE");
    print("TRP 1");
    print("LDR R3, ZERO");
    print("TRP 1");
    print("LDR R3, ONE");
    print("TRP 1");
    print("TRP 1");
    print("TRP 0");

    print("UNDERFLOW LDR R3, ZERO");
    print("TRP 1");
    print("LDR R3, ONE");
    print("TRP 1");
    print("LDR R3, ZERO");
    print("TRP 1");
    print("LDR R3, ONE");
    print("TRP 1");
    print("LDR R3, ZERO");
    print("TRP 1");
    print("TRP 1");
    print("TRP 0");
}

std::string tCode::getLocation(std::string symId){
    Symbol symbol = symbolTable.getSymbol(symId);
    bool global; //If the operand was defined as a global variable. Kxi doesn’t
    //support global variables, however a literal is handled like a global
    //variable.
    bool isStack; //If the operand is a passed-parameter of a function, a local variable
    //of a function or a temporary variable of a function
    bool isHeap; //If the operand is an instance variable or an array element. Arrays
    //and instances can not be created on the Stack in kxi.
    bool isInstruction; //If the operand is immediate (e.g., ADI A, #42).
    std::string reg = "R";
    reg.append(std::to_string(useReg));
    if(isGlobal(symbol)){
        global = true;
        print("LDR " + reg + ", " + symbol.symID);
    }
    else if(checkStack(symbol)){
        isStack = true;
        print("MOV " + reg + ", FP");
        print("ADI " + reg + ", #" + std::to_string(symbol.offset));
        if(symbol.symID[0] == 'R'){
            print("LDR " + reg + ", " + reg);
        }
    }
    else if(checkHeap(symbol)){
        isHeap = true;

        
        print("MOV " + reg + ", FP");
        print("ADI " + reg + ", #-8");
        print("LDR " + reg + ", " + reg);
        print("ADI " + reg + ", #" + std::to_string(symbol.offset));
    }
    else if(checkInstruction(symbol)){
        isInstruction = true;
    }
    return "default String";
}



void tCode::print(std::string output, bool newLine){
    if(newLine){
        std::cout << output << std::endl;
        file << output << std::endl;
    }
    else{
        std::cout << output;
        file << output;
    }
}

std::string tCode::getValue(std::string symId, bool use_old_frame){
    Symbol symbol = symbolTable.getSymbol(symId);
    bool global; //If the operand was defined as a global variable. Kxi doesn’t
    //support global variables, however a literal is handled like a global
    //variable.
    bool isStack; //If the operand is a passed-parameter of a function, a local variable
    //of a function or a temporary variable of a function
    bool isHeap; //If the operand is an instance variable or an array element. Arrays
    //and instances can not be created on the Stack in kxi.
    bool isInstruction; //If the operand is immediate (e.g., ADI A, #42).
    std::string reg = "R";
    reg.append(std::to_string(useReg));
    if(isGlobal(symbol)){
        global = true;
        if(symbol.getData("type") == "char"){
            print("LDB " + reg + ", " + symbol.symID);
        }
        else{
            print("LDR " + reg + ", " + symbol.symID);
        }
    }
    else if(checkStack(symbol)){
        isStack = true;
        if(use_old_frame){
            print("MOV " + reg + ", R2");
            print("ADI " + reg + " #" + std::to_string(symbolTable.getSymbol(curQuad.arg1).offset));
            print("LDR " + reg + ", " + reg);
        }
        else{
            print("MOV " + reg + ", FP");
            print("ADI " + reg + ", #" + std::to_string(symbol.offset));
            print("LDR " + reg + ", " + reg);
        }
        if(symbol.symID[0] == 'R'){
            print("LDR " + reg + ", " + reg);
        }
    }
    else if(checkHeap(symbol)){
        isHeap = true;
        
        
        print("MOV " + reg + ", FP");
        print("ADI " + reg + ", #-8");
        print("LDR " + reg + ", " + reg);
        
        
        //print("MOV " + reg + ", SL");
        print("ADI " + reg + ", #" + std::to_string(symbol.offset));
        print("LDR " + reg + ", " + reg);
    }
    else if(symbol.lexeme == "this"){
        if(use_old_frame){
            print("MOV " + reg + ", R2");
        }
        else{
            print("MOV " + reg + ", FP");
        }
        print("ADI " + reg + ", #-8");
        print("LDR " + reg + ", " + reg);
    }
    else if(checkInstruction(symbol)){
        isInstruction = true;
    }
    
    return "default String";
}
