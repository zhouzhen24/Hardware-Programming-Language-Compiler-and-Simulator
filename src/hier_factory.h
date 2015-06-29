//
//  hier_factory.h
//  Project_Bonus_ECE449
//
//  Created by Zhen Zhou on 12/2/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Project_Bonus_ECE449__hier_factory__
#define __Project_Bonus_ECE449__hier_factory__

#include "netlist_factory.h"





class hier_factory : public netlist_factory_i
{
    virtual gate *make_gate(std::string type, std::string name);
    virtual net *make_net();
    virtual pin *make_pin();
}; // class hier_factory




#endif /* defined(__Project_Bonus_ECE449__hier_factory__) */
