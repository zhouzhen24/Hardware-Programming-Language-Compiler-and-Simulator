//
//  evl_zero.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/23/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "evl_zero.h"
#include "gate_prototypes.h"


bool evl_zero::validate_structural_semantics() {
    
    for (size_t i = 0; i < pins_.size(); i++)
    {
        pins_[i]->set_as_output();
    }
    
    return true;
}

evl_zero * evl_zero::clone(std::string name)
{
    return new evl_zero(name);
}

void evl_zero::store_prototype()
{
    gate_prototypes::instance()->store("evl_zero", new evl_zero("prototype"));
}

bool evl_zero::evaluate(const std::vector<bool> &inputs)
{
    assert(inputs.size() == 0);
    return false;
}
