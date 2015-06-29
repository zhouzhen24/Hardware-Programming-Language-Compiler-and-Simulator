//
//  director_pattern.cpp
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/14/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#include "evl_director.h"

void	update_pin_msb_lsb(evl_wires_table::const_iterator wi, evl_pin &p);

void evl_director(netlist_builder *builder, const evl_wires &wires, const evl_components &comps)
{
    //for each wire w in wires
    for (evl_wires::const_iterator wi = wires.begin(); wi != wires.end(); wi++)
    {
        evl_wire    w = (*wi);
        //for each net w[i] in w
        for (int i = 0; i < w.width; i++)
        {
            std::string net_name;
            if(w.width == 1)
                builder->add_net(w.name);
                //net_name = make_net_name(w.name, i);
            else
                builder->add_net(make_net_name(w.name, i));
        }
    }
    
    //create wires_table
    evl_wires_table wires_table = make_wires_table(wires);
    
    //for each component c in comps
    for (evl_components::const_iterator ci = comps.begin(); ci != comps.end(); ci++)
    {
        evl_component   c = (*ci);
		//bonus
		//for assign
		if (c.type.compare("assign") == 0)
		{
			//for each pin// only two pins
			evl_pin pin_lhs = c.pins.front();
			evl_pin pin_rhs = c.pins.back();

			evl_wires_table::const_iterator	wl = wires_table.find(pin_lhs.name);
			assert(wl != wires_table.end());//pin_lhs.name is in wires_table
			update_pin_msb_lsb(wl, pin_lhs);
			int	lhs_width = (*wl).second;

			evl_wires_table::const_iterator	wr = wires_table.find(pin_rhs.name);
			assert(wr != wires_table.end());//pin_rhs.name is in wires_table
			update_pin_msb_lsb(wr, pin_rhs);
			int	rhs_width = (*wr).second;

			if (lhs_width == 1 || rhs_width == 1)//if one of pins is not bus,
			{
				size_t gate_id = builder->add_gate("buf", "");
				builder->gate_append_pin(gate_id, pin_lhs.name, pin_lhs.bus_lsb, pin_lhs.bus_msb);
				builder->gate_append_pin(gate_id, pin_rhs.name, pin_rhs.bus_lsb, pin_rhs.bus_msb);
			}

			else//if both are buses, create (multiple) buf(s)
			{
				assert((pin_lhs.bus_msb - pin_lhs.bus_lsb) == (pin_rhs.bus_msb - pin_rhs.bus_lsb));
				int width = pin_lhs.bus_msb - pin_lhs.bus_lsb + 1;
				
				for (int i = 0; i < width; i++)
				{
					evl_pin	lhs_temp;
					evl_pin	rhs_temp;

					lhs_temp.name = pin_lhs.name;
					rhs_temp.name = pin_rhs.name;

					lhs_temp.bus_lsb = pin_lhs.bus_lsb + i;
					rhs_temp.bus_lsb = pin_rhs.bus_lsb + i;

					lhs_temp.bus_msb = lhs_temp.bus_lsb;
					rhs_temp.bus_msb = rhs_temp.bus_lsb;

					size_t gate_id = builder->add_gate("buf", "");
					builder->gate_append_pin(gate_id, lhs_temp.name, lhs_temp.bus_lsb, lhs_temp.bus_msb);
					builder->gate_append_pin(gate_id, rhs_temp.name, rhs_temp.bus_lsb, rhs_temp.bus_msb);
				}
			}

		}

		else{// if not assign


			size_t gate_id = builder->add_gate(c.type, c.name);
			//for each pin p in c.pins
			for (evl_pins::iterator pi = c.pins.begin(); pi != c.pins.end(); pi++)
			{
				evl_pin p = (*pi);
				//resolve semantics of p using wires_table
				evl_wires_table::const_iterator	wi = wires_table.find(p.name);

				//test
				if (wi == wires_table.end())
				{
					assert(wi != wires_table.end());
				}
				assert(wi != wires_table.end());//p.name is in wires_table


				if ((p.bus_lsb == -1) && (p.bus_msb == -1))
				{
					if ((*wi).second > 1)//connect to whole bus
					{
						p.bus_lsb = 0;
						p.bus_msb = (*wi).second - 1;
					}
				}
				// if pin connects to a signal net of a bus, lsb = msb
				else if ((p.bus_lsb == -1) && (p.bus_msb != -1))
				{
					p.bus_lsb = p.bus_msb;
				}
				// if pin connects to a bus, width > msb ≥ lsb ≥ 0.
				else
					assert(((*wi).second > p.bus_msb) && (p.bus_msb >= p.bus_lsb) && (p.bus_lsb >= 0));
				//resolve semantics of p ends

				builder->gate_append_pin(gate_id, p.name, p.bus_lsb, p.bus_msb);
			}
		}
    }
    
}



void	update_pin_msb_lsb(evl_wires_table::const_iterator wi, evl_pin &p)
{
	if ((p.bus_lsb == -1) && (p.bus_msb == -1))
	{
		if ((*wi).second > 1)//connect to whole bus
		{
			p.bus_lsb = 0;
			p.bus_msb = (*wi).second - 1;
		}
	}
	// if pin connects to a signal net of a bus, lsb = msb
	else if ((p.bus_lsb == -1) && (p.bus_msb != -1))
	{
		p.bus_lsb = p.bus_msb;
	}
	// if pin connects to a bus, width > msb ≥ lsb ≥ 0.
	else
		assert(((*wi).second > p.bus_msb) && (p.bus_msb >= p.bus_lsb) && (p.bus_lsb >= 0));



}






