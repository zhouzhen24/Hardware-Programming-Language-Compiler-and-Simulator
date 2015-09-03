//
//  priority_scheduler.h
//  Cpractice
//
//  Created by Zhen Zhou on 11/19/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Cpractice__priority_scheduler__
#define __Cpractice__priority_scheduler__

#include <stdio.h>
#include <list>
#include <map>
#include "evl_data_netlist.h"


class priority_scheduler_i;

class priority_event_i
{
    friend class priority_scheduler_i;
    friend class map_priority_scheduler;
    bool scheduled_;
    
    virtual void fire(priority_scheduler_i *scheduler) = 0;
    virtual int get_priority() = 0;
    
	//for optiaml
	virtual	void	optimal_fire(priority_scheduler_i *scheduler) = 0;
    
    
protected:
    priority_event_i() : scheduled_(false) {}

    
public:
  
    virtual bool    get_schedule(){return scheduled_;}
    virtual void    insert_next_level_event(priority_scheduler_i *scheduler) = 0;
    
}; // class priority_event_i



class priority_scheduler_i
{
    virtual void insert(priority_event_i *event, int priority) = 0;

protected:
    void fire_event(priority_event_i *event); // called by derived schedulers

	//for optimal
	void optimal_fire_event(priority_event_i *event);

public:
    void insert_event(priority_event_i *event); // called by derived events
    
}; // class priority_scheduler_i


class map_priority_scheduler : public priority_scheduler_i
{
    typedef std::list<priority_event_i *> event_list;
    
    std::map<int, event_list> events_;//alternativly, std::multimap can be used here
    //       <priority, events>
    virtual void insert(priority_event_i *event, int priority);

    netlist &nl_;

	//optimal
	std::list<gate *>	input_dff;
	//std::list<priority_event_i *> start_events_;

    //bonus
    bool    first_run_flag;
	std::list<gate *>	user_definded;
    
    
public:
    void run();
    void finalize_priority();
    map_priority_scheduler(netlist &nl):nl_(nl), first_run_flag(true){}

	//for optimize
	void	optimal_run(int i);//i = iteration of simulation times, from 0 to cycle-1

	void	deploy_zero_nets();
	void	insert_fire();
	void	update_gates();
    
    void    hier_run(std::vector<net *> &input_nets, std::vector<bool> &inputs);

}; // class map_priority_scheduler




#endif /* defined(__Cpractice__priority_scheduler__) */
