//
//  evl_clock.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/25/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__evl_clock__
#define __Logic_Simulation__evl_clock__

#include "logic_gate.h"

class evl_clock: public logic_gate
{
    bool state_, next_state_;
    evl_clock * clone(std::string name);
    
public:
    evl_clock(std::string name): logic_gate("evl_clock", name), state_(false), next_state_(false) {}
    bool    validate_structural_semantics();
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &inputs);
};

#endif /* defined(__Logic_Simulation__evl_clock__) */
