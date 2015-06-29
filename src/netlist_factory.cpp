//
//  factory_pattern.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "netlist_factory.h"
#include "gate_prototypes.h"
//#include "module_from_gate.h"

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


//***************FOR MODULE****************//
//gate * hier_factory::make_gate(std::string type, std::string name)
//{
//    gate *prototype = gate_prototypes::instance()->locate(type);
//    
//    if (prototype != NULL)
//        return prototype->clone(name);
//    
//    //find ports, wires, and comps for the module type
//    
//    // create a module with type ’type’ and name ’name’
//    module *p = new module(type, name);
//    
//    p->create_ports(ports);
//    
//    // create the internal netlist using builder and factory
//    netlist_builder builder(p->nl_, this);
//    evl_director(&builder, wires, comps);
//    builder.finalize_creation();
//    return p;
//}