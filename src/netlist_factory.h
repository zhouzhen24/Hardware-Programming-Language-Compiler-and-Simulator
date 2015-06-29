//
//  factory_pattern.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__factory_pattern__
#define __Logic_Simulation__factory_pattern__
#include "evl_data_netlist.h"



struct netlist_factory_i
{
    virtual gate *make_gate(std::string type, std::string name) = 0;
    virtual net *make_net() = 0;
    virtual pin *make_pin() = 0;
}; // struct netlist_factory_i


class netlist_factory : public netlist_factory_i
{
public:
    virtual gate *make_gate(std::string type, std::string name);
    virtual net *make_net();
    virtual pin *make_pin();
}; // class netlist_factory




#endif /* defined(__Logic_Simulation__factory_pattern__) */
