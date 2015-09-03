//
//  composite_pattern.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "module_from_gate.h"
#include "assert.h"
#include "priority_scheduler.h"

module::module(std::string type, std::string name): gate(type, name), scheduler(nl_) {}
module::module(std::string type): gate(type,""), scheduler(nl_){}

bool module::validate_structural_semantics() {
    // make sure the width of each pin matches with each port
    assert(pins_.size() == ports_.size());
    for (int i = 0; i < pins_.size(); i++) 
	{
        if (pins_[i]->get_nets().size() != ports_[i]->get_width())
		{
            std::cerr << get_type() << " " << get_name() << " Error: pins don't match ports" << std::endl;
            return false;
        }
        else{    // set the type of each pin using the type of each port
            ports_[i]->assign_pin_outside(pins_[i]);
            ports_[i]->set_index(pins_[i]->get_index());
            
            //set pin_inside
            pin * pin_inside_temp = new pin(pins_[i]->get_index());

			//connect pin_inside into nets in module's netlist
			for (int neti = 0; neti < ports_[i]->get_width(); neti++)
			{
				net * n;
				evl_nets_table	nets_table_temp = nl_.get_nets_table();
				//wire or bus
				std::string	name_temp;
				if (ports_[i]->get_width() == 1)
				{
					name_temp = ports_[i]->get_name();
				}

				else
				{
					name_temp = make_net_name(ports_[i]->get_name(), neti);
				}

				evl_nets_table::const_iterator ni = nets_table_temp.find(name_temp);
				if (ni == nets_table_temp.end())
				{
					assert(ni != nets_table_temp.end());
				}
				assert(ni != nets_table_temp.end());
				n = ni->second;
				pin_inside_temp->nets_.push_back(n);// friend class only// not elegant
				n->append_pin(pin_inside_temp);
			}







            ports_[i]->assign_pin_inside(pin_inside_temp);

			//set pin's input  output
			if (ports_[i]->get_inout() == 0)
			{
				ports_[i]->get_pin_outside()->set_as_output();
				ports_[i]->get_pin_inside()->set_as_input();
			}
			else
			{
				ports_[i]->get_pin_outside()->set_as_input();
				ports_[i]->get_pin_inside()->set_as_output();
			}

            
        }
    }
    
	//convert every port to buf
	std::vector<buf_gate *> ports_bufs;
	for (int i = 0; i < ports_.size(); i++)
	{
		std::vector<buf_gate *> bufs_temp = ports_[i]->create_buf();
		ports_bufs.insert(ports_bufs.end(), bufs_temp.begin(), bufs_temp.end());
	}
	
	//push bufs to local module's gates
	nl_.gates_.insert(nl_.gates_.end(), ports_bufs.begin(), ports_bufs.end());

	//push all nets, gates to top module

    return true;
}


module::~module()
{
    for ( ; !ports_.empty(); ) {
        port * port_temp = ports_.back();
        ports_.pop_back();
        delete port_temp;
    }//delete all ports
}




void    module::create_ports(evl_ports ports){
    //for every evl_port
    for (evl_ports::iterator pi = ports.begin(); pi != ports.end(); pi++) {
        port *p = new port((*pi).name, (*pi).width);
        
        if ((*pi).type.compare("output") == 0) {
            p->set_as_output();
        }
        else{
            p->set_as_input();
        }
        
        ports_.push_back(p);
    }
}




void    module::retrieve(std::vector<bool> &inputs){
    
    for (std::vector<port *>::iterator pi = ports_.begin(); pi != ports_.end(); pi++) {
        if ((*pi)->get_inout() == 0) {
            continue;
        }
        std::list<net *> net_temp = (*pi)->get_pin_outside()->get_nets();
        //for each net in pin
        for (std::list<net *>::iterator ni = net_temp.begin(); ni != net_temp.end(); ni++) {
            bool input = (*ni)->retrieve_logic_value();
            inputs.push_back(input);
        }
    }
}


bool    module::compute_output(std::vector<bool> &v){
    
    //retrieve values from outside
    std::vector<bool>   inputs;
    retrieve(inputs);
    
    //assign to nets
    std::vector<net *> input_nets;
    //for every port
    for (int i = 0; i < ports_.size(); i++) {
        if (ports_[i]->get_inout() == 1) {
            std::list<net *> input_nets_temp = ports_[i]->get_pin_inside()->get_nets();
            for (std::list<net *>::iterator ni = input_nets_temp.begin(); ni != input_nets_temp.end(); ni++) {
                input_nets.push_back((*ni));
            }
        }
    }
    

    
    
    
    //map_priority_scheduler  scheduler(nl_);
    
    scheduler.hier_run(input_nets, inputs);
    
    
    //save output into v
        //from v.front() to v.back(), first pin lsb to msb, second pin lsb to msb ......
    
    //for every port
    for (int i = 0; i < ports_.size(); i++) {
        if (ports_[i]->get_inout() == 0) {
            std::list<net *> input_nets_temp = ports_[i]->get_pin_inside()->get_nets();
            for (std::list<net *>::iterator ni = input_nets_temp.begin(); ni != input_nets_temp.end(); ni++) {
                v.push_back((*ni)->get_value());
            }
        }
    }

    
    
    return true;
}
