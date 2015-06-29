//
//  xor_gate.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/22/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "xor_gate.h"
#include "gate_prototypes.h"
#include "assert.h"

bool xor_gate::validate_structural_semantics() {
    
    if (pins_.size() < 3)
        return false;
    
    pins_[0]->set_as_output();
    for (size_t i = 1; i < pins_.size(); ++i)
        pins_[i]->set_as_input();
    
    
    return true;
}

xor_gate * xor_gate::clone(std::string name)
{
    return new xor_gate(name);
}

void xor_gate::store_prototype()
{
    gate_prototypes::instance()->store("xor", new xor_gate("prototype"));
}
//void xor_gate::store_prototype(gate_prototypes &gps)
//{
//    assert(gps.find("and") == gps.end());
//    gps["and"] = new xor_gate("prototype");
//}

bool xor_gate::evaluate(const std::vector<bool> &inputs)
{
    assert(inputs.size() >= 2);
    
    int count1 = 0;
    
    for (std::vector<bool>::const_iterator input_i = inputs.begin(); input_i != inputs.end(); input_i++) {
        if ((*input_i) == 1) {
            count1++;
        }
    }
    if (count1%2)//if it is odd
        return true;
    else
        return false;
    
    
}

