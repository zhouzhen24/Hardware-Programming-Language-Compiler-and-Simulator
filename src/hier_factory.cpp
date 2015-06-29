//
//  hier_factory.cpp
//  Project_Bonus_ECE449
//
//  Created by Zhen Zhou on 12/2/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "hier_factory.h"
#include "new_struct.h"
#include "gate_prototypes.h"
#include "module_from_gate.h"
#include "netlist_builder.h"
#include "evl_director.h"
#include "new_process_statements_functions.h"

//new_netlists    notop_netlists;



gate *hier_factory::make_gate(std::string type, std::string name) {
    
    gate *prototype = gate_prototypes::instance()->locate(type);
    
    if (prototype != NULL)
        return prototype->clone(name);
    
    //find ports, wires, and comps for the module type
    evl_module  module_local;
    evl_ports   ports;
    evl_wires   wires;
    evl_components  comps;
    
    if(!find_elements(notop_netlists, type, module_local, ports, wires, comps)){
        std::cerr << "Error when make gate '" << type << "'" << " " << name << std::endl;
        assert(false);
    }
    
    // create a module with type ’type’ and name ’name’
    module *p = new module(type, name);
    p->create_ports(ports);
    // create the internal netlist using builder and factory
    netlist_builder builder(p->nl_, this);
    evl_director(&builder, wires, comps);
    builder.finalize_creation();
    //p->connect_ports_to_nets();// not in ppt
    return p;
    
}

net *hier_factory::make_net(){
    return new net();
}

pin *hier_factory::make_pin(){
    return new pin();
}





