//
//  evl_zero.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/23/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__evl_zero__
#define __Logic_Simulation__evl_zero__

#include "logic_gate.h"

class evl_zero: public logic_gate
{
    bool state_, next_state_;
    evl_zero * clone(std::string name);
    
public:
    evl_zero(std::string name): logic_gate("evl_zero", name), state_(false), next_state_(false) {}
    bool    validate_structural_semantics();
    //   static void store_prototype(gate_prototypes &gps);
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &inputs);
    
    //Might Suprise!!!
    //bool    evaluate(const std::vector<bool> &inputs);
};

#endif /* defined(__Logic_Simulation__evl_zero__) */
