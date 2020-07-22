//
//  global.h
//  lexical
//
//  Created by Colby Barton on 3/11/20.
//  Copyright © 2020 Colby Barton. All rights reserved.
//

#ifndef global_h
#define global_h

struct quadStruct{
    std::string label = "";
    std::string op = "";
    std::string arg1 = "";
    std::string arg2 = "";
    std::string arg3 = "";
    std::string lineStatement = "";
    std::string output = "";
    
    std::string printStruct(){
        if(label != ""){
            output.append(label);
        }
        else{
            output.append("  ");
        }
        output.append(" | ");
        if(op != ""){
            output.append(op);
        }
        output.append(" | ");
        if(arg1 != ""){
            output.append(arg1);
        }
        output.append(" | ");
        if(arg2 != ""){
            output.append(arg2);
        }
        output.append(" | ");
        if(arg3 != ""){
            output.append(arg3);
        }
        output.append(" | ");
        output.append(lineStatement);
        output.append("\n \n");
        std::cout << output;
        return output;
    }
};
#endif /* global_h */
