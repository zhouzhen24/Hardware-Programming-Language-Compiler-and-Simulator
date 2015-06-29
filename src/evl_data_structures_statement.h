#ifndef _EVL_DATA_STRUCTURES_STATEMENT
#define	_EVL_DATA_STRUCTURES_STATEMENT

#include	<string>
#include	<list>


struct  evl_wire;
typedef std::list<evl_wire> evl_wires;

struct  evl_port;
typedef std::list<evl_port> evl_ports;



//Tokens
struct evl_token
{
	enum token_type { NAME, SINGLE, NUMBER };


	token_type	type;
	std::string	str;
	int	line_no;

};
typedef	std::list<evl_token>	evl_tokens;

//Statements
struct evl_statement {
	enum statement_type { MODULE, WIRE, COMPONENT, ENDMODULE, ASSIGN };
	statement_type type;
	evl_tokens tokens;
};
typedef std::list<evl_statement> evl_statements;




//MODULE
struct evl_module
{
	std::string	type;
    
    
    //bonus
    evl_ports   ports;
};


//WIRES
struct evl_wire
{
	std::string name;
	int width;
};

//COMPONENTS
//PINS in COMPONENTS
struct evl_pin
{
	std::string	name;
	int	bus_msb;
	int bus_lsb;
};
typedef	std::list<evl_pin>	evl_pins;
struct evl_component
{
	std::string type;
	std::string name;
	evl_pins	pins;
};
typedef std::list<evl_component> evl_components;

//ENDMODULE
struct evl_endmodule
{
	std::string	name;
};




//ASSIGN
struct  evl_assign{
    
    evl_pin lhs;//left hand side
    evl_pin rhs;//right hand side
    
};
typedef std::list<evl_assign> evl_assigns;



//ports in module
struct  evl_port{
    std::string type;
    std::string name;
    int width;
    evl_wire   wire;
    
};




//-------------------------------------------------------------//

//data structure for statements

 // struct evl_statement


//-------------------------------------------------------------//

// data structure for module, wires, components and endmodule

//MODULE

//WIRES



//COMPONENTS
//PINS in COMPONENTS




#endif // !_EVL_DATA_STRUCTURES
