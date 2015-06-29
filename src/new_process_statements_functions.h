//
//  new_process_statements_functions.h
//  Project_Bonus_ECE449
//
//  Created by Zhen Zhou on 12/1/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Project_Bonus_ECE449__new_process_statements_functions__
#define __Project_Bonus_ECE449__new_process_statements_functions__

#include "evl_data_structures_statement.h"
#include "new_struct.h"
#include "evl_functions_tokens_to_statements.h"



bool    segment_statements(evl_statements &all_statements, one_module_statements_group &top_statements, one_module_statements_group &notop_statements);


bool    process_top_statements(evl_statements &statements, evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule, evl_assigns &assigns);



bool    process_notop_statements(evl_statements &statements, evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule, evl_assigns &assigns);


//assign
bool process_assign_statement(evl_assigns &assigns, evl_statement &s);

//new module statement
bool new_process_module_statement(evl_module &module, evl_statement &s);



//new_netlists    notop_netlists;
bool    find_elements(new_netlists &notop_netlists, std::string &type, evl_module &module, evl_ports &ports, evl_wires &wires, evl_components  &comps);

bool    find_net_in_nets(std::vector<net *> input_nets, net * input_net);

bool    find_in_third_party(gate * gate_input);


#endif /* defined(__Project_Bonus_ECE449__new_process_statements_functions__) */
