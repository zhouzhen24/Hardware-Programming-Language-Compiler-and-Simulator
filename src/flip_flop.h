//
//  Flip_flop.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/13/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__flip_flop__
#define __Logic_Simulation__flip_flop__


#include "evl_data_netlist.h"
#include "gate_prototypes.h"
#include "logic_gate.h"
//class flip_flop: public gate {
//    bool state_, next_state_;
//public:
//    flip_flop(std::string name)
//    : gate("evl_dff", name), state_(false), next_state_(false) {}
//    void    compute_next_state();
//    bool    validate_structural_semantics();
//
//}; // class flip_flop(evl_dff)


//
//typedef std::map<std::string, gate *> gate_prototypes;


class   flip_flop: public logic_gate
{
    bool    state_, next_state_;
    
    flip_flop *clone(std::string name);
public:
    flip_flop(std::string name): logic_gate("evl_dff", name), state_(false), next_state_(false) {}
    void    compute_next_state();
    bool    validate_structural_semantics();
//    static void store_prototype(gate_prototypes &gps);
    static void store_prototype();
    bool evaluate(const std::vector<bool> &inputs);
    
};


void store_standard_gate_prototypes();






#endif /* defined(__Logic_Simulation__Flip_flop__) */
