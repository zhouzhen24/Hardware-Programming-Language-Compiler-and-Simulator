//
//  new_netlist_struct.h
//  Project_Bonus_ECE449
//
//  Created by Zhen Zhou on 12/1/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef Project_Bonus_ECE449_new_struct_h
#define Project_Bonus_ECE449_new_struct_h

#include "evl_data_structures_statement.h"
#include "evl_data_netlist.h"

#include <vector>


#include "buf_gate.h"

class   port{
    
    size_t     index_;
    bool    inout_;
    std::string name_;
    int     width_;
    pin *   pin_outside_;
    pin *   pin_inside_;
    
    //std::vector<net *> nets_;
    
public:
    port(std::string name, int width):name_(name), width_(width){}
    ~port(){delete pin_outside_; delete pin_inside_;}
    
    void    set_as_input();
    void    set_as_output();
    std::string get_name();
    int     get_width();
    bool    get_inout();
    pin *   get_pin_outside();
    pin *   get_pin_inside();
    void    assign_pin_outside(pin * pin);
    void    assign_pin_inside(pin * pin);
    void    set_index(size_t index);
    
    
    void    add_net_to_port(net * net_input);
    

	//convert port to buf
	std::vector<buf_gate *> create_buf();


};





//statements
struct  one_module_statements
{
    evl_statements statements_;
    bool    bwtop_;
    
    
};
typedef std::list<one_module_statements> one_module_statements_group;

//netlist
struct  new_netlist{
    
    evl_module		module;			// objects to store computer-readable statements
    evl_wires		all_wires;
    evl_components	all_components;
    evl_endmodule	endmodule;
    
    //assign
    evl_assigns     all_assigns;
    
    
    //evl_ports       all_ports;
    
    evl_components  get_assign_as_buf();
    evl_wires       get_ports_as_wires();
    
};
typedef std::list<new_netlist> new_netlists;


//global variable

extern new_netlists    notop_netlists;




#endif
