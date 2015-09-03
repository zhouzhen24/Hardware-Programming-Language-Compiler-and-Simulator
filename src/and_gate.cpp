//
//  and_gate.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/13/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "and_gate.h"
#include "gate_prototypes.h"


bool and_gate::validate_structural_semantics() {
    
    if (pins_.size() < 3)
        return false;
    
    pins_[0]->set_as_output();
    for (size_t i = 1; i < pins_.size(); ++i)
        pins_[i]->set_as_input();
    
    return true;
}

and_gate * and_gate::clone(std::string name)
{
    return new and_gate(name);
}

void and_gate::store_prototype()
{
    gate_prototypes::instance()->store("and", new and_gate("prototype"));
}


bool and_gate::evaluate(const std::vector<bool> &inputs)
{
    for (size_t i = 0; i < inputs.size(); ++i)
    {
        if (!inputs[i])
        {
            return false;
        }
    }
    return true;
}
