//
//  Flip_flop.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/13/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "flip_flop.h"


void flip_flop::compute_next_state() {
    net *input_net = pins_[1]->get_nets().back();
    next_state_ = input_net->retrieve_logic_value();
    state_ = next_state_;

}

bool flip_flop::validate_structural_semantics() {
    if (pins_.size() != 3)
        return false;
    
    pins_[0]->set_as_output();
    pins_[1]->set_as_input();
    pins_[2]->set_as_input();
    
    return true;
}

flip_flop * flip_flop::clone(std::string name)
{
    return new  flip_flop(name);
}

void flip_flop::store_prototype()
{
    gate_prototypes::instance()->store("evl_dff", new flip_flop("prototype"));
}



bool flip_flop::evaluate(const std::vector<bool> &inputs)
{
    return state_;
}
