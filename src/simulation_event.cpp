//
//  simulation_events.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/21/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "simulation_event.h"
#include "new_process_statements_functions.h"

void    simulation_event::set_priority(int pri)
{
    priority_ = pri;
}

int simulation_event::get_priority()
{
    return priority_;
}


gate * simulation_event::get_gate_pointer()
{
    return gate_;
}

net * simulation_event::get_net_pointer()
{
    return net_;
}

bool    simulation_event::get_bwgate()
{
    return bwgate_;
}

bool simulation_event::compare(simulation_event *event)
{
    if (bwgate_ != event->get_bwgate()) {
        return true;
    }
    else if (bwgate_ == false)// if net
    {
        if (net_ == event->get_net_pointer()) {
            return false;
        }
        else
            return true;
    }
    else
    {
        if (gate_ == event->get_gate_pointer()) {
            return false;
        }
        else
            return true;
    }
}


std::list<simulation_event *>::iterator  simulation_event::find(std::list<simulation_event *> &all_events)
{
    assert(!all_events.empty());
    for (std::list<simulation_event *>::iterator ei = all_events.begin(); ei != all_events.end() ; ei++) {
        if (compare(*ei) == 0) {
            //(*ei)->set_priority(get_priority());//NOTE !!! set priority to all events
            return ei;
        }
    }
    return all_events.end();
    
}

bool    simulation_event::find_next_events(std::list<simulation_event *> &next_events)
{
    next_events.clear();
    if (!bwgate_) {//if it is wire
        net * net_temp = get_net_pointer();
        std::list<pin *> pins_temp = net_temp->get_connections();
        for (std::list<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++) {
            //for every pin connected to net
            if ((*pi)->get_inout() == 1) {
                simulation_event se_temp((*pi)->get_gate(), get_priority()+1);
                std::list<simulation_event *>::iterator sei = se_temp.find(all_events);
                if ((*sei)->check_prev_events()) {
                    next_events.push_back(*sei);
                }
            }
            
        }
    }
    else{// if it is gate
        gate * gate_temp = get_gate_pointer();
        
        if (gate_temp->get_type().compare("evl_dff") == 0 || gate_temp->get_type().compare("evl_output") == 0) {//flip_flop and evl_output should has no next event
            return false;
        }
        
        std::vector<pin *> pins_temp = gate_temp->get_pins();
        for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++){
            // for every pin
            if ((*pi)->get_inout() == 0) {
                std::list<net *> nets_temp = (*pi)->get_nets();
                for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++) {
                    //for every net
                    simulation_event se_temp((*ni), get_priority()+1);
                    std::list<simulation_event *>::iterator sei = se_temp.find(all_events);
                    next_events.push_back(*sei);
                }
            }
        }   
    }
    if (next_events.size() == 0) {
        return false;
    }
    else
        return true;
    
}


bool simulation_event::check_prev_events()
{
    //find all inputs
    if (bwgate_) {//if it is gate
        std::list<simulation_event *> prev_events;
        
        std::vector<pin *> pins_temp = gate_->get_pins();
        for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++) {
            // for every pin
            if ((*pi)->get_inout() == 1) {// find input pin
                std::list<net *> nets_temp = (*pi)->get_nets();
                for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++) {
                    //for every net
                    simulation_event se_temp((*ni), get_priority()-1);
                    std::list<simulation_event *>::iterator sei = se_temp.find(all_events);
                    if (!(*sei)->get_schedule()) {
                        return false;
                    }
                }
                
                
            }
        }
        //if all inputs are scheduled
        return true;
    }
    else{// if it is net
        net * net_temp = get_net_pointer();
        std::list<pin *> pins_temp = net_temp->get_connections();
        for (std::list<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++) {
            //for every pin connected to net
            if ((*pi)->get_inout() == 0) {
                simulation_event se_temp((*pi)->get_gate(), get_priority()-1);
                std::list<simulation_event *>::iterator sei = se_temp.find(all_events);
                if (!(*sei)->get_schedule()) {
                    return false;
                }
            }
        }
        return true; // if all gates are scheduled
    }
}


void    simulation_event::insert_next_level_event(priority_scheduler_i *scheduler)
{
    std::list<simulation_event *> next_events;
    if (find_next_events(next_events)) {
        for (std::list<simulation_event *>::iterator ei = next_events.begin(); ei != next_events.end(); ei++) {
            //for every event in next_event
            if ((*ei)->get_bwgate()){// if the next event is gate
                simulation_event se_temp((*ei)->get_gate_pointer(), get_priority()+1);
                std::list<simulation_event *>::iterator sei = se_temp.find(all_events);
                if ((*sei)->check_prev_events()){   //if not all wire scheduled, skip
                    (*sei)->set_priority(get_priority()+1);
                    scheduler->insert_event(*sei);
                }
            }
            else{
                simulation_event se_temp((*ei)->get_net_pointer(), get_priority()+1);
                std::list<simulation_event *>::iterator sei = se_temp.find(all_events);
                if ((*sei)->check_prev_events()){   //if not all gate shceduled, skip (but not possible)
                    (*sei)->set_priority(get_priority()+1);
                    scheduler->insert_event(*sei);
                }
            }
        }
    }
}



void    simulation_event::fire(priority_scheduler_i *scheduler)
{
    if (bwgate_)
    {
        assert(net_ == NULL);
        //NOTE: update flip_flop here might cause problem
        if (gate_->get_type().compare("evl_dff") == 0) {
        }
        else if(gate_->get_type().compare("evl_lut") == 0)
        {
                //read the lut file
                if(!gate_->compute_output())//if address is illegal
                    assert(false);
                
                //assign values to pins

                std::list<net *> nets_temp = gate_->get_pins().front()->get_nets();
                int word_index = 0;
                for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++) {
                    (*ni)->set_value(gate_->evl_lut_get_pins_value()[word_index]);
                    (*ni)->set_comf();
                    word_index++;
                }
            
        
        }//evl_lut
        
        else if (gate_->get_type().compare("buf") == 0 ||
                 gate_->get_type().compare("tris") == 0 )//note tris!!!
        {
            std::vector<bool>   value_temp;
            gate_->compute_output(value_temp);
            assert(value_temp.size() == 2);
            net * net_temp = gate_->get_pins().front()->get_nets().front();//1-bit output of buf gate
            
            net_temp->set_value(value_temp[1], value_temp[0]);
            net_temp->set_comf();
            
            
        }//buf&tris
        
        else if(find_in_third_party(gate_)){ //find in third party
			return;
        }
        
        
        else{
            bool value_temp = gate_->compute_output();
            //assign value to next nets
            std::vector<pin *> pin_temp = gate_->get_pins();
            for (std::vector<pin *>::iterator pi = pin_temp.begin(); pi != pin_temp.end(); pi++) {
                if ((*pi)->get_inout() == 0) {//if output of the gate found
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //for every net in the pin
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++) {
                        (*ni)->set_comf();
                        (*ni)->set_value(value_temp);
                    }
                }
            }
        }
    }
    else//fire net event
    {
        assert(gate_ == NULL);

		pin * drive;//check drive
		if (!net_->find_drive(drive))//if no drive
		{
			net_->set_value(1, 1);
			net_->set_comf();
		}
		else//if one drive
		{
			gate * gate_temp = drive->get_gate();
			//if buf or tris
            if (gate_temp != NULL){//bonus
                if (gate_temp->get_type().compare("buf") == 0 || gate_temp->get_type().compare("tris") == 0)
                {
                    std::vector<bool> value_temp;
                    gate_temp->compute_output(value_temp);
                    net_->set_value(value_temp[1], value_temp[0]);
                }// if buf or tris
            }
		}
    }
    
}



void	simulation_event::optimal_fire(priority_scheduler_i	*scheduler)
{
	if (bwgate_)// if it is gate event
	{
		if (gate_->get_type().compare("evl_dff") != 0 &&
            gate_->get_type().compare("evl_output") != 0 &&
            gate_->get_type().compare("buf") != 0 &&
            gate_->get_type().compare("tris") != 0 &&
            gate_->get_type().compare("evl_lut") != 0)// if not flip-flop or evl_output
		{
            //bonus
            if(find_in_third_party(gate_))
			{ //find in third party
				return;
            }
            else{
                bool	value_temp = gate_->compute_output();
                net * output_net = gate_->get_pins().front()->get_nets().front();//note combinational gates have only one output
                output_net->update_value(value_temp);
                if (output_net->get_bwchange())//if change
                {
                    scheduler->insert_event(new simulation_event(output_net, output_net->get_priority()));//or output_net->get_priority()get_priority_optimal()+1
                }//may insert multiple time for one net
            }
		}
        else if (gate_->get_type().compare("evl_lut") == 0)
        {
            //read the lut file
            if(!gate_->compute_output())//if address is illegal
                assert(false);
            
            //assign values to pins
            std::list<net *> nets_temp = gate_->get_pins().front()->get_nets();
            int word_index = 0;
            for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++) {
                (*ni)->update_value(gate_->evl_lut_get_pins_value()[word_index]);
                word_index++;
                if ((*ni)->get_bwchange()) {
                    scheduler->insert_event(new simulation_event(*ni, (*ni)->get_priority()));
                }
            }
        }//evl_lut
        
        else if (gate_->get_type().compare("buf") == 0 ||
                 gate_->get_type().compare("tris") == 0 )//note tris!!!
        {
            std::vector<bool>   value_temp;
            gate_->compute_output(value_temp);
            assert(value_temp.size() == 2);
            net * net_temp = gate_->get_pins().front()->get_nets().front();//1-bit output of buf gate
            
            net_temp->update_value(value_temp[1], value_temp[0]);
            if(net_temp->get_bwchange())
            {
                scheduler->insert_event(new simulation_event(net_temp, net_temp->get_priority()));
            }
            
            
        }//buf&tris
        
        
        else if(find_in_third_party(gate_)){ //find in third party
			return;
        }


        
        
        
	}
	else// if net event
	{

		pin * drive;//check drive
		if (!net_->find_drive(drive))//if no drive
		{
			net_->set_value(1, 1);
		}
		else//if one drive
		{

			gate * gate_temp = drive->get_gate();
			//if buf or tris
            if ( gate_temp != NULL){//bonus
                if (gate_temp->get_type().compare("buf") == 0 || gate_temp->get_type().compare("tris") == 0)
                {
                    std::vector<bool> value_temp;
                    gate_temp->compute_output(value_temp);
                    net_->set_value(value_temp[1], value_temp[0]);
                    
                    
                }
            }
		}

        //find all outputs of net, that is input of gate
        std::list<pin *> pins_temp = net_->get_connections();
        //every pin in the net
        for (std::list<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
        {
            if ((*pi)->get_inout() == 1)
            {
                gate * gate_temp = (*pi)->get_gate();
                if (gate_temp != NULL){//bonus
                    if (gate_temp->get_type().compare("evl_dff") != 0 && gate_temp->get_type().compare("evl_output") != 0)// if not flip-flop or evl_output
                        scheduler->insert_event(new simulation_event(gate_temp, gate_temp->get_priority()));
                }
            }
        }
	}
}