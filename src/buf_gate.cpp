//
//  buf_gate.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/23/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "buf_gate.h"
#include "gate_prototypes.h"


bool buf_gate::validate_structural_semantics() {
    
    if (pins_.size() != 2)
        return false;
    
    pins_[0]->set_as_output();
    pins_[1]->set_as_input();

    
    return true;
}

buf_gate * buf_gate::clone(std::string name)
{
    return new buf_gate(name);
}

void buf_gate::store_prototype()
{
    gate_prototypes::instance()->store("buf", new buf_gate("prototype"));
}


bool buf_gate::evaluate(const std::vector<bool> &inputs)
{
    //assert(inputs.size() == 1);
    return inputs.front();
}



//for tris
//void buf_gate::retrieve(std::vector<bool> &inputs)
//{
//    //in buf, only pins_[1] is input , pins_[0] is output, with exact 1-bit
//
//    (pins_[1]->get_nets().front())->get_values(inputs);
//    
//    
//    
//    
//}


bool buf_gate::compute_output(std::vector<bool> &v)
{
    std::vector<bool> inputs;
    //std::vector<bool> outputs;
    //retrieve(inputs);
    (pins_[1]->get_nets().front())->get_values(inputs);
	assert(inputs.size() == 2);
    v.clear();
    v.push_back(inputs[0]);
    v.push_back(inputs[1]);
    
    //v = evaluate(inputs);
    
    return true;
    
}




