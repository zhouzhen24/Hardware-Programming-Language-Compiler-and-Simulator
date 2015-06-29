//
//  logic_gate.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/17/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "logic_gate.h"


bool    logic_gate::compute_output()
{
    std::vector<bool>   inputs;
    //retrieve logic values from all inputs
    //for each pin
    if(this->get_type().compare("evl_dff") != 0)//if it is not flip_flop
        retrieve(inputs);
    
    bool    output = evaluate(inputs);
    return  output;
}


//void    logic_gate::save_inputs(std::vector<bool> inputs)
//{
//    inputs_ = inputs;
//}
//
//bool    logic_gate::check_inputs(std::vector<bool> inputs)
//{
//    assert(inputs.size() == inputs_.size());
//    for (size_t i = 0; i < inputs.size(); i++) {
//        if (inputs[i] != inputs_[i]) {
//            return  false;
//        }
//    }
//    
//    return true;
//}



void    logic_gate::retrieve(std::vector<bool> &inputs)
{
 //if it is not flip_flop
        for (std::vector<pin *>::iterator pi = pins_.begin(); pi != pins_.end(); pi++) {
            if ((*pi)->get_inout() == 0) {
                continue;
            }
            std::list<net *> net_temp = (*pi)->get_nets();
            //for each net in pin
            for (std::list<net *>::iterator ni = net_temp.begin(); ni != net_temp.end(); ni++) {
                bool input = (*ni)->retrieve_logic_value();
                inputs.push_back(input);
            }
        }
    
}