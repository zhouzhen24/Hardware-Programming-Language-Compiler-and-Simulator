//
//  evl_output.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/18/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__evl_output__
#define __Logic_Simulation__evl_output__

#include "logic_gate.h"

class evl_output: public logic_gate
{
    bool state_, next_state_;
    evl_output * clone(std::string name);
    
    bool    first_flag;

public:
    evl_output(std::string name): logic_gate("evl_output", name), state_(false), next_state_(false), first_flag(true) {}
    bool    validate_structural_semantics();
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &inputs);
    
};

#endif /* defined(__Logic_Simulation__evl_output__) */
