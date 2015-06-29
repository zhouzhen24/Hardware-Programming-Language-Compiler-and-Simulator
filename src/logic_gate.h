//
//  logic_gate.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/17/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__logic_gate__
#define __Logic_Simulation__logic_gate__

#include "evl_data_netlist.h"

class logic_gate: public gate
{
    virtual bool evaluate(const std::vector<bool> &inputs) = 0;
    virtual void retrieve(std::vector<bool> &inputs);
protected:
    
    
    logic_gate(std::string type, std::string name):gate(type, name){};
    virtual void compute_next_state(){}
    virtual bool validate_structural_semantics() = 0;
public:
    bool compute_output();
    
    //for tris
    virtual bool compute_output(std::vector<bool> &v){return false;}
    
    
    //for evl_input only
	virtual std::vector<bool> evl_input_get_pins_value(){ std::vector<bool> v; v.push_back(false); return v; }
    virtual std::vector<bool> evl_lut_get_pins_value(){ std::vector<bool> v; v.push_back(false); return v; }




}; // class logic_gate

#endif /* defined(__Logic_Simulation__logic_gate__) */
