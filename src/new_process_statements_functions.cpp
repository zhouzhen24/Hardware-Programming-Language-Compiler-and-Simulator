//
//  new_process_statements_functions.cpp
//  Project_Bonus_ECE449
//
//  Created by Zhen Zhou on 12/1/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "new_process_statements_functions.h"





bool    segment_statements(evl_statements &all_statements, one_module_statements_group &top_statements, one_module_statements_group &notop_statements){
    
    bool    top_flag = false;
    one_module_statements one_module_statement_temp;
    for (evl_statements::iterator si = all_statements.begin(); si != all_statements.end(); si++) {
        //for every statment
        
        //for module
        //whether top module
        if ((*si).type == evl_statement::MODULE) {
            evl_tokens::iterator   token_temp = (*si).tokens.begin();
            token_temp++;
            token_temp++;//point to 3rd token in module
            if ((*token_temp).str.compare("(") == 0) {
                top_flag = false;
            }
            else{
                top_flag = true;
            }
        }
        
        one_module_statement_temp.statements_.push_back((*si));
        
        
        
        
        //for endmodule
        if ((*si).type == evl_statement::ENDMODULE) {
            one_module_statement_temp.bwtop_ = top_flag;
            if (top_flag) {
                top_statements.push_back(one_module_statement_temp);
            }
            else{
                notop_statements.push_back(one_module_statement_temp);
            }
            one_module_statement_temp.statements_.clear();
        }
    }    
    return true;
}




bool    process_top_statements(evl_statements &statements, evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule, evl_assigns &assigns){
    
    
        for (; !statements.empty(); statements.pop_front())
        {
            if ((statements.front().type) == (evl_statement::MODULE))
            {
                if (!new_process_module_statement(module, statements.front()))
                {
                    std::cerr << "Wrong MODULE statement when store in Object" << std::endl;
                    return	false;
                }
            }
            else if ((statements.front().type) == (evl_statement::WIRE))
            {
                if (!process_wire_statement(wires, statements.front()))
                {
                    std::cerr << "Wrong WIRE statement when store in Object" << std::endl;
                    return	false;
                }
            }
            else if ((statements.front().type) == (evl_statement::COMPONENT))
            {
                if (!process_component_statement(components, statements.front()))
                {
                    std::cerr << "Wrong COMPONENTS statement when store in Object" << std::endl;
                    return	false;
                }
            }
            else if ((statements.front().type) == (evl_statement::ENDMODULE))
            {
                if (!process_endmodule_statement(endmodule, statements.front()))
                {
                    std::cerr << "Wrong ENDMODULE statement when store in Object" << std::endl;
                    return	false;
                }
            }
            else if ((statements.front().type) == (evl_statement::ASSIGN))
            {
                if (!process_assign_statement(assigns, statements.front()))
                {
                    std::cerr << "Wrong ASSIGN statement when store in Object" << std::endl;
                    return	false;
                }
            }
            else
            {
                std::cerr << "Wrong Statmement Type" << std::endl;
                return false;
            }
        }
    return true;
}



bool    process_notop_statements(evl_statements &statements, evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule, evl_assigns &assigns){
    
    for (; !statements.empty(); statements.pop_front())
    {
        if ((statements.front().type) == (evl_statement::MODULE))
        {
            if (!new_process_module_statement(module, statements.front()))
            {
                std::cerr << "Wrong MODULE statement when store in Object" << std::endl;
                return	false;
            }
        }
        else if ((statements.front().type) == (evl_statement::WIRE))
        {
            if (!process_wire_statement(wires, statements.front()))
            {
                std::cerr << "Wrong WIRE statement when store in Object" << std::endl;
                return	false;
            }
        }
        else if ((statements.front().type) == (evl_statement::COMPONENT))
        {
            if (!process_component_statement(components, statements.front()))
            {
                std::cerr << "Wrong COMPONENTS statement when store in Object" << std::endl;
                return	false;
            }
        }
        else if ((statements.front().type) == (evl_statement::ENDMODULE))
        {
            if (!process_endmodule_statement(endmodule, statements.front()))
            {
                std::cerr << "Wrong ENDMODULE statement when store in Object" << std::endl;
                return	false;
            }
        }
        else if ((statements.front().type) == (evl_statement::ASSIGN))
        {
            if (!process_assign_statement(assigns, statements.front()))
            {
                std::cerr << "Wrong ASSIGN statement when store in Object" << std::endl;
                return	false;
            }
        }
        else
        {
            std::cerr << "Wrong Statmement Type" << std::endl;
            return false;
        }
    }
    return true;
}



bool process_assign_statement(evl_assigns &assigns, evl_statement &s){
    
    enum state_type { INIT, LHS, LHS_NAME, LHS_BUS, LHS_MSB, LHS_COLON, LHS_LSB, LHS_DONE, RHS, RHS_NAME, RHS_BUS, RHS_MSB, RHS_COLON, RHS_LSB, RHS_DONE, DONE };
    state_type state = INIT;
    evl_assign	assign;
    for (; !s.tokens.empty() && state != DONE; s.tokens.pop_front())
    {
        evl_token t = s.tokens.front();

        //Enter state machine        
        if (state == INIT)
        {
            if (t.str.compare("assign") == 0)
            {
                state = LHS;
            }
            else
            {
                std::cerr << "Need 'assign' token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == LHS)
        {
            if (t.type == evl_token::NAME)
            {
                assign.lhs.name = t.str;
                assign.lhs.bus_lsb = -1;
                assign.lhs.bus_msb = -1;
                state = LHS_NAME;
            }
            else
            {
                std::cerr << "Need a name but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == LHS_NAME)
        {
            if (t.str == "=")
            {
                state = RHS;
            }
            else if (t.str == "["){
                state = LHS_BUS;
            }
            else
            {
                std::cerr << "Need '[' or '=' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == LHS_BUS)
        {
            
            if (t.type == evl_token::NUMBER)
            {
                assign.lhs.bus_msb = atoi(t.str.c_str());
                state = LHS_MSB;
            }
            else
            {
                std::cerr << "Need a number but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == LHS_MSB)
        {
            if (t.str == "]")
            {
                state = LHS_DONE;
            }
            else if (t.str == ":")
            {
                state = LHS_COLON;
            }
            else
            {
                std::cerr << "Need ']' or ':' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == LHS_COLON)
        {
            if (t.type == evl_token::NUMBER)
            {
                assign.lhs.bus_lsb = atoi(t.str.c_str());
                state = LHS_LSB;
            }
            else
            {
                std::cerr << "Need a number but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == LHS_LSB)
        {
            if (t.str == "]")
            {
                state = LHS_DONE;
            }
            else
            {
                std::cerr << "Need ']' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == LHS_DONE)
        {
            if (t.str == "=")
            {
                state = RHS;
            }
            else
            {
                std::cerr << "Need '=' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        //RHS
        else if (state == RHS)
        {
            if (t.type == evl_token::NAME)
            {
                assign.rhs.name = t.str;
                assign.rhs.bus_lsb = -1;
                assign.rhs.bus_msb = -1;
                state = RHS_NAME;
            }
            else
            {
                std::cerr << "Need a name but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == RHS_NAME)
        {
            if (t.str == ";")
            {
                state = DONE;
            }
            else if (t.str == "["){
                state = RHS_BUS;
            }
            else
            {
                std::cerr << "Need '[' or '=' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == RHS_BUS)
        {
            
            if (t.type == evl_token::NUMBER)
            {
                assign.rhs.bus_msb = atoi(t.str.c_str());
                state = RHS_MSB;
            }
            else
            {
                std::cerr << "Need a number but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == RHS_MSB)
        {
            if (t.str == "]")
            {
                state = RHS_DONE;
            }
            else if (t.str == ":")
            {
                state = RHS_COLON;
            }
            else
            {
                std::cerr << "Need ']' or ':' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == RHS_COLON)
        {
            if (t.type == evl_token::NUMBER)
            {
                assign.rhs.bus_lsb = atoi(t.str.c_str());
                state = RHS_LSB;
            }
            else
            {
                std::cerr << "Need a number but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == RHS_LSB)
        {
            if (t.str == "]")
            {
                state = RHS_DONE;
            }
            else
            {
                std::cerr << "Need ']' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == RHS_DONE)
        {
            if (t.str == ";")
            {
                state = DONE;
            }
            else
            {
                std::cerr << "Need ';' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
    }//s.token == empty
    assigns.push_back(assign);
    return true;
}


bool new_process_module_statement(evl_module &module, evl_statement &s){
    
    enum state_type { INIT, MODULE, TYPE, PORTS, PORT_TYPE, BUS, BUS_MSB, BUS_COLON, BUS_LSB, BUS_DONE, PORT_NAME, PORTS_DONE, DONE };
    state_type state = INIT;
    //evl_module  module_temp;
    evl_port    port;

    for (; !s.tokens.empty() && state != DONE; s.tokens.pop_front())
    {
        evl_token t = s.tokens.front();
    
        
        if (state == INIT) {
            if (t.str.compare("module") == 0) {
                state = MODULE;
            }
            else{
                std::cerr << "Need 'module' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
    
        else if (state == MODULE) {
            if (t.type == evl_token::NAME) {
                module.type = t.str;
                state = TYPE;
            }
            else{
                std::cerr << "Need a NAME token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }

    
        else if (state == TYPE) {
            if (t.str == ";") {
                state = DONE;
            }
            else if (t.str == "("){
                state = PORTS;
            }
            else{
                std::cerr << "Need ';' or '(' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        
        else if (state == PORTS) {
            if (t.type == evl_token::NAME) {
                port.type = t.str;
                port.width = 1;
                state = PORT_TYPE;
            }
            else{
                std::cerr << "Need NAME token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }

        
        else if (state == PORT_TYPE) {
            if (t.type == evl_token::NAME) {
                port.name = t.str;
                
                //create wires
                port.wire.name = port.name;
                port.wire.width = port.width;
                
                
                
                module.ports.push_back(port);
                
                
                state = PORT_NAME;
            }
            else if (t.str == "["){
                state = BUS;
            }
            else{
                std::cerr << "Need a NAME token or '[' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        
        
        else if (state == BUS){
            
            if (t.type == evl_token::NUMBER) {
                port.width = atoi(t.str.c_str()) + 1;
                state = BUS_MSB;
            }
            else{
                std::cerr << "Need a NUBMER token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        
        else if (state == BUS_MSB){
            
            if (t.str == ":") {
                state = BUS_COLON;
            }
            else{
                std::cerr << "Need ':' but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
    
    
        else if (state == BUS_COLON){
            
            if (t.str == "0") {
                state = BUS_LSB;
            }
            else{
                std::cerr << "Need a '0' token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
    
    
        else if (state == BUS_LSB){
            
            if (t.str == "]") {
                state = BUS_DONE;
            }
            else{
                std::cerr << "Need ']' token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }
        }
        
        else if (state == BUS_DONE){
            
            if (t.type == evl_token::NAME) {
                
                port.name = t.str;
                
                //create wires
                port.wire.name = port.name;
                port.wire.width = port.width;
                
                module.ports.push_back(port);
                state = PORT_NAME;
            }
        }
    
        else if (state == PORT_NAME){
            
            if (t.str == ",") {
                state = PORTS;
            }
            else if (t.str == ")"){
                state = PORTS_DONE;
            }
            else{
                std::cerr << "Need ',' or ')' token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }

        }
        
        else if (state == PORTS_DONE){
            if (t.str == ";") {
                state = DONE;
            }
            else{
                std::cerr << "Need ';' token but found '" << t.str << "' on line " << t.line_no << std::endl;
                return false;
            }

        }
        
    
    
    }//for every token in the statement
    
    
    
    return true;
}



bool    find_elements(new_netlists &notop_netlists, std::string &type, evl_module &module, evl_ports &ports, evl_wires &wires, evl_components  &comps){
    //for each netlist in notop_netlist
    for (new_netlists::iterator ni = notop_netlists.begin(); ni != notop_netlists.end(); ni++) {
        if (type.compare((*ni).module.type) == 0) {//find the module
            module = (*ni).module;
            ports = (*ni).module.ports;
            wires = (*ni).all_wires;

			//for ci system
			evl_wires	wires_temp = (*ni).get_ports_as_wires();
			wires.insert(wires.end(), wires_temp.begin(), wires_temp.end());





            comps = (*ni).all_components;
            return true;
        }
    }
    
    std::cerr << "The type: " << type << " is not predefined" << std::endl;
    return false;
    
}


bool    find_net_in_nets(std::vector<net *> input_nets, net * input_net){
    
    for (std::vector<net *>::iterator ni = input_nets.begin(); ni != input_nets.end(); ni++) {
        if (input_net == (*ni)) {
            return true;
        }
    }
    return false;
}



bool    find_in_third_party(gate * gate_input){
    
    //in notop_netlist's module
    for (std::list<new_netlist>::iterator nni = notop_netlists.begin(); nni != notop_netlists.end(); nni++) {
        if ((*nni).module.type.compare(gate_input->get_type()) == 0) {
            return true;
        }
    }
    return false;   
}
