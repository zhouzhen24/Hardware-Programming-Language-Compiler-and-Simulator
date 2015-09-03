//
//  netlist_builder.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__netlist_builder__
#define __Logic_Simulation__netlist_builder__


#include <string>
#include "gate_prototypes.h"
#include "evl_data_netlist.h"
#include "netlist_factory.h"

struct netlist_builder_i
{
    virtual void add_net(std::string name) = 0;
    
    // return the ID# of the gate
    virtual size_t add_gate(std::string type, std::string name) = 0;
    
    // create a pin and append it to a gate
    virtual void gate_append_pin(size_t gate_id, std::string bus_name, int lsb, int msb) = 0;
    
}; // struct netlist_builder_i


class netlist_builder : public netlist_builder_i
{
    netlist_builder(const netlist_builder &);            // no copy
    netlist_builder &operator=(const netlist_builder &); // no assignment
    
    netlist &nl_; // the netlist to be built
    
    netlist_factory_i *factory_;

public:

    void    add_net(std::string name);
    size_t  add_gate(std::string type, std::string name);
    void    gate_append_pin(size_t gate_id, std::string bus_name, int lsb, int msb);
    
    netlist_builder(netlist &nl, netlist_factory_i *factory): nl_(nl), factory_(factory) {}//4th version
    bool finalize_creation();
    
}; // class netlist_builder




#endif /* defined(__Logic_Simulation__netlist_builder__) */
