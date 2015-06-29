//
//  simulation_events.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/21/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__simulation_events__
#define __Logic_Simulation__simulation_events__

#include "priority_scheduler.h"
#include "gate_implementation.h"

class   simulation_event: public    priority_event_i
{
    
    gate * gate_;
    net * net_;
    bool    bwgate_;
    int priority_;
    
	//bool	bwp;// if set, = true
    
public:
    
    
    virtual void fire(priority_scheduler_i *scheduler);
    virtual int get_priority();
    void    set_priority(int pri);
    
    gate *    get_gate_pointer();
    net *     get_net_pointer();
    bool    get_bwgate();
    
    bool compare(simulation_event *event);// if equal, return 0
    
    std::list<simulation_event *>::iterator   find(std::list<simulation_event *> &all_events);
    
    bool    find_next_events(std::list<simulation_event *> &next_events);
    bool    check_prev_events();// return 1 if all previous events are scheduled
    //simulation_event &operator=(const simulation_event &se);
    
    virtual void    insert_next_level_event(priority_scheduler_i *scheduler);
    
  
	simulation_event(gate* gate, int pri) :gate_(gate), net_(NULL), bwgate_(true), priority_(pri), priority_event_i()
	{
		//gate->set_priority(pri); gate->set_p(); priority_ = gate->get_priority();
	}
	simulation_event(net* net, int pri) :gate_(NULL), net_(net), bwgate_(false), priority_(pri), priority_event_i()
	{
		//net->set_priority(pri); net->set_p(); priority_ = net->get_priority();
	}
    simulation_event():gate_(NULL), net_(NULL), bwgate_(false), priority_(-1), priority_event_i(){}
    

//	//for optimal
	void	optimal_fire(priority_scheduler_i *scheduler);
	int	get_priority_optimal()
	{
		if (bwgate_)
		{
			return	gate_->get_priority();
		}
		else
		{
			return	net_->get_priority();
		}
	}

	void	set_priority_optimal()
	{
		if (bwgate_)
		{
			gate_->set_priority(0);
			priority_ = gate_->get_priority();
		}
		else
		{
			net_->set_priority(0);
			priority_ = net_->get_priority();
		}
	}




};



extern std::list<simulation_event *> all_events;



#endif /* defined(__Logic_Simulation__simulation_events__) */
