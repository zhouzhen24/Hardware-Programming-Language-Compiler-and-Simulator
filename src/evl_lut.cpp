//
//  evl_lut.cpp
//  Logic_Simulation_Phase2
//
//  Created by Zhen Zhou on 11/28/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "evl_lut.h"
#include "gate_prototypes.h"
#include "evl_data_netlist.h"
#include "hex_to_bin.h"
#include <cmath>

bool evl_lut::validate_structural_semantics() {
    
    assert(pins_.size() == 2);
    
    pins_[0]->set_as_output();
    pins_[1]->set_as_input();
    
    return true;
}

evl_lut * evl_lut::clone(std::string name)
{
    return new evl_lut(name);
}

void evl_lut::store_prototype()
{
    gate_prototypes::instance()->store("evl_lut", new evl_lut("prototype"));
}

std::vector<bool> evl_lut::evl_lut_get_pins_value()
{
    return values_;
}


bool evl_lut::evaluate(const std::vector<bool> &inputs)// read the evl_lut file and get 1 word into gate
                                                            //assign values_ to output pins in simulation_events::optimal_fire()
{
    //read file
    std::string lut_file_name = netlist::evl_file_name + "." + get_name() + ".evl_lut";
    std::ifstream   lut;
    lut.open(lut_file_name.c_str());
    assert(lut.is_open());
    
    //convert inputs(pins_[1]) from binary into decimal
        //note index in inputs!!!!!!
    int input_line_no = 0;//input line number
    for (int i = 0; i < inputs.size(); i++) {//i indicates 2^i
        input_line_no = input_line_no + inputs[i] * int(pow(2,i));
    }
    std::string line;
    for (int line_no = 1; std::getline(lut, line); line_no++) {
        //for each line
        
        
        //only first time read in pins_width
        if (first_flag) {
            first_flag = false;
            
            assert(line_no == 1);
            
            for (size_t i = 0; i < line.size();)
            {
                //read output(pins_[0])'s size()
                int size_buf = 0;//store pin_[1] and pin[0] 's size
                if (line[i] <= '9' && line[i] >= '0' )
                {
                    size_t number_begin = i;
                    for (++i; i < line.size(); ++i)		//find the end of NUBMER
                    {
                        if (line[i] == ' ')
                        {
                            break;
                        }
                    }
                    for (; number_begin < i; number_begin++) {
                        size_buf = size_buf*10 + int(line[number_begin] - '0');
                    }
                    if(size_buf != pins_[0]->get_nets().size()){
                        std::cerr << "output pin size error in " << get_type() << " " << get_name() << std::endl;
                        assert(false);
                    }
                    pins_width.push_back(size_buf);
                    size_buf = 0;
                }
                i++;
                //read input(pins_[1])'s size()
                //int size_buf = 0;
                if (line[i] <= '9' && line[i] >= '0' )
                {
                    size_t number_begin = i;
                    for (++i; i < line.size(); ++i)		//find the end of NUBMER
                    {
                        if (line[i] == ' ')
                        {
                            break;
                        }
                    }
                    for (; number_begin < i; number_begin++) {
                        size_buf = size_buf*10 + int(line[number_begin] - '0');
                    }
                    if(size_buf != pins_[1]->get_nets().size()){
                        std::cerr << "input pin size error in " << get_type() << " " << get_name() << std::endl;
                        assert(false);
                    }
                    pins_width.push_back(size_buf);
                    size_buf = 0;
                }
                
            }
        }//if first_flag
        
        if (line_no == input_line_no + 2) {//note address(inputs) "0" indicate line 2 in lut file
            
            
            
            //int   pin_no = 0;// count pins' number
            std::bitset<4> bit_temp(0000);
            std::string    values_temp;
            
            
            values_.clear();
            //start to read current line
            for (size_t i = 0; i < line.size();)
            {
                
                
                //read lut values
                for (values_temp.clear();
                    ((line[i] <= '9' && line[i] >= '0')||
                     (line[i] <= 'f' && line[i] >= 'a')||
                     (line[i] <= 'F' && line[i] >= 'A'))
                     &&(i != line.size());
                     i++)
                {
                    bit_temp = hex_to_bin(line[i]);
                    values_temp = values_temp + bit_temp.to_string();
                }
                
                //save the pin's value
                if (values_temp.size() < pins_width[0])
                {
                    for (std::string::reverse_iterator ci = values_temp.rbegin(); ci != values_temp.rend(); ci++)
                    {
                        values_.push_back((*ci)-'0');
                    }
                    for (size_t compliment = values_temp.size(); compliment < pins_width[0]; compliment++) {
                        values_.push_back(0);
                    }
                }
                else//compliment with "0"s
                {
                    for (std::string::reverse_iterator ci = values_temp.rbegin(); ci != values_temp.rbegin()+pins_width[0]; ci++)
                    {
                        values_.push_back((*ci)-'0');
                    }
                }//save finish
		lut.close();
                return true;//force to exit the for loop
            }
            //break;
        }//else if (line_no == input_line_no + 2)
    }//getline()
    
    lut.close();
    
    //assign values to pins in simulation_events::optimal_fire(), in netlist::simulation()
    
    std::cerr << "Error address in " << get_type() << " " << get_name() << std::endl;
    return false;
}
