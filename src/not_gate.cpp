//
//  not_gate.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/18/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "not_gate.h"
#include "gate_prototypes.h"


bool not_gate::validate_structural_semantics() {
    
    if (pins_.size() != 2)
        return false;
    
    pins_[0]->set_as_output();
    pins_[1]->set_as_input();
    
    return true;
}

not_gate * not_gate::clone(std::string name)
{
    return new not_gate(name);
}

void not_gate::store_prototype()
{
    gate_prototypes::instance()->store("not", new not_gate("prototype"));
}

bool not_gate::evaluate(const std::vector<bool> &inputs)
{
    assert(inputs.size() == 1);
    return !inputs.front();
}
