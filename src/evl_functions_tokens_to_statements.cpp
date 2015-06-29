#include	"evl_data_structures_statement.h"
#include	<iostream>
#include	<fstream>
#include	<assert.h>
#include	"evl_functions_tokens_to_statements.h"

bool read_tokens_file(evl_tokens	&tokens, int argc, std::string file_name)
{
	if (argc < 2)
	{
		std::cerr << "You should provide a file name." << std::endl;
		return false;
	}
	std::ifstream input_file(file_name.c_str());
	if (!input_file)
	{
		std::cerr << "I can't read " << file_name.c_str() << "." << std::endl;
		return false;
	}

	std::string line;
	for (int line_no = 1; std::getline(input_file, line); ++line_no)//read a file
	{

		for (size_t i = 0; i < line.size();)//read a line
		{
			evl_token	token;
			token.line_no = line_no;
			//first word
			// spaces
			if (line[i] == ' ')//|| (line[i] == '\t')|| (line[i] == '\r') || (line[i] == '\n')
			{
				std::cerr << "wrong in .tokens file at line: " << line_no << std::endl;; // skip this space character
				return false; // skip the rest of the iteration
			}

			// type
			if ((line[i] == 'N') || (line[i] == 'S'))
			{
				size_t type_begin = i + 1;
				for (++i; i < line.size(); ++i)
				{
					if (line[i] == ' ')
					{
						++i;
						break; // [name_begin, i) is the range for the token
					}
				}
				if (line[type_begin] == 'I')
					token.type = evl_token::SINGLE;
				else if (line[type_begin] == 'A')
					token.type = evl_token::NAME;
				else if (line[type_begin] == 'U')
					token.type = evl_token::NUMBER;
				else
				{
					std::cerr << "Wrong Type in line: " << line_no << std::endl;
					return false;
				}
			}

			//second word

			// SINGLE
			if ((line[i] == '(') || (line[i] == ')')
				|| (line[i] == '[') || (line[i] == ']')
				|| (line[i] == ':') || (line[i] == ';')
				|| (line[i] == ','))
			{
				token.str = line[i];
				++i;
			}

			// NAME or NUMBER
			else
			{
				size_t str_begin = i;
				for (++i; i < line.size(); ++i)
				{
					if (line[i] == ' ')
					{
						break; // [name_begin, i) is the range for the token
					}
				}

				token.str = line.substr(str_begin, i - str_begin);
			}
			tokens.push_back(token);
		}
	}
	return true;


}


bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens)
{
	for (; !tokens.empty();)
	{ // generate one statement per iteration evl_token token = tokens.front();
		evl_token token = tokens.front();

		if (token.type != evl_token::NAME)
		{
			std::cerr << "Need a NAME token but found '" << token.str << "' on line " << token.line_no << std::endl;
			return false;
		}
		if (token.str == "module") // MODULE statement 
		{
			evl_statement module;
			module.type = evl_statement::MODULE;
			if (!move_tokens_to_statement(module.tokens, tokens))
				return false;
			statements.push_back(module);

		}
		else if (token.str == "endmodule") // ENDMODULE statement 
		{
			evl_statement endmodule;
			endmodule.type = evl_statement::ENDMODULE; endmodule.tokens.push_back(token);
			tokens.erase(tokens.begin()); statements.push_back(endmodule);


		}
		else if (token.str == "wire") // WIRE statement
		{
			evl_statement wire;
			wire.type = evl_statement::WIRE;
			if (!move_tokens_to_statement(wire.tokens, tokens))
				return false;
			statements.push_back(wire);
		}
        
        //ASSIGN statement
        else if (token.str == "assign"){
            evl_statement assign;
            assign.type = evl_statement::ASSIGN;
            if (!move_tokens_to_statement(assign.tokens, tokens))
                return false;
            statements.push_back(assign);
        }
        
        
        
		else // COMPONENT statement
		{
			evl_statement component;
			component.type = evl_statement::COMPONENT;
			if (!move_tokens_to_statement(component.tokens, tokens))
				return false;
			statements.push_back(component);
		}
	}






	return true;
}

bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens)
{
	for (; !tokens.empty();)
	{
		statement_tokens.push_back(tokens.front());
		tokens.erase(tokens.begin()); // consume one token per iteration 
		if (statement_tokens.back().str == ";")
			break; // exit if the ending ";" is found 
	}
	if (statement_tokens.back().str != ";")
	{
		std::cerr << "Look for ';' but reach the end of file" << std::endl;
		return false;
	}
	return true;
}


bool process_all_statements(evl_statements &statements, evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule)
{
	for (; !statements.empty(); statements.pop_front())
	{
		if ((statements.front().type) == (evl_statement::MODULE))
		{
			if (!process_module_statement(module, statements.front()))
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
		else
		{
			std::cerr << "Wrong Statmement Type" << std::endl;
			return false;
		}
	}
	return true;
}


bool process_module_statement(evl_module &module, evl_statement &s)
{
	evl_tokens::iterator	i = s.tokens.begin();
	assert(!s.tokens.empty());
	module.type = (*(++i)).str;
	return true;
}

bool process_wire_statement(evl_wires &wires, evl_statement &s)
{
	enum state_type { INIT, WIRE, DONE, WIRES, WIRE_NAME, BUS, BUS_MSB, BUS_COLON, BUS_LSB, BUS_DONE };
	state_type state = INIT;
	int	width = 1;//set default width
	for (; !s.tokens.empty() && (state != DONE); s.tokens.pop_front())
	{
		evl_token t = s.tokens.front();
		// use branches here to compute state transitions
		if (state == INIT)
		{
			if (t.str == "wire")
			{
				state = WIRE;
			}
			else
			{
				std::cerr << "Need 'wire' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
		else if (state == WIRE)
		{
			if (t.type == evl_token::NAME)
			{
				evl_wire wire;
				wire.name = t.str;
				wire.width = width;
				wires.push_back(wire);
				state = WIRE_NAME;
			}
			else if (t.str == "[")
			{
				state = BUS;
			}
			else
			{
				std::cerr << "Need NAME but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}

		}
		else if (state == WIRES)
		{

			if (t.type == evl_token::NAME)
			{
				evl_wire wire;
				wire.name = t.str;
				wire.width = width;
				wires.push_back(wire);
				state = WIRE_NAME;
			}
			else
			{
				std::cerr << "Need NAME but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}

		}
		else if (state == WIRE_NAME)
		{
			if (t.str == ",")
			{
				state = WIRES;
			}
			else if (t.str == ";")
			{
				state = DONE;
			}
			else
			{
				std::cerr << "Need ',' or ';' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}
		else if (state == BUS)
		{

			if (t.type == evl_token::NUMBER)
			{
				width = atoi(t.str.c_str());
				width++;

			}
			else
			{
				std::cerr << "Need Width but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
			state = BUS_MSB;
		}
		else if (state == BUS_MSB)
		{
			if (t.str != ":")
			{
				std::cerr << "Need ':' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
			state = BUS_COLON;
		}
		else if (state == BUS_COLON)
		{
			if (t.str != "0")
			{
				std::cerr << "Need '0' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
			state = BUS_LSB;
		}
		else if (state == BUS_LSB)
		{
			if (t.str != "]")
			{
				std::cerr << "Need ']' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
			state = BUS_DONE;
		}
		else if (state == BUS_DONE)
		{
			evl_wire	wire;
			wire.name = t.str;
			wire.width = width;
			wires.push_back(wire);
			state = WIRE_NAME;
		}
		else if (state == DONE)
		{
			break;
		}
		else
		{
			std::cerr << "Wire State Machine is Wrong" << std::endl;
			return false;
		}

	}
	if (!s.tokens.empty() || (state != DONE))
	{
		std::cerr << "something wrong with the statement" << std::endl;
		return false;
	}
	return true;



}

bool process_component_statement(evl_components &components, evl_statement &s)
{
	enum state_type { INIT, TYPE, NAME, PINS, PIN_NAME, BUS, BUS_MSB, BUS_LSB, BUS_COLON, BUS_DONE, PINS_DONE, DONE };
	state_type state = INIT;
	evl_component	comp;
	evl_pin	pin;
	for (; !s.tokens.empty() && state != DONE; s.tokens.pop_front())
	{
		evl_token t = s.tokens.front();


		//Enter state machine

		if (state == INIT)
		{
			if (t.type == evl_token::NAME)
			{
				comp.type = t.str;
				comp.name = "";
				state = TYPE;
			}
			else
			{
				std::cerr << "Need component type but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == TYPE)
		{
			if (t.type == evl_token::NAME)
			{
				comp.name = t.str;
				state = NAME;
			}
			else if (t.str == "(")
			{
				state = PINS;
			}
			else
			{
				std::cerr << "Need a name or '(' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == NAME)
		{
			if (t.str == "(")
			{
				state = PINS;
			}
			else
			{
				std::cerr << "Need '(' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == PINS)
		{

			if (t.type == evl_token::NAME)
			{
				pin.name = t.str;
				pin.bus_msb = -1;
				pin.bus_lsb = -1;
				state = PIN_NAME;
			}
			else
			{
				std::cerr << "Need a name but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == PIN_NAME)
		{
			if (t.str == "[")
			{
				state = BUS;
			}
			else if (t.str == ")")
			{
				comp.pins.push_back(pin);
				state = PINS_DONE;
			}
			else if (t.str == ",")
			{
				comp.pins.push_back(pin);
				state = PINS;
			}
			else
			{
				std::cerr << "Need '[', ')' or ',' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == PINS_DONE)
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

		else if (state == BUS)
		{
			if (t.type == evl_token::NUMBER)
			{
				pin.bus_msb = atoi(t.str.c_str());
				state = BUS_MSB;
			}
			else
			{
				std::cerr << "Need bus_msb but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == BUS_MSB)
		{
			if (t.str == ":")
			{
				state = BUS_COLON;
			}
			else if (t.str == "]")
			{
				state = BUS_DONE;
			}
			else
			{
				std::cerr << "Need ':' or ']' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == BUS_COLON)
		{
			if (t.type == evl_token::NUMBER)
			{
				pin.bus_lsb = atoi(t.str.c_str());
				state = BUS_LSB;
			}
			else
			{
				std::cerr << "Need bus_lsb but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == BUS_LSB)
		{
			if (t.str == "]")
			{
				state = BUS_DONE;
			}
			else
			{
				std::cerr << "Need ']' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

		else if (state == BUS_DONE)
		{
			if (t.str == ")")
			{
				comp.pins.push_back(pin);
				state = PINS_DONE;
			}
			else if (t.str == ",")
			{
				comp.pins.push_back(pin);
				state = PINS;
			}
			else
			{
				std::cerr << "Need ')' or ',' but found '" << t.str << "' on line " << t.line_no << std::endl;
				return false;
			}
		}

	}
	components.push_back(comp);
	return true;
}

bool process_endmodule_statement(evl_endmodule &endmodule, evl_statement &s)
{
	assert(!s.tokens.empty());
	endmodule.name = "ENDMODULE";
	return true;
}

bool output_syntax(char *argv[], evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule)
{


	std::string output_file_name = std::string(argv[1]) + ".syntax";
	std::ofstream output_file(output_file_name.c_str());
	if (!output_file)
	{
		std::cerr << "I can't write " << argv[1] << ".tokens ." << std::endl;
		return false;
	}

	if (!module.type.empty())//if .evl has a module, output its name
	{
		output_file << "module " << module.type << std::endl;
	}

	if (!wires.empty())//if .evl has wires, output names
	{
		output_file << "wires " << wires.size() << std::endl;
		for (evl_wires::iterator i = wires.begin(); i != wires.end(); ++i)
		{
			evl_wire	wire = *i;
			output_file << "  wire " << wire.name << " " << wire.width << std::endl;
		}
	}

	if (!components.empty())//if .evl has components, output pins, types and names
	{
		output_file << "components " << components.size() << std::endl;
		for (evl_components::iterator i = components.begin(); i != components.end(); ++i)
		{
			evl_component	component = *i;
			output_file << "  component " << component.type << " ";
			if (component.name != "")// component name is optional
				output_file << component.name << " ";
			output_file << component.pins.size() << std::endl;
			for (evl_pins::iterator j = component.pins.begin(); j != component.pins.end(); ++j)
			{
				evl_pin	pin = *j;
				output_file << "    pin " << pin.name;
				if (pin.bus_msb >= 0)
				{
					output_file << " " << pin.bus_msb;
				}
				if (pin.bus_lsb >= 0)
				{
					output_file << " " << pin.bus_lsb;
				}
				output_file << std::endl;
			}
		}
	}

	if (!endmodule.name.empty())//if .evl has endmodule, output it
	{
		output_file << "endmodule" << std::endl;
	}
	return true;
}


    
    
    
    
    
    
    
    
    



















