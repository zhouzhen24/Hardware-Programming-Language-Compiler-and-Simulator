//
//  evl_lut.h
//  Logic_Simulation_Phase2
//
//  Created by Zhen Zhou on 11/28/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation_Phase2__evl_lut__
#define __Logic_Simulation_Phase2__evl_lut__

#include "logic_gate.h"


//in .evl
// an example for evl_lut is
// evl_lut  rom(word, address);
// word is output, address is input

class evl_lut: public logic_gate
{
    std::vector<bool> state_, next_state_;
    evl_lut * clone(std::string name);
    
    //int lineno_;// = 1;
    //int tranno_;// = 0;
    std::vector<int> pins_width;
    std::vector<bool> values_;//pin with least index on the top of the vector
    /********** For Example ***************/
    //
    //  word in evl_lut file is a   (1010)
    //                              word[3] word[2] word[1] word[0]
    //  word[0] is values_[0]   value:0
    //  word[1] is values_[1]   value:1
    //  word[2] is values_[2]   value:0
    //  word[3] is values_[3]   value:1
    //
    //
    
    
    
    bool    first_flag;// = true;
    
    
public:
    evl_lut(std::string name): logic_gate("evl_lut", name), state_(false), next_state_(false),first_flag(true) {}/*,lineno_(1), tranno_(0), */
    bool    validate_structural_semantics();
    //   static void store_prototype(gate_prototypes &gps);
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &luts);
    std::vector<bool> evl_lut_get_pins_value();
    
    
    //Might Suprise!!!
    //bool    evaluate(const std::vector<bool> &luts);
    
    
    
    
};


#endif /* defined(__Logic_Simulation_Phase2__evl_lut__) */
