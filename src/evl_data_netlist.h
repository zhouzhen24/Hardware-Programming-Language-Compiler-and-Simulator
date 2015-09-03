#ifndef GUARD_NETLIST_H
#define GUARD_NETLIST_H

#include    "evl_data_structures_statement.h"
#include    <map>
#include	<sstream>
#include	<iostream>
#include	<fstream>
#include    <vector>


class netlist;
class gate;
class net;
class pin;

// wires table
typedef std::map<std::string, int> evl_wires_table;
evl_wires_table make_wires_table(const evl_wires &wires);

// nets table
typedef std::map<std::string, net *> evl_nets_table;
std::string make_net_name(std::string wire_name, int i);

class net
{
    friend  class   netlist_builder;
	friend	class   port;

    std::list<pin *> connections_;
	std::string	name_;
    bool    value_;
    bool    bwcomf;//1:comfirmed,not ?   ; 0:uncomfirmed, ?
	
	 //for optimize
	bool	next_value_;
	bool	bwchange_;
	int		priority_;
	bool	bwp_;// if set, = true
	
    //for tris
    bool    value1_;
    bool    next_value1_;
    //      value1_ value_      true value in net
    //          00                      0
    //          01                      1
    //          10                      Z
    //          11                      X
    
public:
	net(std::string net_name);
	net() :bwcomf(false), value_(false), value1_(false){}
    bool    find_drive(pin * &drive);
    bool    retrieve_logic_value();
    
    void             append_pin(pin *p);
	std::string      get_name();
	int              get_size();
	std::list<pin *> get_connections();
    bool             get_value(){return value_;}
    void             set_comf(){bwcomf = true;}
    void             set_uncomf(){bwcomf = false;}
    void             set_value(bool v){value_ =v; value1_ = false; }
    //for tris
    void             set_value(bool v1, bool v);
    void             update_value(bool inpput_value1, bool input_value);
	void             get_values(std::vector<bool> &v); //v[0] == value_;v[1] == value1

    
	//for optimal
	bool	get_bwchange();
	void	update_value(bool input_value);
	int		get_priority();
	void	set_priority(int input_p) { priority_ = input_p; }
	void	set_unp() { bwp_ = false; }
	void	set_p() { bwp_ = true; }
	int		ask_priority();// return value is the priority of this level



}; // class net

class pin
{
    friend  class   netlist_builder;
    friend  class   port;
	friend	class	module;

    gate *gate_;
    size_t pin_index_;
    std::list<net *> nets_;
    
    bool    in_out_;// out:0  ;  in:1
public:
	bool	create(gate *g, size_t pin_index, const evl_pin &p, const evl_nets_table &nets_table);
    
	//read members
	gate *	get_gate();
	size_t	get_index();
	std::list<net *> get_nets();
    bool    get_inout(){return in_out_;}
    
    void    set_as_output(){ in_out_ = 0; }
    void    set_as_input() { in_out_ = 1; }
    
    //bonus
    pin(size_t pin_index):gate_(NULL), pin_index_(pin_index){}
    pin(){}
    
}; // class pin

class gate
{
    friend  class   netlist_builder;
    friend  class   netlist_factory;
    friend  class   hier_factory;
    
private:

	std::string	type_;
	std::string	name_;
    
    
    
	bool            create_pin(const evl_pin &ep, size_t pin_index, const evl_nets_table &nets_table, const evl_wires_table &wires_table);


    virtual gate * clone(std::string name)
    {
        return new gate;
    }

	//for optimal
	int		priority_;
	bool	bwp_;// if set, = true
protected:
        std::vector<pin *> pins_;
    
public:
	bool	create(evl_component c, const evl_nets_table &nets_table_, const evl_wires_table &wires_table);
    
	gate(std::string type, std::string name) :type_(type), name_(name)/*priority_(-1), bwp_(false)*/{}
    gate(){}
	//read members
	std::vector<pin *> get_pins();
	std::string	get_type();
	std::string	get_name();

    virtual bool	validate_structural_semantics();
    virtual bool    compute_output(){return false;}
    virtual bool    compute_output(std::vector<bool> &v){return false;}
    virtual void    compute_next_state(){}

    //for evl_input only
    virtual std::vector<bool> evl_input_get_pins_value(){std::vector<bool> v;v.push_back(false); return v;}
    //for evl_lut only
    virtual std::vector<bool> evl_lut_get_pins_value(){std::vector<bool> v;v.push_back(false); return v;}
    
	//for optimal
	virtual int		get_priority();
	void	set_priority(int input_p) { priority_ = input_p; }
	void	set_unp() { bwp_ = false; }
	void	set_p() { bwp_ = true; }
	int		ask_priority();//return value is priority of THIS level

	virtual void get_nl(netlist * &nl){}



}; // class gate

class netlist
{
    friend  class   netlist_builder;
    friend  class   map_priority_scheduler;
	friend	class   module;
    
private:
    std::vector<gate *> gates_;
    std::vector<net *> nets_;
    
    evl_nets_table nets_table_;
    
    
    // functions
    bool    create_nets(const evl_wires &wires);
    void    create_net(std::string net_name);
    bool    create_gates(const evl_components &comps, const evl_wires_table &wires_table);
	bool	create_gate(const evl_component &c, const evl_wires_table &wires_table);


public:
    netlist(char* argv[]){evl_file_name = std::string(argv[1]);}
    netlist(){}
    bool   create(const evl_wires &wires, const evl_components &comps, const evl_wires_table &wires_table);
    bool   save(std::string file_name, std::string module_name);
    void   simulate(int cycles);
    
    static std::string evl_file_name;
    
    //read members
    std::vector<gate *> get_gates();
    std::vector<net *> get_nets();
    evl_nets_table  get_nets_table();
    
}; // class netlist



#endif
