%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	int start_grammar;
	program_node * program;
	input_node* input;
	processor_node* proc;
	block_node * block;
	line_node * line;
	exit_var_node * exit_var;
	operator_node * operator;
	argument_node * argument;
	connection_node * connection;
	arrow_node * arrow;
	new_line_arrow_node * new_line_arrow;
	group_node * group;
	group_aux_node * group_aux;

	// Terminales.
	token token;
	boolean_t boolean;
	unary_operator_t unary_operator;
	binary_operator_t binary_operator;
	text_t identifier;
	number_t number;
}

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> PROCESSOR

%token <token> INPUT
%token <token> OUTPUT

%token <boolean> TRUE
%token <boolean> FALSE

%token <unary_operator> UNARY_OP
%token <binary_operator> BINARY_OP
%token <token> ASSIGNMENT_OP
%token <token> ARROW_OP

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

%token <token> OPEN_BRACES
%token <token> CLOSE_BRACES

%token <token> COMMA
%token <token> SEMICOLON

%token <token> DOLLAR_SIGN
%token <token> HASH_SIGN

%token <identifier> IDENTIFIER
%token <number> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <start_grammar> start_grammar
%type <program> program
%type <proc> proc
%type <input> input
%type <block> block
%type <line> line
%type <exit_var> exit_var
%type <operator> operator
%type <argument> argument
%type <connection> connection
%type <arrow> arrow
%type <new_line_arrow> new_line_arrow
%type <group> group
%type <group_aux> group_aux



// Reglas de asociatividad y precedencia (de menor a mayor).
//%left ADD SUB
//%left MUL DIV

// El símbolo inicial de la gramatica.
%start start_grammar

%%
start_grammar: program 					{$$ = ProgramGrammarAction($1);}
	;

program: proc program 						{ $$ = ProcessorAdditionAction($1,$2); }
		| input connection 					{ $$ = ConnectionDefinitionAction($1, $2);}
	;

proc: PROCESSOR IDENTIFIER OPEN_BRACES block CLOSE_BRACES SEMICOLON			{ $$ = ProcessorDefinitionAction($2,$4); }
	;
	
block: line block 	{ $$ = MultiLineBlockDefinitionAction($1,$2);}
		| line	   	{ $$ = SingleLineBlockDefinitionAction($1);}	
	;

line: IDENTIFIER ASSIGNMENT_OP operator SEMICOLON 	{ $$ = LocalVariableAssignmentAction($1,$3);}
	| exit_var ASSIGNMENT_OP operator SEMICOLON 	{ $$ = ExitVariableAssignmentAction($1,$3);}
	;

exit_var: HASH_SIGN INTEGER  	{ $$ = ExitVariableDefinitionAction($2);}
	;

operator: BINARY_OP OPEN_PARENTHESIS argument COMMA argument CLOSE_PARENTHESIS 	{ $$ = BinaryOperationAction($1, $3, $5);}
		| UNARY_OP OPEN_PARENTHESIS argument CLOSE_PARENTHESIS 					{ $$ = UnaryOperationAction($1,$3);}
	;

argument: IDENTIFIER  			{ $$ = IdentifierArgumentAction($1);}
		| DOLLAR_SIGN INTEGER  	{ $$ = InputVariableArgumentAction($2);}
		| TRUE 					{ $$ = ConstantArgumentAction($1);}
		| FALSE					{ $$ = ConstantArgumentAction($1);}
	;


input: INPUT OPEN_PARENTHESIS INTEGER CLOSE_PARENTHESIS SEMICOLON  	{ $$ = InputCountDefinitionAction($3);}
	;

connection: INPUT arrow			{ $$ = ConnectionBlockDefinitionAction($2);}
	;

arrow: ARROW_OP IDENTIFIER arrow  										{ $$ = SingleIdentifierArrowAction($2, $3);}
	| ARROW_OP OPEN_BRACES group CLOSE_BRACES arrow 					{ $$ = GroupIdentifierArrowAction($3,$5);}
	| ARROW_OP OUTPUT SEMICOLON											{ $$ = OutputEndArrowAction();}
	| ARROW_OP IDENTIFIER SEMICOLON new_line_arrow						{ $$ = IdentifierEndArrowAction($2,$4);}
	| ARROW_OP OPEN_BRACES group CLOSE_BRACES SEMICOLON new_line_arrow	{ $$ = GroupIdentifierEndArrowAction($3,$6);}
	;

new_line_arrow: INPUT arrow 					{ $$ = InputNewLineArrowAction($2); }
	| IDENTIFIER arrow							{ $$ = IdentifierNewLineArrowAction($1, $2);}
	| OPEN_BRACES group CLOSE_BRACES arrow		{ $$ = GroupIdentifierNewLineArrowAction($2,$4);}
	;

group: IDENTIFIER COMMA group_aux				{ $$ = GroupDefinitionAction($1, $3);}
	;

group_aux: IDENTIFIER COMMA group_aux			{ $$ = GroupAuxDefinitionAction($1, $3);}
	| IDENTIFIER								{ $$ = GroupAuxLastIdentifierAction($1);}
	;



/*
expression: expression[left] ADD expression[right]					{ $$ = AdditionExpressionGrammarAction($left, $right); }
	| expression[left] SUB expression[right]						{ $$ = SubtractionExpressionGrammarAction($left, $right); }
	| expression[left] MUL expression[right]						{ $$ = MultiplicationExpressionGrammarAction($left, $right); }
	| expression[left] DIV expression[right]						{ $$ = DivisionExpressionGrammarAction($left, $right); }
	| factor														{ $$ = FactorExpressionGrammarAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorGrammarAction($2); }
	| constant														{ $$ = ConstantFactorGrammarAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantGrammarAction($1); }
	;
*/

%%
