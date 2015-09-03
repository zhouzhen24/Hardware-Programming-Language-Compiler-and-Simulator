//
//  buf_gate.h
//  Logic_Simulation
//
//  Created by Zhen Zhou on 11/23/14.
//  Copyright (c) 2014 Zhen Zhou. All rights reserved.
//

#ifndef __Logic_Simulation__buf_gate__
#define __Logic_Simulation__buf_gate__

#include "logic_gate.h"


class buf_gate: public logic_gate
{

	friend	class  port;

    bool state_, next_state_;
    buf_gate * clone(std::string name);

public:
    buf_gate(std::string name): logic_gate("buf", name), state_(false), next_state_(false) {}
    bool    validate_structural_semantics();
    static void store_prototype();
    virtual bool evaluate(const std::vector<bool> &inputs);
    virtual bool compute_output(std::vector<bool> &v);
};

#endif /* defined(__Logic_Simulation__buf_gate__) */
