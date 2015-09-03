#include "xnor_gate.h"
#include "gate_prototypes.h"
#include "assert.h"

bool xnor_gate::validate_structural_semantics() {

	if (pins_.size() < 3)
		return false;

	pins_[0]->set_as_output();
	for (size_t i = 1; i < pins_.size(); ++i)
		pins_[i]->set_as_input();


	return true;
}

xnor_gate * xnor_gate::clone(std::string name)
{
	return new xnor_gate(name);
}

void xnor_gate::store_prototype()
{
	gate_prototypes::instance()->store("xnor", new xnor_gate("prototype"));
}

bool xnor_gate::evaluate(const std::vector<bool> &inputs)
{
	assert(inputs.size() >= 2);

	int count1 = 0;

	for (std::vector<bool>::const_iterator input_i = inputs.begin(); input_i != inputs.end(); input_i++) {
		if ((*input_i) == 1) {
			count1++;
		}
	}
	if (count1 % 2)//if it is odd
		return false;
	else
		return true;


}


