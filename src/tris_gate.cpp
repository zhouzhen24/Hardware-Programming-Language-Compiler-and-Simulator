//
//  tris_gate.cpp
//  Logic_Simulation_Phase2
//
//  Created by Zhen Zhou on 11/28/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "tris_gate.h"
#include "gate_prototypes.h"


bool tris_gate::validate_structural_semantics() {
    
    if (pins_.size() != 3)
        return false;
    
    pins_[0]->set_as_output();
    pins_[1]->set_as_input();
    pins_[2]->set_as_input();
    
    return true;
}

tris_gate * tris_gate::clone(std::string name)
{
    return new tris_gate(name);
}

void tris_gate::store_prototype()
{
    gate_prototypes::instance()->store("tris", new tris_gate("prototype"));
}
//void tris_gate::store_prototype(gate_prototypes &gps)
//{
//    assert(gps.find("tris") == gps.end());
//    gps["tris"] = new tris_gate("prototype");
//}

bool tris_gate::evaluate(const std::vector<bool> &inputs)
{
    //assert(inputs.size() == 1);
    return inputs.front();
}



//for tris
//void tris_gate::retrieve(std::vector<bool> &inputs)
//{
//    //in tris, only pins_[1] is input , pins_[0] is output, with exact 1-bit
//
//    (pins_[1]->get_nets().front())->get_values(inputs);
//
//
//
//
//}


bool tris_gate::compute_output(std::vector<bool> &v)
{
    //tris name(out, in, en)
    std::vector<bool> in;
    std::vector<bool> en;
    //retrieve(inputs);
    (pins_[1]->get_nets().front())->get_values(in);//get in
    (pins_[2]->get_nets().front())->get_values(en);//get en
    v.clear();
        //check en
    if ((en[1] == 0)&&(en[0] == 1)) {//if en is 1
        v.push_back(in[0]);
        v.push_back(in[1]);
    }
    else{
        v.push_back(1);
        v.push_back(1);
    }
    //v = evaluate(inputs);
    
    return true;
    
}


