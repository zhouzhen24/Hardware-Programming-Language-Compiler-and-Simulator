//
//  evl_clock.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/25/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "evl_clock.h"
#include "gate_prototypes.h"


bool evl_clock::validate_structural_semantics() {
	assert(pins_.size() == 1);

	pins_[0]->set_as_output();
    return true;
}

evl_clock * evl_clock::clone(std::string name)
{
    return new evl_clock(name);
}

void evl_clock::store_prototype()
{
    gate_prototypes::instance()->store("evl_clock", new evl_clock("prototype"));
}

bool evl_clock::evaluate(const std::vector<bool> &inputs)
{
    assert(inputs.size() == 0);
    return true;
}
