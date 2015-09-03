




#include	"syn.h"

bool	syn(int argc, char* argv[], evl_module& module, evl_wires& all_wires, evl_components& all_components, evl_endmodule& endmodule)
{
	if(!lex(argc, argv))
	{
		std::cerr << "Error in Lexcial Analysis" << std::endl;
		return false;
	}
	std::string	input_tokens = argv[1];
	input_tokens+=".tokens";
	evl_tokens	all_tokens;// to store all tokens from the .tokens file
	if (!read_tokens_file(all_tokens, argc, input_tokens))
	{
		std::cerr << "Cannot read tokens from .tokens file" << std::endl;
		return false;
	}
		
	evl_statements	all_statements;// to store all statements from all_tokens
	if (!group_tokens_into_statements(all_statements,all_tokens))
	{
		std::cerr << "Cannot group tokens into statements from structure 'all_tokens' " << std::endl;
		return false;
	}
	
	//evl_module		module;			// objects to store computer-readable statements
	//evl_wires		all_wires;
	//evl_components	all_components;
	//evl_endmodule	endmodule;

	if (!process_all_statements(all_statements, module, all_wires, all_components, endmodule))//process statements, store in objects
	{
		std::cerr << "Cannot convert statemetns to objects" << std::endl;
		return false;
	}

	if (!output_syntax(argv, module, all_wires, all_components, endmodule))
	{
		std::cerr << "Cannot write .syntax file" << std::endl;
		return false;
	}
	return true;
}