//
//  evl_output.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/18/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "evl_output.h"
#include "gate_prototypes.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include "evl_data_netlist.h"
#include <string>
#include <bitset>
#include "bin_to_hex.h"

bool evl_output::validate_structural_semantics() {
    
    for (size_t i = 0; i < pins_.size(); i++)
    {
        pins_[i]->set_as_input();
    }
    
    return true;
}

evl_output * evl_output::clone(std::string name)
{
    return new evl_output(name);
}

void evl_output::store_prototype()
{
    gate_prototypes::instance()->store("evl_output", new evl_output("prototype"));
}

bool evl_output::evaluate(const std::vector<bool> &inputs)
{
    //only support logical values
    //std::ofstream   out_put(std::string("ARGV")+std::string(".")+std::string(get_name())+".evl_output");
    std::string output_file_name(netlist::evl_file_name+std::string(".")+std::string(get_name())+".evl_output");
    
    //check whether file exits...
    //std::ifstream   in_put;
    //std::ofstream   out_put;
    if (first_flag) {//if output file does not exit
        first_flag = false;
        std::ofstream out_put(output_file_name.c_str());
        out_put << get_pins().size() << std::endl;
        std::cout << "Number of out_pins: " << get_pins().size() << std::endl;
        for (std::vector<pin *>::iterator pi = pins_.begin(); pi != pins_.end(); pi++) {
            out_put << (*pi)->get_nets().size() << std::endl;
            std::cout << "pin[" << int(pi-pins_.begin()) << "] 's width: " << (*pi)->get_nets().size() << std::endl;
        }
        for (std::vector<bool>::const_iterator i = inputs.begin(); i != inputs.end(); ) {
            //for every pin in evl_output gate
            for (std::vector<pin *>::iterator pi = pins_.begin(); pi != pins_.end(); pi++) {
                size_t  pinsize = (*pi)->get_nets().size();
                //size_t  bitsize = ceil(pinsize/4);//bit size for hex
                std::string outpin;
                std::bitset<4> bset;
                for (size_t epi = 0; epi < pinsize; epi++) {
                    size_t  bpos = epi%4;
                    bset.set(bpos,(*i));
                    i++;
                    if ((bpos == 3)||(epi == pinsize -1)) {
                        //outpin = bset.to_string() + outpin;
                        outpin = bin_to_hex(bset)+outpin;
                        bset.reset();
                    }
                }
                if (pi != pins_.begin()) {
                    out_put << " ";
                    std::cout << " ";
                }
                out_put << outpin;
                std::cout << outpin;
            }
        }
        out_put << std::endl;
        std::cout << std::endl;
    }
    else{// if exist
        
//        rename(output_file_name.c_str(), "temp.evl_output");
//        std::ifstream in_put("temp.evl_output");
//        std::ofstream out_put(output_file_name.c_str());
//        //copy every line
//        std::string line;
//        for (; std::getline(in_put,line); ) {
//            out_put << line << std::endl;
//        }
//        remove("temp.evl_output");
        std::ofstream    out_put;
        out_put.open(output_file_name.c_str(),std::ios::app);
	if (!out_put.is_open())
	{
		std::cerr << "Cannot reopen " << output_file_name << std::endl;
	}
        for (std::vector<bool>::const_iterator i = inputs.begin(); i != inputs.end(); ) {
            //for every pin in evl_output gate
            for (std::vector<pin *>::iterator pi = pins_.begin(); pi != pins_.end(); pi++) {
                size_t  pinsize = (*pi)->get_nets().size();
                //size_t  bitsize = ceil(pinsize/4);//bit size for hex
                std::string outpin;
                std::bitset<4> bset;
                for (size_t epi = 0; epi < pinsize; epi++) {
                    size_t  bpos = epi%4;
                    bset.set(bpos,(*i));
                    i++;
                    if ((bpos == 3)||(epi == pinsize -1)) {
                        //outpin = bset.to_string() + outpin;
                        outpin = bin_to_hex(bset)+outpin;
                        bset.reset();
                    }
                }
                if (pi != pins_.begin()) {
                    out_put << " ";
                    std::cout << " ";
                }
                out_put << outpin;
                std::cout << outpin;
            }
        }
        out_put << std::endl;
        std::cout << std::endl;
        out_put.close();
    }
    


    
    
    

    
    return true;
}


//bool    evl_output::first_flag = true;

