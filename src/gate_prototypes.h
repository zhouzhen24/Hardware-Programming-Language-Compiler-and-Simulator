//
//  gate_prototypes.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/16/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef Logic_Simulation_gate_prototypes_h
#define Logic_Simulation_gate_prototypes_h
#include <string>
#include <map>
#include "evl_data_netlist.h"
#include "assert.h"

//typedef std::map<std::string, gate *> gate_prototypes;



class gate_prototypes
{
    gate_prototypes(const gate_prototypes &);            // no copy
    gate_prototypes &operator=(const gate_prototypes &); // no assignment
    
    gate_prototypes() {}   // use private ctor and dtor to prevent
    ~gate_prototypes() {}  // the creation of additional instances
    
    std::map<std::string, gate *> prototypes_;
    
    static gate_prototypes *instance_; // a pointer to the only instance
    
public:
    
    void store(std::string name, gate *g);
    gate *locate(std::string name);
    static gate_prototypes *instance(); // access the only instance
    
}; // class gate_prototypes

void store_standard_gate_prototypes();

void store_third_party_prototypes();



#endif
