//
//  evl_input.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/24/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "evl_input.h"
#include "gate_prototypes.h"
#include "evl_data_netlist.h"
#include "hex_to_bin.h"


bool evl_input::validate_structural_semantics() {
    
    for (size_t i = 0; i < pins_.size(); i++)
    {
        pins_[i]->set_as_output();
    }
    
    return true;
}

evl_input * evl_input::clone(std::string name)
{
    return new evl_input(name);
}

void evl_input::store_prototype()
{
    gate_prototypes::instance()->store("evl_input", new evl_input("prototype"));
}

std::vector<bool> evl_input::evl_input_get_pins_value()
{
    return pins_value;
}


bool evl_input::evaluate(const std::vector<bool> &inputs)
{
    //read file
    std::string input_file_name = netlist::evl_file_name + "." + get_name() + ".evl_input";
    std::ifstream   in_put;
    in_put.open(input_file_name.c_str());
    assert(in_put.is_open());
    
    int number = 0;
    std::string line;
    int current_line_trans_no = 0;
    for (int line_no = 1; std::getline(in_put, line); line_no++) {
        if (first_flag) {
            first_flag = false;
            
            for (size_t i = 0; i < line.size();)
            {
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
                        number = number*10 + int(line[number_begin] - '0');
                    }
                    assert(number == pins_.size());
                    number = 0;
                }
                i++;
                for (; i < line.size(); )
                {
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
                            number = number*10 + int(line[number_begin] - '0');
                        }
                        pins_width.push_back(number);
                        number = 0;
                    }
                    i++;
                }
                
                
            }
            lineno_ = 2;
            tranno_ = 0;
            continue;
        }
        
        if (line_no == lineno_) {
            
            
            
            int   pin_no = 0;// count pins' number
            std::bitset<4> bit_temp(0000);
            std::string    values_temp;
            
            
            size_t i = 0;//start to read current line
            
            //read transition number
            if (line[i] <= '9' && line[i] >= '0')
            {
                size_t number_begin = i;
                for (++i; i < line.size(); ++i)		//find the end of NUBMER
                {
                    if (line[i] == ' ')
                    {
                        break;
                    }
                }
                
                int transition_temp = 0;
                for (; number_begin < i; number_begin++) {
                    transition_temp = transition_temp*10 + int(line[number_begin] - '0');
                }
                current_line_trans_no = transition_temp;
                if (tranno_ >= current_line_trans_no) {// '>' for the end of file when simulation has not ended
                    tranno_ = 0;
                    lineno_++;
                    
                    continue;///////////////////NOTE the exit
                }
                
            }
            i++;
            //pins_value.clear();
            
            for (pins_value.clear(); i < line.size();i++)
            {
                
                
                //read input values
                
                //pins_value.clear();
                values_temp.clear();
                for (; ((line[i] <= '9' && line[i] >= '0')||
                        (line[i] <= 'f' && line[i] >= 'a')||
                        (line[i] <= 'F' && line[i] >= 'A'))
                     &&(i != line.size());i++)
                {
                    bit_temp = hex_to_bin(line[i]);
                    values_temp = values_temp + bit_temp.to_string();
                }
                
                //save the pin's value
                if (values_temp.size() < pins_width[pin_no])
                {
                    for (std::string::reverse_iterator ci = values_temp.rbegin(); ci != values_temp.rend(); ci++)
                    {
                        //std::cout << *ci << std::endl;
                        pins_value.push_back((*ci)-'0');
                    }
                    for (size_t compliment = values_temp.size(); compliment < pins_width[pin_no]; compliment++) {
                        pins_value.push_back(0);
                    }
                }
                else
                {
                    for (std::string::reverse_iterator ci = values_temp.rbegin(); ci != values_temp.rbegin()+pins_width[pin_no]; ci++)
                    {
                        //std::cout << *ci << std::endl;
                        pins_value.push_back((*ci)-'0');
                    }
                }//save finish
                
                pin_no++;
                if (pin_no == pins_width.size()) {//if all pins' values have been saved
                    break;
                }
                
                
                
            }
            break;
        }
    }//getline()
    
    in_put.close();
    
    //assign values to pins in netlist::simulation()
    
    //update transition number
    
    tranno_++;
    
    return true;
}
