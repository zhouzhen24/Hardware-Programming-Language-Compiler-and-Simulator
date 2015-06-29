//
//  new_struct.cpp
//  Project_Bonus_ECE449
//
//  Created by Zhen Zhou on 12/1/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "new_struct.h"
#include <assert.h>

void    port::set_as_input(){
    inout_ = 1;
    //pin_outside_->set_as_input();
    //pin_inside_->set_as_output();
}

void    port::set_as_output(){
    inout_ = 0;
    //pin_outside_->set_as_output();
    //pin_inside_->set_as_input();
}


std::string port::get_name(){
    return name_;
}

int     port::get_width(){
    return width_;
}

bool    port::get_inout(){
    return inout_;
}

pin * port::get_pin_outside(){
    return pin_outside_;
}

pin * port::get_pin_inside(){
    return pin_inside_;
}

void  port::set_index(size_t index){
    index_ = index;
}

void    port::add_net_to_port(net * net_input){
    pin_inside_->nets_.push_back(net_input);
}

void    port::assign_pin_outside(pin * pin){
    pin_outside_ = pin;
    index_ = pin->get_index();
}

void    port::assign_pin_inside(pin * pin){
    pin_inside_ = pin;
}


evl_components new_netlist::get_assign_as_buf(){
    evl_components  comps_temp;
    
    for (evl_assigns::iterator ai = all_assigns.begin(); ai != all_assigns.end(); ai++) {
        evl_component   comp_temp;
        comp_temp.type = "assign";
        comp_temp.pins.push_back((*ai).lhs);
        comp_temp.pins.push_back((*ai).rhs);
        comps_temp.push_back(comp_temp);
    }
    return comps_temp;
}

evl_wires   new_netlist::get_ports_as_wires(){
    evl_wires wires_temp;
    
    for (evl_ports::iterator pi = module.ports.begin(); pi != module.ports.end(); pi++) {
        evl_wire    wire_temp;
        wire_temp.name = (*pi).name;
        wire_temp.width = (*pi).width;
        wires_temp.push_back(wire_temp);
    }
    return wires_temp;
    
}


std::vector<buf_gate *> port::create_buf()
{
	assert(pin_inside_->get_nets().size() == pin_outside_->get_nets().size());

	std::vector<buf_gate *> bufs;
	if (inout_ == 0)//if output port
	{
		
		if (pin_inside_->get_nets().size() ==1)
		{
			buf_gate * p = new buf_gate("port");
			pin_inside_->gate_ = p;
			pin_outside_->gate_ = p;

			p->pins_.push_back(pin_outside_);
			p->pins_.push_back(pin_inside_);
			p->validate_structural_semantics();
			bufs.push_back(p);
			return bufs;
		}
		else
		{
			int size = pin_inside_->get_nets().size();
			std::list<net *> nets_inside = pin_inside_->get_nets();
			std::list<net *> nets_outside = pin_outside_->get_nets();

			std::list<net *>::iterator	outi = nets_outside.begin();
			std::list<net *>::iterator	ini = nets_inside.begin();
			for (; outi != nets_outside.end(); outi++)
			{
				buf_gate * p = new buf_gate("port");
				pin * pin_out = new pin(0);
				pin * pin_in = new pin(1);

				pin_out->gate_ = p;
				pin_in->gate_ = p;

				pin_out->nets_.push_back(*outi);
				pin_in->nets_.push_back(*ini);



				p->pins_.push_back(pin_out);
				p->pins_.push_back(pin_in);
				p->validate_structural_semantics();
				bufs.push_back(p);


				//delete pin_inside_ and pin_outside_ in nets connections
				
				
				for (std::list<pin *>::iterator pin_net_out = (*outi)->connections_.begin(); pin_net_out != (*outi)->connections_.end(); pin_net_out++)
				{
					if ((*pin_net_out) == pin_outside_)
					{
						//(*outi)->connections_.erase(pin_net_out);

						(*pin_net_out) = pin_out;
						break;
					}
				}

				for (std::list<pin *>::iterator pin_net_in = (*ini)->connections_.begin(); pin_net_in != (*ini)->connections_.end(); pin_net_in++)
				{
					if ((*pin_net_in) == pin_inside_)
					{
						//(*ini)->connections_.erase(pin_net_in);
						(*pin_net_in) = pin_in;
						break;
					}
				}


				ini++;
			}
		
			return bufs;


		}
	}
	else
	{
		if (pin_inside_->get_nets().size() == 1)
		{
			buf_gate * p = new buf_gate("port");
			pin_inside_->gate_ = p;
			pin_outside_->gate_ = p;


			p->pins_.push_back(pin_inside_);
			p->pins_.push_back(pin_outside_);
			p->validate_structural_semantics();
			bufs.push_back(p);
			return bufs;
		}
		else
		{
			int size = pin_inside_->get_nets().size();
			std::list<net *> nets_inside = pin_inside_->get_nets();
			std::list<net *> nets_outside = pin_outside_->get_nets();

			std::list<net *>::iterator	outi = nets_outside.begin();
			std::list<net *>::iterator	ini = nets_inside.begin();
			for (; outi != nets_outside.end(); outi++)
			{
				buf_gate * p = new buf_gate("port");
				pin * pin_out = new pin(1);
				pin * pin_in = new pin(0);

				pin_out->gate_ = p;
				pin_in->gate_ = p;

				pin_out->nets_.push_back(*outi);
				pin_in->nets_.push_back(*ini);



				p->pins_.push_back(pin_in);
				p->pins_.push_back(pin_out);
				p->validate_structural_semantics();
				bufs.push_back(p);

				//delete pin_inside_ and pin_outside_ in nets connections


				for (std::list<pin *>::iterator pin_net_out = (*outi)->connections_.begin(); pin_net_out != (*outi)->connections_.end(); pin_net_out++)
				{
					if ((*pin_net_out) == pin_outside_)
					{
						//(*outi)->connections_.erase(pin_net_out);
						(*pin_net_out) = pin_out;
						break;
					}
				}

				for (std::list<pin *>::iterator pin_net_in = (*ini)->connections_.begin(); pin_net_in != (*ini)->connections_.end(); pin_net_in++)
				{
					if ((*pin_net_in) == pin_inside_)
					{
						//(*ini)->connections_.erase(pin_net_in);
						(*pin_net_in) = pin_in;
						break;
					}
				}


				ini++;
			}

			return bufs;


		}
	}

}










//global variable
new_netlists    notop_netlists;


