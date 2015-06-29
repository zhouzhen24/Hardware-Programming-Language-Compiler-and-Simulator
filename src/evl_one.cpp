//
//  evl_one.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/19/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "evl_one.h"
#include "gate_prototypes.h"


bool evl_one::validate_structural_semantics() {
    
    for (size_t i = 0; i < pins_.size(); i++)
    {
        pins_[i]->set_as_output();
    }
    
    return true;
}

evl_one * evl_one::clone(std::string name)
{
    return new evl_one(name);
}

void evl_one::store_prototype()
{
    gate_prototypes::instance()->store("evl_one", new evl_one("prototype"));
}

bool evl_one::evaluate(const std::vector<bool> &inputs)
{
    assert(inputs.size() == 0);
    return true;
}
