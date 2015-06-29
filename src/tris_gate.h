//
//  tris_gate.h
//  Logic_Simulation_Phase2
//
//  Created by Zhen Zhou on 11/28/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation_Phase2__tris_gate__
#define __Logic_Simulation_Phase2__tris_gate__

#include "logic_gate.h"


class tris_gate: public logic_gate
{
    bool state_, next_state_;
    tris_gate * clone(std::string name);
    //for tris
    //void retrieve(std::vector<bool> &inputs);
public:
    tris_gate(std::string name): logic_gate("tris", name), state_(false), next_state_(false) {}
    bool    validate_structural_semantics();
    //   static void store_prototype(gate_prototypes &gps);
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &inputs);
    
    //for tris
    virtual bool compute_output(std::vector<bool> &v);
    
    
    
    
    //Might Suprise!!!
    //bool    evaluate(const std::vector<bool> &inputs);
};

#endif /* defined(__Logic_Simulation_Phase2__tris_gate__) */
