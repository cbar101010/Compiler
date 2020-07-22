//
//  sar.hpp
//  lexical
//
//  Created by Colby Barton on 2/20/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#ifndef sar_hpp
#define sar_hpp

#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "token.hpp"

class sar{
public:
    Token token;
    std::string symID;
    std::string type;
    std::string classs;
    std::string mem;
    std::string op;
    std::string returnType;
    std::string top;
    std::string bottom;
    std::vector<sar> argumentList;
    std::string methodName;
    std::string indexSymId;
    std::string arrayIndex;
    sar();
    ~sar();
    void print();
    bool Empty();
};
class idSar : public sar
{
public:
    idSar();
};
class vSar : public sar
{
public:
    vSar();
};
class newSar : public sar
{
public:
    newSar();
};

class funcSar : public sar
{
public:
    funcSar();
};
class arrSar : public sar
{
public:
    arrSar();
};

class refSar : public sar
{
public:
    refSar();
};

class lSar : public sar
{
public:
    lSar();
};

class balSar : public sar
{
public:
    balSar();
};

class alSar : public sar
{
public:
    alSar();
};

class typeSar : public sar
{
public:
    typeSar();
};

class tSar : public sar
{
public:

    tSar();
};
#endif /* sar_hpp */
