//
//  netlist_builder.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "netlist_builder.h"
#include "flip_flop.h"
#include "and_gate.h"
#include <assert.h>
#include "gate_prototypes.h"

//bonus phase 2
#include "new_process_statements_functions.h"

void netlist_builder::add_net(std::string name)
{
    assert(nl_.nets_table_.find(name) == nl_.nets_table_.end());
    
    // append net and update nets_table
    net *n = factory_->make_net();
    n->name_ = name;
    nl_.nets_table_[name] = n;
    nl_.nets_.push_back(n);
}

size_t netlist_builder::add_gate(std::string type, std::string name)
{
    nl_.gates_.push_back(factory_->make_gate(type, name));
    return nl_.gates_.size()-1;
}





void netlist_builder::gate_append_pin(size_t gate_index, std::string bus_name, int lsb, int msb)
{
    assert(gate_index < nl_.gates_.size());
    
    // append pin to gates_[gate_index]
    pin *p = factory_->make_pin();
    nl_.gates_[gate_index]->pins_.push_back(p);
    p->gate_ = nl_.gates_[gate_index];
    p->pin_index_ = nl_.gates_[gate_index]->pins_.size() - 1;
    
    //and make connections to nets
        //the for loop makes the order in the nets in the pin is from lsb to msb
        // i.e. pin::nets_[0] connects to pin's lsb,pin[0]
    for (int bus_index = lsb; bus_index <= msb; bus_index++)
    {
        std::string bus_name_cor;
        if (msb != -1)
            bus_name_cor = make_net_name(bus_name, bus_index);
        else
            bus_name_cor = bus_name;
        net * n;
        evl_nets_table::const_iterator ni=nl_.nets_table_.find(bus_name_cor);
		if (ni == nl_.nets_table_.end())
		{
			assert(ni != nl_.nets_table_.end());
		}
        assert(ni != nl_.nets_table_.end());
        n = ni->second;
        p->nets_.push_back(n);
        n->append_pin(p);
    }
}

bool netlist_builder::finalize_creation() {
    //for each gate g in nl_.gates_
	std::vector<gate *> nl_gate_temp;
	std::vector<net *>	nl_net_temp;


    for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++)
    {
        if (!(*gi)->validate_structural_semantics())
        {
            return false;
        }
		//move all gates in user_definded gate  into nl_
		if (find_in_third_party(*gi))
		{
			netlist * nl_temp;
			(*gi)->get_nl(nl_temp);
			std::vector<gate *> gates_temp = nl_temp->get_gates();
			std::vector<net *> nets_temp = nl_temp->get_nets();

			nl_gate_temp.insert(nl_gate_temp.end(), gates_temp.begin(), gates_temp.end());
			nl_net_temp.insert(nl_net_temp.end(), nets_temp.begin(), nets_temp.end());

		}



    }

	nl_.gates_.insert(nl_.gates_.end(), nl_gate_temp.begin(), nl_gate_temp.end());
	nl_.nets_.insert(nl_.nets_.end(), nl_net_temp.begin(), nl_net_temp.end());

    return true;
}
