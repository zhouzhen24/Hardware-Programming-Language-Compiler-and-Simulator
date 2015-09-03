#pragma once
#include "logic_gate.h"
class xnor_gate : public logic_gate
{
	bool state_, next_state_;
	xnor_gate * clone(std::string name);

public:
	xnor_gate(std::string name) : logic_gate("xnor", name), state_(false), next_state_(false) {}
	bool    validate_structural_semantics();
	static void store_prototype();
	virtual bool evaluate(const std::vector<bool> &inputs);
};

