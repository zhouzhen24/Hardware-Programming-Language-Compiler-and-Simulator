//
//  gate_prototypes.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "gate_prototypes.h"
#include "gate_implementation.h"

//bonus
#include "module_from_gate.h"
#include "new_struct.h"
//new_netlists    notop_netlists;

gate_prototypes *gate_prototypes::instance_ = NULL;

gate_prototypes *gate_prototypes::instance() {
    if (instance_ == NULL)
        instance_ = new gate_prototypes;
    return instance_;
}

void    gate_prototypes::store(std::string name, gate * g)
{
    assert(prototypes_.find(name) == prototypes_.end());
    prototypes_[name] = g;
}

gate *  gate_prototypes::locate(std::string name)
{
    if (prototypes_.find(name) == prototypes_.end()) {
        return NULL;
    }
    return prototypes_[name];
}



void store_standard_gate_prototypes() {
    flip_flop::store_prototype();
    and_gate::store_prototype();
    not_gate::store_prototype();
    evl_output::store_prototype();
    evl_one::store_prototype();
    xor_gate::store_prototype();
    buf_gate::store_prototype();
    or_gate::store_prototype();
    evl_input::store_prototype();
    evl_zero::store_prototype();
    evl_clock::store_prototype();
    
    evl_lut::store_prototype();
    tris_gate::store_prototype();
    
    //bonus
	xnor_gate::store_prototype();
    
    
    //...
}

void store_third_party_prototypes(){
    
    //for every notop netlist
    for (new_netlists::iterator nsi = notop_netlists.begin(); nsi != notop_netlists.end(); nsi++) {
            gate_prototypes::instance()->store((*nsi).module.type, new module("prototype"));
    }
    
    ///Note!!!!
    assert(false);
    
    
    
    //    gate_prototypes::instance()->store("evl_lut", new evl_lut("prototype"));

    
}



