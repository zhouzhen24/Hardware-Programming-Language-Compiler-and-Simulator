//
//  composite_pattern.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__composite_pattern__
#define __Logic_Simulation__composite_pattern__


#include "evl_data_netlist.h"
#include "new_struct.h"
#include "priority_scheduler.h"

class module: public gate
{
    friend  class   hier_factory;
    
    
//    module *parent_; // optional, will be NULL for top module
//    std::list<gate *> children_;
    netlist nl_;                // internal structure
    std::vector<port *> ports_; // should match pins_
    
    map_priority_scheduler  scheduler;
    
    bool validate_structural_semantics();
    
    
    
    
public:
    module(std::string type, std::string name);
    module(std::string type);
    //module(std::string type, std::string name, module *parent);
    ~module();
    
    
    void    create_ports(evl_ports ports);
    //void    connect_ports_to_nets();
    
    virtual void retrieve(std::vector<bool> &inputs);
    virtual bool    compute_output(std::vector<bool> &v);
    
    //bonus
	void get_nl(netlist * &nl){
		nl = &nl_;
	}
    
    
}; // class module


#endif /* defined(__Logic_Simulation__composite_pattern__) */
