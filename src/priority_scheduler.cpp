//
//  priority_scheduler.cpp
//  Cpractice
//
//  Created by Zhen Zhou on 11/19/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "priority_scheduler.h"
#include "simulation_event.h"//to see all_events

//bonus
#include "new_process_statements_functions.h"

std::list<simulation_event *> all_events;

void priority_scheduler_i::fire_event(priority_event_i *event)
{
    event->scheduled_ = false;
    event->fire(this);
}

void priority_scheduler_i::insert_event(priority_event_i *event)
{
    if (!event->scheduled_)
    {
        event->scheduled_ = true;
        insert(event, event->get_priority());
    }
}


//map_priority_scheduler

void map_priority_scheduler::insert(priority_event_i *event, int priority) {
    // an empty list will be created first if the key doesn’t exist
    events_[priority].push_back(event);
}


void map_priority_scheduler::run() {
    
    //debug
    //int	count = 0;
    
    for (; !events_.empty();) {
        if (events_.begin()->second.empty()) {
            events_.erase(events_.begin());
            //count++;
            continue;
        }
        //if (count == 49)
        //{
        //	std::cout << "Caution!!!" << std::endl;
        //	count++;//for pause once
        //}
        priority_event_i *event = events_.begin()->second.front();
        events_.begin()->second.pop_front();
        
        fire_event(event);
    }
}


void map_priority_scheduler::finalize_priority()
{
    //initiate all events
    
    if (all_events.empty()){
        for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++) {
            all_events.push_back(new simulation_event(*gi, 0));
        }
        for (std::vector<net *>::iterator ni = nl_.nets_.begin(); ni != nl_.nets_.end(); ni++)
        {
            (*ni)->set_uncomf();
            all_events.push_back(new simulation_event(*ni, 0));
        }
    }
    else{
        for (std::vector<net *>::iterator ni = nl_.nets_.begin(); ni != nl_.nets_.end(); ni++)
        {
            (*ni)->set_uncomf();
            //all_events.push_back(new simulation_event(*ni,0));
        }
        
    }
    
    
    //set all nets "?"//intitiate the computation(find start nets)
    //    for (std::vector<net *>::iterator ni = nets_.begin(); ni != nets_.end(); ni++) {
    //        (*ni)->set_uncomf();//set net as "?"
    //        std::list<pin *> pins_temp = (*ni)->get_connections();
    //        //every pin in nets
    //        for (std::list<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++) {
    //            if ((*pi)->get_inout()==0) {
    //                if ((*pi)->get_gate()->get_type().compare("evl_dff") == 0||
    //                    (*pi)->get_gate()->get_type().compare("evl_one") == 0||
    //                    (*pi)->get_gate()->get_type().compare("evl_zero") == 0)
    //                {
    //                    simulation_event event_temp(*ni,0);
    //                    std::list<simulation_event *>::iterator ei =  event_temp.find(all_events_);
    //                    insert_event((*ei));
    //                }
    //            }
    //        }
    //    }
    
    //compute "0" priorities
    for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++)
    {
        if ((*gi)->get_type().compare("evl_clock") == 0) {
            net * clk = (*gi)->get_pins().front()->get_nets().front();
            clk->set_comf();
            clk->set_value(true);
            simulation_event event_temp(clk, -1);
            std::list<simulation_event *>::iterator ei = event_temp.find(all_events);
            //insert_event((*ei));
            (*ei)->scheduled_ = true;//be careful
        }
        else if ((*gi)->get_type().compare("evl_dff") == 0 ||
                 (*gi)->get_type().compare("evl_one") == 0 ||
                 (*gi)->get_type().compare("evl_zero") == 0 ||
                 (*gi)->get_type().compare("evl_input") == 0)
            //||(*gi)->get_type().compare("evl_clock") == 0)
        {
            bool value_temp = false;// = (*gi)->compute_output();//read once
            int net_index = 0;
            std::vector<bool> pins_value_temp;
            if ((*gi)->get_type().compare("evl_input") == 0) {
                (*gi)->compute_output();
                pins_value_temp = (*gi)->evl_input_get_pins_value();
            }
            else
                value_temp = (*gi)->compute_output();
            //for every pin
            std::vector<pin *> pins_temp = (*gi)->get_pins();
            for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
            {
                if ((*pi)->get_inout() == 0)//if this pin is out!!! filp_flop has input pin !!!!!!!!!!!!!!!!
                {
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //for every net
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                    {
                        if ((*gi)->get_type().compare("evl_input") == 0){
                            (*ni)->set_value(pins_value_temp[net_index]);
                            net_index++;
                        }
                        else{
                            (*ni)->set_value(value_temp);
                        }
                        (*ni)->set_comf();
                        simulation_event event_temp(*ni, 0);
                        std::list<simulation_event *>::iterator ei = event_temp.find(all_events);
                        insert_event((*ei));
                        
                    }
                }
            }
        }
        
    }
    
    
    //compute next priority and event
    for (int i = 0; i < events_.size(); i++) {//NOTE!!!! events_.size() should change in the iteration
        //for every level
        event_list  this_level = events_[i];
        for (event_list::iterator ei = this_level.begin(); ei != this_level.end(); ei++) {
            //for every event
            //sent this scheduler to the current event
            (*ei)->insert_next_level_event(this);
        }
    }
}





void	map_priority_scheduler::optimal_run(int i)
{
    
    if (i == 0)//if the 1st simulation cycle
    {
        //compute priority********************************
        //set all unp
        for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++) {
            (*gi)->set_unp();
        }
        for (std::vector<net *>::iterator ni = nl_.nets_.begin(); ni != nl_.nets_.end(); ni++)
        {
            (*ni)->set_unp();
            (*ni)->set_uncomf();
        }
        //set one by one
        
        //if output of flip-flop, evl_one, evl_zero, evl_input, set as 0
        for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++) {
            int p_temp = (*gi)->get_priority();
            if ((*gi)->get_type().compare("evl_dff") != 0 &&
                (*gi)->get_type().compare("evl_input") != 0 &&
                (*gi)->get_type().compare("evl_output") != 0 &&
                (*gi)->get_type().compare("evl_one") != 0 &&
                (*gi)->get_type().compare("evl_zero") != 0 &&
                (*gi)->get_type().compare("evl_clock") != 0 )
            {

                insert_event(new simulation_event(*gi, p_temp));
            }
            
            //compute input and assign values to nets
            else if ((*gi)->get_type().compare("evl_input") == 0)
            {
                input_dff.push_back(*gi);
                (*gi)->compute_output();//read the file
                int net_index = 0;// for assigning values to net
                
                std::vector<bool> pins_value_temp = (*gi)->evl_input_get_pins_value();// get values
                //every pin in evl_input
                std::vector<pin *> pins_temp = (*gi)->get_pins();
                for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
                {
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //every net in the pin
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                    {
                        (*ni)->set_value(pins_value_temp[net_index]);
                        (*ni)->set_comf();
                        net_index++;
                        //if ((*ni)->get_bwchange())//if change
                        //{
                        //	insert_event(new simulation_event(*ni, 0));
                        //}
                    }
                    
                }
            }
            
            
            else if ((*gi)->get_type().compare("evl_dff") == 0)
            {
                net * net_temp = (*gi)->get_pins().front()->get_nets().front();
                net_temp->set_value((*gi)->compute_output());//update net's value
                net_temp->set_comf();
                //if (net_temp->get_bwchange())
                //{
                //	insert_event(new simulation_event(net_temp, 0));
                //}
                input_dff.push_back(*gi);
            }
            
            else if ((*gi)->get_type().compare("evl_one") == 0)
            {
                //(*gi)->compute_output();//read the file
                //int net_index = 0;// for assigning values to net
                
                std::vector<bool> pins_value_temp = (*gi)->evl_input_get_pins_value();// get values
                //every pin in evl_input
                std::vector<pin *> pins_temp = (*gi)->get_pins();
                for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
                {
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //every net in the pin
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                    {
                        (*ni)->set_value(1);
                        (*ni)->set_comf();
                        
                        //insert_event(new simulation_event(*ni, 0));
                    }
                    
                }
            }
            
            else if ((*gi)->get_type().compare("evl_zero") == 0)
            {
                //(*gi)->compute_output();//read the file
                //int net_index = 0;// for assigning values to net
                
                std::vector<bool> pins_value_temp = (*gi)->evl_input_get_pins_value();// get values
                //every pin in evl_input
                std::vector<pin *> pins_temp = (*gi)->get_pins();
                for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
                {
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //every net in the pin
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                    {
                        (*ni)->set_value(0);
                        (*ni)->set_comf();
                        //net_index++;
                        
                        //insert_event(new simulation_event(*ni, 0));
                    }
                    
                }
            }
            
            
            
            //(*gi)->set_p();
        }
        for (std::vector<net *>::iterator ni = nl_.nets_.begin(); ni != nl_.nets_.end(); ni++)
        {
            int p_temp = (*ni)->get_priority();
            
            
            //compute possible start events
            //if(p_temp == 0)
            //	possible_start_events_.push_back(new simulation_event(*ni, 0));
            
            //all_events.push_back(new simulation_event(*ni, p_temp));
            
            //
            
            
            insert_event(new simulation_event(*ni, p_temp));//note  output of evl_clock
            //(*ni)->set_p();
        }
        
        
        
        
        //compute priority end*************************************
        
        
        //for first circle, start events are all possible events
        //start_events_ = possible_start_events_;
        
        
        //set their values
        
        
        
        
        
        
        
        
        //finalize_priority();
        run();
        
        update_gates();
        
        
    }
    
    else
    {
        deploy_zero_nets();
        insert_fire();
        update_gates();
    }
    
    
    
    
}



void	map_priority_scheduler::deploy_zero_nets()//need intiate all priority
{
    
    //start_events_.clear();
    for (std::list<gate *>::iterator gi = input_dff.begin(); gi != input_dff.end(); gi++) {
        if ((*gi)->get_type().compare("evl_dff") == 0)
        {
            net * net_temp = (*gi)->get_pins().front()->get_nets().front();
            net_temp->update_value((*gi)->compute_output());//update net's value
            if (net_temp->get_bwchange())
            {
                insert_event(new simulation_event(net_temp, 0));
                //start_events_.push_back(new simulation_event(net_temp, 0));
            }
            
        }
        else if ((*gi)->get_type().compare("evl_input") == 0)
        {
            int net_index = 0;// for assigning values to net
            (*gi)->compute_output();//read the file
            std::vector<bool> pins_value_temp = (*gi)->evl_input_get_pins_value();// get values
            //every pin in evl_input
            std::vector<pin *> pins_temp = (*gi)->get_pins();
            for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
            {
                std::list<net *> nets_temp = (*pi)->get_nets();
                //every net in the pin
                for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                {
                    (*ni)->update_value(pins_value_temp[net_index]);
                    net_index++;
                    if ((*ni)->get_bwchange())//if change
                    {
                        insert_event(new simulation_event(*ni, 0));
                    }
                }
                
            }
        }

    }
	//for (std::list<gate *>::iterator gi = user_definded.begin(); gi != user_definded.end(); gi++)
	//{
	//		insert_event(new simulation_event(*gi, (*gi)->get_priority()));	
	//}
    
    
}




void	map_priority_scheduler::insert_fire()
{
    for (; !events_.empty();) {
        if (events_.begin()->second.empty()) {
            events_.erase(events_.begin());
            continue;
        }
        priority_event_i *event = events_.begin()->second.front();
        events_.begin()->second.pop_front();
        optimal_fire_event(event);
    }
    
    
    
    
}

void	priority_scheduler_i::optimal_fire_event(priority_event_i *event)
{
    event->scheduled_ = false;
    event->optimal_fire(this);
    
}



void	map_priority_scheduler::update_gates()
{
    for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++) {
        if ((*gi)->get_type().compare("evl_dff") == 0)
            (*gi)->compute_next_state();
        
        else if ((*gi)->get_type().compare("evl_output") == 0)// in lec13, evl_input update here
            (*gi)->compute_output();
        
    }
    
    
}






//bonus
void    map_priority_scheduler::hier_run(std::vector<net *> &input_nets, std::vector<bool> &inputs){
    
    if (first_run_flag) {
        first_run_flag = false;
        
        
        //compute priority********************************
        //set all unp
        for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++) {
            (*gi)->set_unp();
        }
        for (std::vector<net *>::iterator ni = nl_.nets_.begin(); ni != nl_.nets_.end(); ni++)
        {
            (*ni)->set_unp();
            (*ni)->set_uncomf();
        }
        
        //initiate 0 priority nets, from ports
		if (inputs.size() != input_nets.size())
		{
			assert(inputs.size() == input_nets.size());
		}
        assert(inputs.size() == input_nets.size());
        //initiate nets
        for (int i = 0; i < inputs.size(); i++) {
            input_nets[i]->set_value(inputs[i]);
            input_nets[i]->set_comf();
            input_nets[i]->set_priority(0);
            input_nets[i]->set_p();
        }
        
        
        
        //set one by one
        
        //if output of flip-flop, evl_one, evl_zero, evl_input, set as 0
        for (std::vector<gate *>::iterator gi = nl_.gates_.begin(); gi != nl_.gates_.end(); gi++) {
            int p_temp = (*gi)->get_priority();
            if ((*gi)->get_type().compare("evl_dff") != 0 &&
                (*gi)->get_type().compare("evl_input") != 0 &&
                (*gi)->get_type().compare("evl_output") != 0 &&
                (*gi)->get_type().compare("evl_one") != 0 &&
                (*gi)->get_type().compare("evl_zero") != 0 &&
                (*gi)->get_type().compare("evl_clock") != 0 )
            {
                
				if (find_in_third_party(*gi))
				{
					user_definded.push_back(*gi);
				}
				else
				{
					insert_event(new simulation_event(*gi, p_temp));
				}
            }
            
            //compute input and assign values to nets
            else if ((*gi)->get_type().compare("evl_input") == 0)
            {
                input_dff.push_back(*gi);
                (*gi)->compute_output();//read the file
                int net_index = 0;// for assigning values to net
                
                std::vector<bool> pins_value_temp = (*gi)->evl_input_get_pins_value();// get values
                //every pin in evl_input
                std::vector<pin *> pins_temp = (*gi)->get_pins();
                for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
                {
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //every net in the pin
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                    {
                        (*ni)->set_value(pins_value_temp[net_index]);
                        (*ni)->set_comf();
                        net_index++;
                        //if ((*ni)->get_bwchange())//if change
                        //{
                        //	insert_event(new simulation_event(*ni, 0));
                        //}
                    }
                    
                }
            }
            
            
            else if ((*gi)->get_type().compare("evl_dff") == 0)
            {
                net * net_temp = (*gi)->get_pins().front()->get_nets().front();
                net_temp->set_value((*gi)->compute_output());//update net's value
                net_temp->set_comf();
                //if (net_temp->get_bwchange())
                //{
                //	insert_event(new simulation_event(net_temp, 0));
                //}
                input_dff.push_back(*gi);
            }
            
			else if ((*gi)->get_type().compare("evl_one") == 0 || (*gi)->get_type().compare("evl_clock") == 0)
            {
                //(*gi)->compute_output();//read the file
                //int net_index = 0;// for assigning values to net
                
                std::vector<bool> pins_value_temp = (*gi)->evl_input_get_pins_value();// get values
                //every pin in evl_input
                std::vector<pin *> pins_temp = (*gi)->get_pins();
                for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
                {
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //every net in the pin
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                    {
                        (*ni)->set_value(1);
                        (*ni)->set_comf();
                        
                        //insert_event(new simulation_event(*ni, 0));
                    }
                    
                }
            }
            
            else if ((*gi)->get_type().compare("evl_zero") == 0)
            {
                //(*gi)->compute_output();//read the file
                //int net_index = 0;// for assigning values to net
                
                std::vector<bool> pins_value_temp = (*gi)->evl_input_get_pins_value();// get values
                //every pin in evl_input
                std::vector<pin *> pins_temp = (*gi)->get_pins();
                for (std::vector<pin *>::iterator pi = pins_temp.begin(); pi != pins_temp.end(); pi++)
                {
                    std::list<net *> nets_temp = (*pi)->get_nets();
                    //every net in the pin
                    for (std::list<net *>::iterator ni = nets_temp.begin(); ni != nets_temp.end(); ni++)
                    {
                        (*ni)->set_value(0);
                        (*ni)->set_comf();
                        //net_index++;
                        
                        //insert_event(new simulation_event(*ni, 0));
                    }
                    
                }
            }
            
            
            
            //(*gi)->set_p();
        }
        for (std::vector<net *>::iterator ni = nl_.nets_.begin(); ni != nl_.nets_.end(); ni++)
        {
            int p_temp = (*ni)->get_priority();
            
            //avoid insert twice
            if (find_net_in_nets(input_nets, (*ni))) {
                continue;
            }
            
            insert_event(new simulation_event(*ni, p_temp));//note  output of evl_clock
            //(*ni)->set_p();
        }
        
        
        
        
        //compute priority end*************************************

        run();//need support for user-defined gate
        
        update_gates();
        
        
    }//if(first_run_flag)
    
    else{   //if not first time run
        
        for (int i = 0; i < inputs.size(); i++) {
            input_nets[i]->update_value(inputs[i]);
            if (input_nets[i]->get_bwchange()) {
                insert_event(new simulation_event(input_nets[i], input_nets[i]->get_priority()));
            }
        }

        deploy_zero_nets();
        
        insert_fire();//need support for user-defined gate
        
        update_gates();
        
    }//if(!first_run_flag)
    
}









