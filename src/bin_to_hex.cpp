//
//  bin_to_hex.cpp
//  Logic_Simulation_Phase2
//
//  Created by Zhen Zhou on 11/28/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "bin_to_hex.h"




std::string  bin_to_hex(std::bitset<4> bit)
{
    if (bit.to_string().compare("0000") == 0)   return  std::string("0");
    if (bit.to_string().compare("0001") == 0)   return  std::string("1");
    if (bit.to_string().compare("0010") == 0)   return  std::string("2");
    if (bit.to_string().compare("0011") == 0)   return  std::string("3");
    if (bit.to_string().compare("0100") == 0)   return  std::string("4");
    if (bit.to_string().compare("0101") == 0)   return  std::string("5");
    if (bit.to_string().compare("0110") == 0)   return  std::string("6");
    if (bit.to_string().compare("0111") == 0)   return  std::string("7");
    if (bit.to_string().compare("1000") == 0)   return  std::string("8");
    if (bit.to_string().compare("1001") == 0)   return  std::string("9");
    if (bit.to_string().compare("1010") == 0)   return  std::string("A");
    if (bit.to_string().compare("1011") == 0)   return  std::string("B");
    if (bit.to_string().compare("1100") == 0)   return  std::string("C");
    if (bit.to_string().compare("1101") == 0)   return  std::string("D");
    if (bit.to_string().compare("1110") == 0)   return  std::string("E");
    if (bit.to_string().compare("1111") == 0)   return  std::string("F");
    
    else
        return std::string("");
    
}
