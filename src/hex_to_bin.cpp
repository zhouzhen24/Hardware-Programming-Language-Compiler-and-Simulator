//
//  hex_to_bin.cpp
//  Logic_Simulation_Phase2
//
//  Created by Zhen Zhou on 11/28/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "hex_to_bin.h"

std::bitset<4>  hex_to_bin(char s)
{
    
    if (s == '0')             return std::bitset<4>(0x0);
    if (s == '1')             return std::bitset<4>(0x1);
    if (s == '2')             return std::bitset<4>(0x2);
    if (s == '3')             return std::bitset<4>(0x3);
    if (s == '4')             return std::bitset<4>(0x4);
    if (s == '5')             return std::bitset<4>(0x5);
    if (s == '6')             return std::bitset<4>(0x6);
    if (s == '7')             return std::bitset<4>(0x7);
    if (s == '8')             return std::bitset<4>(0x8);
    if (s == '9')             return std::bitset<4>(0x9);
    if (s == 'a' || s == 'A') return std::bitset<4>(0xa);
    if (s == 'b' || s == 'B') return std::bitset<4>(0xb);
    if (s == 'c' || s == 'C') return std::bitset<4>(0xc);
    if (s == 'd' || s == 'D') return std::bitset<4>(0xd);
    if (s == 'e' || s == 'E') return std::bitset<4>(0xe);
    if (s == 'f' || s == 'F') return std::bitset<4>(0xf);
    
    
    else    return std::bitset<4>(0000);
}

