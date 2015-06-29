//
//  evl_input.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/24/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__evl_input__
#define __Logic_Simulation__evl_input__

#include "logic_gate.h"

class evl_input: public logic_gate
{
    std::vector<bool> state_, next_state_;
    evl_input * clone(std::string name);
    
    int lineno_ ;
    int tranno_ ;
    std::vector<int> pins_width;
    std::vector<bool> pins_value;
    
    
    bool    first_flag;
    
    
public:
    evl_input(std::string name): logic_gate("evl_input", name), state_(false), next_state_(false), lineno_(1), tranno_(0), first_flag(true) {}
    bool    validate_structural_semantics();
    //   static void store_prototype(gate_prototypes &gps);
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &inputs);
    std::vector<bool> evl_input_get_pins_value();
    
    
    //Might Suprise!!!
    //bool    evaluate(const std::vector<bool> &inputs);
    
    
    
    
};

#endif /* defined(__Logic_Simulation__evl_input__) */
