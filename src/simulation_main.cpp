//
//  main.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/7/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//


#include "simulation_main.h"



int simulation_main(int argc, char * argv[]) {
    
    
    //    if(netlist_main(argc, argv) != 0){
    //        std::cerr << "Error in Netlist Construction" << std::endl;
    //        return -1;
    //    }
    
    // testing part
    
    // Syntactic Analysis
    evl_module		module;			// objects to store computer-readable statements
    evl_wires		wires;
    evl_components	comps;
    evl_endmodule	endmodule;
    
    //everything completed in Project 1 & 2 is hidden in syn()
    if (!syn(argc, argv, module, wires, comps, endmodule))
    {
        std::cerr << "Error in Syntacitc Analysis" << std::endl;
        return	-1;
    }
    
    
    store_standard_gate_prototypes();
    //        gate_prototypes gps;
    //        store_standard_gate_prototypes(gps);
    //store_third_party_prototypes();
    
    netlist nl(argv);
    
    //netlist_builder builder(nl);//1st
    
    netlist_factory factory;
    netlist_builder builder(nl, &factory);
    
    evl_director(&builder, wires, comps);
    builder.finalize_creation();
    // testing part ends
    // save the netlist nl or perform simulation
    nl.save(std::string(argv[1])+".netlist", module.type);
    nl.simulate(1000);
    
    
    return 0;
}
