//
//  director_pattern.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/14/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__director_pattern__
#define __Logic_Simulation__director_pattern__


#include "evl_data_structures_statement.h"
#include "netlist_builder.h"
#include "netlist_main.h"

void evl_director(netlist_builder *builder, const evl_wires &wires, const evl_components &comps);


#endif /* defined(__Logic_Simulation__director_pattern__) */
