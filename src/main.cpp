//
//  main.cpp
//  Project_Bonus_ECE449
//
//  Created by Zhen Zhou on 12/1/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//


//from previous
#include "simulation_main.h"
#include "evl_functions_tokens_to_statements.h"


//new in bonus
#include "new_struct.h"
#include "new_process_statements_functions.h"
#include "hier_factory.h"

int main(int argc, char * argv[]) {
    
    
    //*segment statements when syntatic analysis is operating
    //*need new structrue to store groups of statements
    //*the group has bwtop_ member
    
    //lexcial
    if(!lex(argc, argv))
    {
        std::cerr << "Error in Lexcial Analysis" << std::endl;
        return -1;
    }
    std::string	input_tokens = argv[1];
    input_tokens+=".tokens";
    evl_tokens	all_tokens;// to store all tokens from the .tokens file
    if (!read_tokens_file(all_tokens, argc, input_tokens))
    {
        std::cerr << "Cannot read tokens from .tokens file" << std::endl;
        return -1;
    }
    evl_statements	all_statements;// to store all statements from all_tokens
    if (!group_tokens_into_statements(all_statements,all_tokens))
    {
        std::cerr << "Cannot group tokens into statements from structure 'all_tokens' " << std::endl;
        return -1;
    }
    
    
    //all statements saved in all_statements
    
    one_module_statements_group top_statements, notop_statements;
    
    segment_statements(all_statements, top_statements, notop_statements);//segment
    
    assert(top_statements.size()==1);//only one top module
    
    //*convert each group statements into module, components, wires, endmodule, (probably also ports)
    //*new structures needed, to store different modules to endmodules
    
    new_netlist    top_netlist;//, notop_netlists;//notop_netlist is from new_struct.h
    
    //if bwtop_
    if(!process_top_statements(top_statements.front().statements_,
		 top_netlist.module,
		 top_netlist.all_wires,
		 top_netlist.all_components,
		 top_netlist.endmodule,
		 top_netlist.all_assigns))
    {
        std::cerr << "Cannot process statements in top module" << std::endl;
        return -1;
    }

    //add assign to "assign" in evl_components
	if (!top_netlist.all_assigns.empty())
	{
		//for ci system
		evl_components	buf_components_temp = top_netlist.get_assign_as_buf();
		top_netlist.all_components.insert(top_netlist.all_components.end(),
						                    buf_components_temp.begin(),
						                    buf_components_temp.end());
	}


    //if not bwtop_
    for (one_module_statements_group::iterator nsi = notop_statements.begin(); nsi != notop_statements.end(); nsi++) {
        new_netlist notop_netlist_temp;
        if(!process_notop_statements((*nsi).statements_, 
		notop_netlist_temp.module, 
		notop_netlist_temp.all_wires,
	        notop_netlist_temp.all_components, 
		notop_netlist_temp.endmodule,
		notop_netlist_temp.all_assigns))
	{
            std::cerr << "Cannot process statements in user-definded gate" << std::endl;
            return -1;
        }
	if (!notop_netlist_temp.all_assigns.empty())
		{
			//for ci system
			evl_components	buf_components_temp = notop_netlist_temp.get_assign_as_buf();
			notop_netlist_temp.all_components.insert(notop_netlist_temp.all_components.end(),
							 buf_components_temp.begin(),
							 buf_components_temp.end());
		}
        notop_netlists.push_back(notop_netlist_temp);
    }
    
    //*each group of statements in new structure establied module or netlist
        //in top_netlist and notop_netlists
    
    //if bwtop_
    //*add third party
    
    store_standard_gate_prototypes();
    
    netlist nl(argv);
    
    hier_factory factory;
    netlist_builder builder(nl, &factory);
    
    evl_director(&builder, top_netlist.all_wires, top_netlist.all_components);
    builder.finalize_creation();

    // testing part ends
    // save the netlist nl or perform simulation
    nl.save(std::string(argv[1])+".netlist", top_netlist.module.type);
    nl.simulate(1000);
    
    std::cout << "Program Ends\n";
    return 0;
}
