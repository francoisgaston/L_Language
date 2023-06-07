#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#define AND "AND"
#define OR "OR"
#define XOR "XOR"
#define NAND "NAND"
#define NOR "NOR"
#define XNOR "XNOR"
#define NOT "NOT"
#define BUFF "BUFF"
#define UNDEF_OP -1
#define TRUE_TEXT "True"
#define FALSE_TEXT "False"

// Emular tipo "booleanean".
typedef enum {
	false = 0,
	true = 1
} boolean;

typedef enum {
	AND_OP,
	OR_OP,
	XOR_OP,
	NAND_OP,
	NOR_OP, 
	XNOR_OP
} binary_operator;

typedef enum{
	NOT_OP,
	BUFF_OP
} unary_operator;

// El tipo de los tokens emitidos por Flex.
typedef int token;

typedef struct {
	boolean value;
	char * text;
} boolean_t;

typedef struct {
	unary_operator type;
	char * op;			// MUST BE FREED WHEN DISCARDED
} unary_operator_t;

typedef struct {
	binary_operator type;
	char * op;			// MUST BE FREED WHEN DISCARDED
} binary_operator_t;

typedef struct {
	unsigned int n;
	char * text;
} number_t;

typedef struct {
	char * text;		// MUST BE FREED WHEN DISCARDED
} text_t;


//Typedefs
typedef struct start_grammar_node start_grammar_node;
typedef struct program_node program_node;
typedef struct processor_node processor_node;
typedef struct input_node input_node;
typedef struct connection_node connection_node;
typedef struct block_node block_node;
typedef struct line_node line_node;
typedef struct exit_var_node exit_var_node;
typedef struct argument_node argument_node;
typedef struct operator_node operator_node;
typedef struct input_variable_node input_variable_node;
typedef struct constant_variable_node constant_variable_node;
typedef struct arrow_node arrow_node;
typedef struct new_line_arrow_node new_line_arrow_node;
typedef struct group_aux_node group_aux_node;
typedef struct group_node group_node;


//Type of nodes
typedef enum{
	single_identifier_type,
	group_identifier_type,
	output_identifier_type,
	identifier_end_type,
	group_identifier_end_type
}arrow_node_type;

typedef enum{
	binary_operator_type,
	unary_operator_type
}operator_node_type;

typedef enum{
	local_assigment_type,
	exit_assigment_type
}line_node_type; 

typedef enum {
	processor_node_type,
	connection_node_type
} program_node_type;

typedef enum{
	multi_line_block_type,
	single_line_block_type
} block_node_type;

typedef enum{
	identifier_argument_type,
	input_argument_type,
	constant_argument_type,
} argument_node_type;

typedef enum{
	input_new_line_type,
	single_identifier_new_line_type,
	group_identifier_new_line_type
} new_line_arrow_node_type;

typedef enum{
	common_group_aux_type,
	last_group_aux_type
}group_aux_node_type;

struct start_grammar_node{
	const program_node* program_node;
};

struct program_node{
	program_node_type program_node_type;
	const processor_node* processor_node;
	const program_node* program_node;
	const input_node* input_node;
	const connection_node* connection_node;
};

struct processor_node{
	text_t identifier;
	const block_node* block_node;
};

struct block_node{
	block_node_type block_node_type;
	const line_node* line_node;
	const block_node* block_node;
};

struct line_node{
	line_node_type line_node_type;
	text_t identifier;
	const operator_node* operator_node;
	const exit_var_node* exit_var_node;
};

struct exit_var_node{
	number_t exit_var_index;
};

struct operator_node{
	operator_node_type operator_node_type;
	binary_operator_t binary_operator;
	const argument_node* argument_node_1;
	const argument_node* argument_node_2;
	unary_operator_t unary_operator;
};

struct argument_node{
	argument_node_type argument_node_type;
	number_t input_variable;
	boolean_t constant_value;
	text_t identifier;
};

struct input_node{
	number_t integer;
};

struct connection_node{
	const arrow_node* arrow_node;
};

struct arrow_node{
	arrow_node_type arrow_node_type;
	text_t identifier;
	const arrow_node* arrow_node;
	const group_node* group_node;
	const new_line_arrow_node* new_line_arrow_node;
};

struct new_line_arrow_node{
	new_line_arrow_node_type new_line_arrow_node_type;
	const arrow_node* arrow_node;
	text_t identifier;
	const group_node* group_node;
};

struct group_node{
	text_t identifier;
	const group_aux_node* group_aux_node;
};

struct group_aux_node{
	group_aux_node_type group_aux_node_type;
	text_t identifier;
	const group_aux_node* group_aux_node;
};


// /**
// * Se realiza este tipo de definiciones cuando el tipo de dato debe
// * auto-referenciarse, como es el caso de una "Expression", que está compuesta
// * de otras 2 expresiones.
// */
// typedef struct Expression Expression;

// /**
// * Para cada no-terminal se define una nueva estructura que representa su tipo
// * de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
// */
// typedef struct {
// 	int value;
// } Constant;

// /**
// * En caso de que un no-terminal ("Factor" en este caso), posea más de una
// * regla de producción asociada, se crea además de la estructura un enumerado
// * que identitifque cada reducción posible (cada regla de producción). Luego,
// * se agrega una instancia de este enumerado dentro del nodo del no-terminal.
// *
// * De este modo, al recorrer el AST, es posible determinar qué nodos hijos
// * posee según el valor de este enumerado.
// */
// typedef enum {
// 	EXPRESSION,
// 	CONSTANT
// } FactorType;

// typedef struct {
// 	FactorType type;
// 	Expression * expression;
// } Factor;

// typedef enum {
// 	ADDITION,
// 	SUBTRACTION,
// 	MULTIPLICATION,
// 	DIVISION,
// 	FACTOR
// } ExpressionType;

// struct Expression {
// 	ExpressionType type;
// 	Expression * leftExpression;
// 	Expression * rightExpression;
// };

// typedef struct {
// 	Expression * expression;
// } Program;


#endif
