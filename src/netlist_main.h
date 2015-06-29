//
//  netlist.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/7/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef _netlist_main
#define _netlist_main

#include	"syn.h"
#include    "evl_data_netlist.h"



//	the function pin_semantic_update() can handle pin semantic.
//  But for the case where pin connect to a whole bus is different from what's in Project 3 specification
//  As required, lsb = 0 and msb = -1 in this case
//  in the function, lsb = 0 and msb = wire.width - 1
bool	pin_semantic_update(const evl_wires_table &wires_table, evl_components &comps);


int	netlist_main(int argc, char* argv[]);




#endif
