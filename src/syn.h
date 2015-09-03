#ifndef _SYN_FUNC
#define	_SYN_FUNC

#include	<iostream>
#include	<vector>
#include	<string>
#include	<fstream>
#include	<list>
#include	<assert.h>
#include	<stdlib.h>


#include	"evl_functions_tokens_to_statements.h"
#include	"evl_data_structures_statement.h"
#include	"lex.h"


bool	syn(int argc, char* argv[], evl_module& module, evl_wires& all_wires, evl_components& all_components, evl_endmodule& endmodule);

#endif // !_SYN_FUNC
