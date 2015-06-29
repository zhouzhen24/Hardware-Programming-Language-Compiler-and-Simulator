//
//  evl_data_netlist.cpp
//  Project3_Netlist
//
//  Created by Zhen Zhou on 10/22/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include    "evl_data_netlist.h"
#include    <assert.h>


#include "simulation_event.h"


// netlist


// netlist create function
bool netlist::create(const evl_wires &wires,
                     const evl_components &comps,
                     const evl_wires_table &wires_table)
{
    return create_nets(wires)&&create_gates(comps, wires_table);
}


// output netlist to .netlist file
bool   netlist::save(std::string file_name,
                     std::string module_name)
{
    //std::string fname = evl_file_name + ".netlist";
    //std::ofstream   output_file(fname.c_str());
    std::ofstream output_file(file_name.c_str());

    
    //output module name
    output_file << "module " << module_name << std::endl;
    
    //output nets
    output_file << "nets " << nets_.size() << std::endl;
    
    
    for (std::vector<net *>::iterator ni = nets_.begin(); ni != nets_.end(); ++ni)
    {
        //output net	(net has a sequence of pins(connections) connected to the net) (net also has a name)
        output_file << "  net " << (*ni)->get_name() << " " << (*ni)->get_size() << std::endl;
        
        //output pin_gate & pin_position(pin_index)
        std::list<pin *>	net_pins = (*ni)->get_connections();
        for (std::list<pin *>::iterator pi = net_pins.begin(); pi != net_pins.end(); ++pi)
        {
            output_file << "    " << ((*pi)->get_gate())->get_type()<<" ";
            if (((*pi)->get_gate())->get_name() != "")//(*pi)->get_gate()->get_name().empty()
            {
                output_file << ((*pi)->get_gate())->get_name() << " ";
            }
            output_file << (*pi)->get_index() << std::endl;
        }
    }
    
    //output components (gates)
    output_file << "components " << gates_.size() << std::endl;
    
    
    for (std::vector<gate *>::iterator ci = gates_.begin(); ci != gates_.end(); ++ci)
    {
        //output component	(component has type and a sequence of pins)
        output_file << "  component " << (*ci)->get_type() << " ";
        if ((*ci)->get_name() != "")//(*pi)->get_gate()->get_name().empty()
        {
            output_file << (*ci)->get_name() << " ";
        }
        output_file << (*ci)->get_pins().size() << std::endl;
        
        std::vector<pin *>	gate_pins = (*ci)->get_pins();
        for (std::vector<pin*>::iterator pi = gate_pins.begin(); pi != gate_pins.end(); ++pi)
        {
            //output pin	(pin has width & net names connected to it)
            output_file << "    pin " << (*pi)->get_nets().size();
            
            std::list<net *>	pin_net = (*pi)->get_nets();
            for (std::list<net *>::iterator ni = pin_net.begin(); ni != pin_net.end(); ++ni)
            {
                //output net names connected to pin
                output_file << " " << (*ni)->get_name();
            }
            output_file << std::endl;
        }
    }
    return true;
}


// functions to create nets

bool netlist::create_nets(const evl_wires &wires)
{
    for(evl_wires::const_iterator w=wires.begin(); w!=wires.end();++w)
    {
        if (w->width == 1)
        {
            create_net(w->name);
        }
        else
        {
            for (int i = 0; i < w->width; ++i)
            {
                std::string	net_name = make_net_name(w->name, i);
                create_net(net_name);
            }
        }
    }
    return	true;
}

void netlist::create_net(std::string net_name)
{
    assert(nets_table_.find(net_name) == nets_table_.end());
    net *n = new net(net_name);
    nets_table_[net_name] = n;
    nets_.push_back(n);
}


// functions to create gates

bool    netlist::create_gates(const evl_components &comps,
                              const evl_wires_table &wires_table)
{
    for (evl_components::const_iterator ci = comps.begin(); ci !=comps.end(); ++ci)
    {
        evl_component   c = (*ci);
        create_gate(c, wires_table); // gate semantic vilation only output an error, does not stop the program
        
    }
    return true;
}

bool	netlist::create_gate(const evl_component &c,
                             const evl_wires_table &wires_table)
{
//    gate *g;
//    if (c.type == "evl_dff") {             //details in ppt:lec10, Page 108
//        g = new flip_flop(c.name);
//    }
//    else if (c.type == "and") {
//        g = new and_gate(c.name);
//    }
    //else if...
    
    gate *g = new gate;
    gates_.push_back(g);
    return g->create(c, nets_table_, wires_table);
}

// to access nets, gates and nets_table
std::vector<gate *> netlist::get_gates()
{
    return gates_;
}

std::vector<net *> netlist::get_nets()
{
    return nets_;
}

evl_nets_table  netlist::get_nets_table()
{
    return  nets_table_;
}

/*----------------------------------------*/
// net

net::net(std::string net_name)
{
    name_ = net_name;
    value_ = false;
	//bwp_ = false;
	//next_value_ = false;
	//bwchange_ = true;
	//priority_ = -1;
	bwcomf = false;
    value1_ = false;
}

void net::append_pin(pin *p)
{
    connections_.push_back(p);
}

std::string	net::get_name()
{
    return	name_;
}

int	net::get_size()
{
    return	int(connections_.size());
}

std::list<pin *> net::get_connections()
{
    return	connections_;
}

bool    net::find_drive(pin * &drive)
{
    size_t drive_count = 0;
    pin *temp = NULL;
    for (std::list<pin *>::iterator pi = connections_.begin(); pi != connections_.end(); pi++) {
        if ((*pi)->get_inout() == 0) {
            gate * gate_temp = (*pi)->get_gate();
            if (gate_temp == NULL) {//bonus//in port,
                drive_count++;
                temp = (*pi);
                continue;
            }
            else if (gate_temp->get_type().compare("buf") == 0 ||
                gate_temp->get_type().compare("tris") ==0 ) {//for tris
                std::vector<bool> output_of_gate;
                gate_temp->compute_output(output_of_gate);
                if (output_of_gate[1] == 1) {//if Z or X
                    continue;//skip drive_count
                }
                
            }
            drive_count++;
            temp = (*pi);
        }
    }
    if (drive_count == 1) {
        drive = temp;
        return true;
    }
    else  if  (drive_count ==0){
       // std::cerr << get_name() << " has no drive" << std::endl;
        return false;
    }
    else{
        std::cerr << get_name() << " has too many drives" << std::endl;
        assert(false);
        //return false;
    }
}


bool    net::retrieve_logic_value()
{
	pin * drive = NULL;
    if(find_drive(drive)){
        if (bwcomf) {
            return value_;
        }
        else{
            set_comf();
            value1_ = 0;
            value_ = (drive->get_gate())->compute_output();//NOTE: flip_flop
            return value_;
        }
    }
    else{//if all is Z
        value1_ = 1;
        value_ = 1;
		set_comf();
        return value_;

    }
    
}


//



/*----------------------------------------*/
// gate

bool	gate::create(evl_component c,
                     const evl_nets_table &nets_table,
                     const evl_wires_table &wires_table)
{
    //store gate type and name;
    type_ = c.type;
    if (c.name != "")
        name_ = c.name;
    size_t pin_index = 0;
    //	evl_pins	c_pins = c->pins;
    for (evl_pins::const_iterator p = (c.pins).begin(); p != (c.pins).end(); ++p)//each evl_pin ep in c
    {
        evl_pin	ep = (*p);
        create_pin(ep, pin_index, nets_table, wires_table);
        ++pin_index;
    }
    return validate_structural_semantics();
}

bool gate::create_pin(const evl_pin &ep,
                      size_t pin_index,
                      const evl_nets_table &nets_table,
                      const evl_wires_table &wires_table) //const net table
{
    
    assert(wires_table.find(ep.name) != wires_table.end());//resolve semantics of ep using wires_table. This guarantee that the name of a pin is the name of some wire.
    pin *p = new pin;
    pins_.push_back(p);
    return p->create(this, pin_index, ep, nets_table);
}


 bool	gate::validate_structural_semantics()   //
{
    //for project 4. details in ppt:lec10, Page 97
    //assert(false);
    //return false;
    
    // predef_gates
    //  type                                 # of pins
    //	and, or, xor                            >=3
    //  not, buf                                =2
    //  evl_dff, tris                           =3
    //  evl_clock                               =1
    //	evl_one, evl_zero, evl_input            >=1
    //  evl_output                              >=1
    
    
    if ((type_ == "and") || (type_ == "or") || (type_ == "xor"))
    {
        if (pins_.size() < 3)
        {
            std::cerr << "at least " << (3 - pins_.size()) << "more pins needed in " << type_ << std::endl;
            return false;
        }
        std::vector<pin *> pins = get_pins();
        for (std::vector<pin *>::const_iterator pi = pins_.begin(); pi != pins_.end(); ++pi)
        {
            // for each pin
            std::list<net *> pin_nets = (*pi)->get_nets();
            if(pin_nets.size()!=1)
            {
                std::cerr << "Pins connected to " << type_ << " does not connect to exactly 1 net" << std::endl;
                return false;
            }
        }
        
    }
    
    else if ((type_ == "not") || (type_ == "buf"))
    {
        if (pins_.size() != 2)
        {
            std::cerr << type_ << " gate must have exactly 2 pins" << std::endl;
            return false;
        }
        std::vector<pin *> pins = get_pins();
        for (std::vector<pin *>::const_iterator pi = pins_.begin(); pi != pins_.end(); ++pi)
        {
            // for each pin
            std::list<net *> pin_nets = (*pi)->get_nets();
            if(pin_nets.size()!=1)
            {
                std::cerr << "Pins connected to " << type_ << " does not connect to exactly 1 net" << std::endl;
                return false;
            }
        }
        
    }
    
    else if ((type_ == "evl_dff") || (type_ == "tris"))
    {
        if (pins_.size() != 3)
        {
            std::cerr << type_ << " gate must have exactly 3 pins" << std::endl;
            return false;
        }
        std::vector<pin *> pins = get_pins();
        for (std::vector<pin *>::const_iterator pi = pins_.begin(); pi != pins_.end(); ++pi)
        {
            // for each pin
            std::list<net *> pin_nets = (*pi)->get_nets();
            if(pin_nets.size()!=1)
            {
                std::cerr << "Pins connected to " << type_ << " does not connect to exactly 1 net" << std::endl;
                return false;
            }
        }
        
    }
    
    
    else if ((type_ == "evl_clock"))
    {
        if (pins_.size() != 1)
        {
            std::cerr << type_ << " gate must have exactly 1 pin" << std::endl;
            return false;
        }
        std::vector<pin *> pins = get_pins();
        for (std::vector<pin *>::const_iterator pi = pins_.begin(); pi != pins_.end(); ++pi)
        {
            // for each pin
            std::list<net *> pin_nets = (*pi)->get_nets();
            if(pin_nets.size()!=1)
            {
                std::cerr << "Pins connected to " << type_ << " does not connect to exactly 1 net" << std::endl;
                return false;
            }
        }
    }
    
    else if ((type_ == "evl_one") || (type_ == "evl_zero") || (type_ == "evl_input") || (type_ == "evl_output"))
    {
        if (pins_.size() < 1)
        {
            std::cerr << "at least 1 more pin needed in " << type_ << std::endl;
            return false;
        }
        
    }
    
    else
    {
        std::cerr << name_ << "'s type '" << type_ << "' is not predefined" << std::endl;
        return false;
    }
    
    return true;
    
    
}


std::vector<pin *> gate::get_pins()
{
    return	pins_;
}

std::string	gate::get_type()
{
    return	type_;
}

std::string	gate::get_name()
{
    return	name_;
}

/*------------------------------------*/
// pin

bool pin::create(gate *g,
                 size_t pin_index,
                 const evl_pin &p,
                 const evl_nets_table &nets_table)
{
    gate_ = g;
    pin_index_ = pin_index;//store g and pin_index;
    if (p.bus_msb == -1)
    {
        // a 1-bit wire
        net * n;
        evl_nets_table::const_iterator ni=nets_table.find(p.name);
        if (ni != nets_table.end())
        {
            n = ni->second;
        }
        else
        {
            std::cerr << "Cannot find pin's name in nets_table" << std::endl;
            return false;
        }
        //net = find net_name in nets_table
        //assert(nets_table.find(p.name) != nets_table.end());
        nets_.push_back(n);
        n->append_pin(this);
    }
    else
    {
        // a bus ...
        for (int bus_index = p.bus_lsb; bus_index <= p.bus_msb; ++bus_index)
        {
            //			if (bus_index == -1)
            //				bus_index = p.bus_msb;
            std::string	pin_name = make_net_name(p.name, bus_index);
            net	*n;
            evl_nets_table::const_iterator	ni = nets_table.find(pin_name);
            if (ni != nets_table.end())
            {
                n = ni->second;
            }
            else
            {
                std::cout << "Cannot find pin's name in nets_table" << std::endl;
                return false;
            }
            //net = find net_name in nets_table
            //assert(nets_table.find(p.name) != nets_table.end());
            nets_.push_back(n);
            n->append_pin(this);
        }
    }
    return	true;
}


gate *	pin::get_gate()
{
    return	gate_;
}

size_t	pin::get_index()
{
    return	pin_index_;
}

std::list<net *> pin::get_nets()
{
    return	nets_;
}





// Other functions for Netlist Construction

evl_wires_table make_wires_table(const evl_wires &wires)
{
    evl_wires_table wires_table;
    
    for (evl_wires::const_iterator it = wires.begin();
         it != wires.end(); ++it)
    {
        std::pair<std::string, int>  _pair=std::make_pair(it->name, it->width);
        wires_table.insert(_pair);
    }
    
    return wires_table;
}


std::string make_net_name(std::string wire_name,
                          int i)
{
    assert(i >= 0);
    std::ostringstream oss;
    oss << wire_name << "[" << i << "]";
    return oss.str();
}



//Project 4 Simulation

void    netlist::simulate(int cycles)
{
	map_priority_scheduler  scheduler(*this);
    for (int i = 0; i < cycles; i++)
    {

		//scheduler.optimal_run(i);

		//std::vector<net *> &input_nets, std::vector<bool> &inputs

		std::vector<net *> input_nets;
		std::vector<bool> inputs;


		scheduler.hier_run(input_nets, inputs);
        
    }//simulate desired cycles
    
    
}

std::string netlist::evl_file_name = "default_evl_file_name";







//optimal

int	net::ask_priority()
{
	int maxp = -1;
	for (std::list<pin *>::iterator pi = connections_.begin(); pi != connections_.end(); pi++)
	{
		if ((*pi)->get_inout() == 0)// if output of a gate
		{
			int p = -1;
			if ((*pi)->get_gate()->get_type().compare("evl_dff") == 0 ||
				(*pi)->get_gate()->get_type().compare("evl_input") == 0 ||
				(*pi)->get_gate()->get_type().compare("evl_one") == 0 ||
				(*pi)->get_gate()->get_type().compare("evl_zero") == 0)
			{
				p = -1;
			}
			else if ((*pi)->get_gate()->get_type().compare("evl_clock") == 0)
			{
				p = -1;
			}
			else
                p = (*pi)->get_gate()->get_priority();
			if (p > maxp)// find max priority
			{
				maxp = p;
			}
		}

	}
	return	maxp + 1;
}

int	net::get_priority()
{
	if (bwp_)
	{
		return	priority_;
	}
	else
	{
		priority_ = ask_priority();
		bwp_ = true;
		return priority_;
	}
}



int	gate::get_priority()
{
	if (type_.compare("evl_dff") == 0 ||
		type_.compare("evl_input") == 0 || 
		type_.compare("evl_one") == 0 || 
		type_.compare("evl_zero") == 0)
	{
		return -1;
	}


	if (bwp_)
	{
		return	priority_;
	}
	else
	{
		priority_ = ask_priority();
		bwp_ = true;
		return priority_;
	}
}

int	gate::ask_priority()
{
	int maxp = -1;
	//every pin of the gate
	for (std::vector<pin *>::iterator pi = pins_.begin(); pi != pins_.end(); pi++)
	{
		if ((*pi)->get_inout() == 1)// if intput of a gate
		{
			//for every net in the pin
			std::list<net *> nets_temp = (*pi)->get_nets();
			for (std::list<net *>::iterator ei = nets_temp.begin(); ei != nets_temp.end(); ei++)
			{
				int p = (*ei)->get_priority();
				if (p > maxp)// find max priority
				{
					maxp = p;
				}
			}


		}

	}
	return	maxp + 1;
}




bool	net::get_bwchange()
{
	if ((next_value_ != value_) || (next_value1_ != value1_))
	{
		value1_ = next_value1_;
		value_ = next_value_;
		bwchange_ = true;
	}
	else
	{
		bwchange_ = false;
	}
	return		bwchange_;
}


void	net::update_value(bool input_value)
{
	next_value_ = input_value;
	next_value1_ = false;
	/*if (next_value_ != value_)
	{
		value_ = next_value_;
		bwchange_ = true;
	}
	else
	{
		bwchange_ = false;
	}*/

}

//for tris
void    net::update_value(bool input_value1, bool input_value)
{
    next_value1_ = input_value1;
    next_value_ = input_value;
    
    //pin * drive;
    //if(find_drive(drive)){// set value from drive
    //    
    //    if (drive->get_gate()->get_type().compare("buf")==0||
    //        drive->get_gate()->get_type().compare("tris")==0) {
    //        std::vector<bool> gate_output_temp;
    //        drive->get_gate()->compute_output(gate_output_temp);
    //        next_value1_ = gate_output_temp[1];
    //        next_value_ = gate_output_temp[0];
    //    }
    //    
    //}
    //else//if no drive, that is, all Z
    //{
    //    value1_ = 1;
    //    value_ = 1;
    //}
    
    
    
    //if ((next_value_ != value_) || (next_value1_ != value1_))
    //{
    //    value1_ = next_value1_;
    //    value_ = next_value_;
    //    bwchange_ = true;
    //}
    //else
    //{
    //    bwchange_ = false;
    //}
    
    
}


void    net::set_value(bool v1, bool v)
{
	value1_ = v1;
	value_ = v;
	set_comf();




    //pin * drive;
    //if(find_drive(drive)){// set value from drive


 /*       if (drive->get_gate()->get_type().compare("buf")==0||
            drive->get_gate()->get_type().compare("tris")==0) {
            std::vector<bool> gate_output_temp;
            drive->get_gate()->compute_output(gate_output_temp);
            value1_ = gate_output_temp[1];
            value_ = gate_output_temp[0];
			set_comf();
        }
        */
  //  }
  //  else
  //  {
		//set_comf();
  //      value1_ = 1;
  //      value_ = 1;
  //  }

}



void	net::get_values(std::vector<bool> &v)    //v[0] == value_;v[1] == value1
{
	if (bwcomf) {
		v.clear(); v.push_back(value_); v.push_back(value1_); assert(v.size() == 2);
	}
	else/*else ask compute, but not possible*/
	{
		std::cerr << "net: " << name_ << " is not comfirmed" << std::endl;
		assert(false);
	}

}

