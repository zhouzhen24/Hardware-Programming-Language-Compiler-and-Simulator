



#ifndef _EVL_FUNCTIONS_TOKENS_TO_STATEMENTS
#define	_EVL_FUNCTIONS_TOKENS_TO_STATEMENTS

#include	"evl_data_structures_statement.h"


//--------------------------------------------------------//
//functions to process tokens and statements

//function to read tokens from .tokens file
//argv[] is .tokens file
bool	read_tokens_file(evl_tokens	&tokens, int argc, std::string file_name);

//functions to group tokens into statements
bool group_tokens_into_statements(evl_statements &statements, evl_tokens &tokens);
bool move_tokens_to_statement(evl_tokens &statement_tokens, evl_tokens &tokens);// called by group_tokens_into_statements()

//functions to process module, wires, components and endmodule
bool process_all_statements(evl_statements &statements, evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule);
bool process_module_statement(evl_module &module, evl_statement &s);
bool process_wire_statement(evl_wires &wires, evl_statement &s);
bool process_component_statement(evl_components &components, evl_statement &s);
bool process_endmodule_statement(evl_endmodule &endmodule, evl_statement &s);





//function to write .syntax file
bool output_syntax(char* argv[], evl_module &module, evl_wires &wires, evl_components &components, evl_endmodule &endmodule);

#endif // !_EVL_FUNCTIONS_TOKENS_TO_STATEMENTS
