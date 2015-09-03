//
//  factory_pattern.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "netlist_factory.h"
#include "gate_prototypes.h"

gate *netlist_factory::make_gate(std::string type, std::string name)
{
    gate *prototype = gate_prototypes::instance()->locate(type);
    if (prototype == NULL)//... handling errors
    {
        std::cerr << "Cannot find the type in Prototype_" << std::endl;
        assert(false);
        //return NULL; // handling errors
    }
    return prototype->clone(name);
}

net *netlist_factory::make_net()
{
    return new net();
}

pin *netlist_factory::make_pin()
{
    return new pin();
}