//
//  and_gate.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/13/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__and_gate__
#define __Logic_Simulation__and_gate__

#include "logic_gate.h"

class and_gate: public logic_gate
{
    bool state_, next_state_;
    and_gate * clone(std::string name);

public:
    and_gate(std::string name): logic_gate("and", name), state_(false), next_state_(false) {}
    bool    validate_structural_semantics();
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &inputs);
};


#endif /* defined(__Logic_Simulation__and_gate__) */
