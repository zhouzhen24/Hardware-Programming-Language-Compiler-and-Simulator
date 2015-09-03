#include	"netlist_main.h"

int	netlist_main(int argc, char* argv[])
{

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
    

    evl_wires_table wires_table = make_wires_table(wires);

	//correct pins' width
	if (!pin_semantic_update(wires_table, comps))   // if the pin connect to the whole bus, the msb=width-1 and lsb=0
	{
		return -1;
	}

    netlist nl;
    if (!nl.create(wires, comps, wires_table))
    {
        std::cerr<<"Cannot Create Netlist"<<std::endl;
        return -1;
    }
    
    std::string nl_file = std::string(argv[1])+".netlist";
    nl.save(nl_file, module.type); 
    
    
    return 0;
	
}




bool	pin_semantic_update(const evl_wires_table &wires_table, evl_components &comps)
{
	for (evl_components::iterator comp_index = comps.begin(); comp_index != comps.end(); ++comp_index)
	{
		//for each component
		for (evl_pins::iterator pin_index = (*comp_index).pins.begin(); pin_index != (*comp_index).pins.end(); ++pin_index)
		{
			//for each pin
			evl_wires_table::const_iterator	wi = wires_table.find((*pin_index).name);
			if ( wi == wires_table.end())
            {
                // if not find pin's name in wire_table, throw error
                std::cerr << (*comp_index).name << "'s pin " << (*pin_index).name << "must connect to a wire" << std::endl;
                return false;
            }
            else
			{
				if (((*pin_index).bus_lsb == -1) && ((*pin_index).bus_msb == -1) && ((*wi).second > 1))
				{
					(*pin_index).bus_lsb = 0;
					(*pin_index).bus_msb = (*wi).second - 1;
				}
                
                // if pin connects to a signal net of a bus, lsb = msb
                else if (((*pin_index).bus_lsb == -1) && ((*pin_index).bus_msb != -1))
                {
                    (*pin_index).bus_lsb = (*pin_index).bus_msb;
                }
                
                // if pin connects to a bus, width > msb = lsb = 0.
				else if (((*pin_index).bus_lsb >= (*wi).second)
                         || ((*pin_index).bus_msb >= (*wi).second)
                         || ((*pin_index).bus_msb < (*pin_index).bus_lsb))
				{
					std::cerr << (*comp_index).name << "'s pin:" << (*pin_index).name << "has wrong bus range" << std::endl;
					return	false;
				}
			}
		}
	}
	return	true;
}