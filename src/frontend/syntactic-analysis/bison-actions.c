#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * Implementación de "bison-grammar.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
	LogError("En ASCII es:");
	LogErrorRaw("\t");
	const int length = strlen(yytext);
	for (int i = 0; i < length; ++i) {
		LogErrorRaw("[%d]", yytext[i]);
	}
	LogErrorRaw("\n\n");
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
int ProgramGrammarAction(const program_node* program_node) {
	LogDebug("\tProgramGrammarAction(%p)", program_node);
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	state.program_node = program_node;
	return 0;
}

// int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tAdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Add(leftValue, rightValue);
// }

// int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tSubtractionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Subtract(leftValue, rightValue);
// }

// int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tMultiplicationExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Multiply(leftValue, rightValue);
// }

// int DivisionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tDivisionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Divide(leftValue, rightValue);
// }

// int FactorExpressionGrammarAction(const int value) {
// 	LogDebug("\tFactorExpressionGrammarAction(%d)", value);
// 	return value;
// }

// int ExpressionFactorGrammarAction(const int value) {
// 	LogDebug("\tExpressionFactorGrammarAction(%d)", value);
// 	return value;
// }

// int ConstantFactorGrammarAction(const int value) {
// 	LogDebug("\tConstantFactorGrammarAction(%d)", value);
// 	return value;
// }

// int IntegerConstantGrammarAction(const int value) {
// 	LogDebug("\tIntegerConstantGrammarAction(%d)", value);
// 	return value;
// }



//NOSOTROS

program_node* ProcessorAdditionAction(const processor_node * proc, const program_node * program){
	LogDebug("\tProcessorAdditionAction(%p,%p)", proc, program);
	program_node* ans = (program_node*) calloc(1,sizeof(program_node));
	ans->program_node_type = processor_node_type;
	ans->processor_node = proc;
	ans-> program_node = program;
	return ans;
}

program_node * ConnectionDefinitionAction(const input_node* input, const connection_node* conection){
	LogDebug("\tConnectionDefinitionAction(%p, %p)", input, conection);
	program_node* ans = (program_node*) calloc(1,sizeof(program_node));
	ans->program_node_type = connection_node_type;
	ans->connection_node = conection;
	ans-> input_node = input;
	return ans;	
}

processor_node * ProcessorDefinitionAction(const text_t identifier,const block_node* block_node){
	LogDebug("ProcessorDefinitionAction(%p,%p)",identifier, block_node);
	//Generar y check de la variable en la tabla
	processor_node* ans = (processor_node*) calloc(1,sizeof(processor_node));
	ans->block_node = block_node;
	ans->identifier = identifier;
	return ans;
}
block_node * MultiLineBlockDefinitionAction(const line_node* line_node,const block_node* block){
	LogDebug("MultiLineBlockDefinitionAction(%p, %p)", line_node, block);
	block_node* ans = (block_node*) calloc(1,sizeof(block_node));
	ans->block_node_type = multi_line_block_type;
	ans->line_node = line_node;
	ans->block_node = block;
	return ans;
}
block_node * SingleLineBlockDefinitionAction(const line_node * line){
	LogDebug("SingleLineBlockDefinitionAction(%p)", line);
	block_node* ans = (block_node*) calloc(1,sizeof(block_node));
	ans->block_node_type = single_line_block_type;
	ans->line_node = line;
	return ans;
}
line_node * LocalVariableAssignmentAction(const text_t identifier,const operator_node* operator){
	LogDebug("LocalVariableAssignmentAction(%p, %p)", identifier, operator);
	line_node* ans = (line_node*) calloc(1,sizeof(line_node));
	ans->line_node_type = local_assigment_type;
	ans->identifier = identifier;
	ans->operator_node = operator;
	return ans;
}
line_node * ExitVariableAssignmentAction(const exit_var_node* exit_var_node,const operator_node* operator){
	LogDebug("ExitVariableAssignmentAction(%p, %p)", exit_var_node, operator);
	line_node* ans = (line_node*) calloc(1,sizeof(line_node));
	ans->line_node_type = exit_assigment_type;
	ans->operator_node = operator;
	ans->exit_var_node = exit_var_node;
	return ans;
}
exit_var_node * ExitVariableDefinitionAction(const number_t exit_var_number){
	LogDebug("ExitVariableDefinitionAction(%d)", exit_var_number);
	exit_var_node* ans = (exit_var_node*) calloc(1,sizeof(exit_var_node));
	ans->exit_var_index = exit_var_number;
	return ans;
}

operator_node * BinaryOperationAction(const binary_operator_t binary_op, const argument_node * arg1, const argument_node * arg2) {
	LogDebug("BinaryOperationAction(%s, %p, %p)", binary_op.op, arg1, arg2);
	if(binary_op.type == UNDEF_OP) {
		LogError("UnknownBinaryOperator: %s", binary_op.op);
	}
	operator_node * op_node = (operator_node *) calloc(1, sizeof(operator_node));
	op_node->operator_node_type = binary_operator_type;
	op_node->binary_operator = binary_op;
	op_node->argument_node_1 = arg1;
	op_node->argument_node_2 = arg2;
	return op_node;
}

operator_node * UnaryOperationAction(const unary_operator_t unary_op, const argument_node * arg) {
	LogDebug("UnaryOperationAction(%s, %p)", unary_op.op, arg);
	if(unary_op.type == UNDEF_OP) {
		LogError("UnknownUnaryOperator: %s", unary_op.op);
	}
	operator_node * op_node = (operator_node *) calloc(1, sizeof(operator_node));
	op_node->operator_node_type = unary_operator_type;
	op_node->unary_operator = unary_op;
	op_node->argument_node_1 = arg;
	return op_node;
}

argument_node * IdentifierArgumentAction(const text_t identifier){
	LogDebug("IdentifierArgumentAction(%p)", identifier);
	argument_node* ans = (argument_node*) calloc(1,sizeof(argument_node));
	ans->argument_node_type = identifier_argument_type;
	ans->identifier = identifier;
	return ans;
}

argument_node * InputVariableArgumentAction(const number_t input_number) {
	LogDebug("InputVariableArgumentAction(%d)", input_number);
	argument_node * a_node = (argument_node *) calloc(1, sizeof(argument_node));
	a_node->argument_node_type = input_argument_type;
	a_node->input_variable = input_number;
	return a_node;
}

argument_node * ConstantArgumentAction(const boolean_t value){
	LogDebug("ConstantArgumentAction(%d)", value.value);
	argument_node* ans = (argument_node*) calloc(1,sizeof(argument_node));
	ans->argument_node_type = constant_argument_type;
	ans->constant_value = value;
	return ans;
}

input_node * InputCountDefinitionAction(const number_t input_size){
	LogDebug("InputCountDefinitionAction(%d)", input_size.n);
	input_node* ans = (input_node*) calloc(1,sizeof(input_node));
	ans->integer = input_size;
	return ans;
}

connection_node * ConnectionBlockDefinitionAction(const arrow_node * arrow) {
	LogDebug("ConnectionBlockDefinitionAction(%p)", arrow);
	connection_node * node = (connection_node *) calloc(1, sizeof(connection_node));
	node->arrow_node = arrow;
	return node;
}

arrow_node * SingleIdentifierArrowAction(const text_t identifier, const arrow_node * arrow) {
	LogDebug("SingleIdentifierArrowAction(%s, %p)", identifier.text, arrow);
	arrow_node * a_node = (arrow_node *) calloc(1, sizeof(arrow_node));
	a_node->identifier = identifier;
	a_node->arrow_node = arrow;
	a_node->arrow_node_type = single_identifier_type;
	return a_node;
}	

arrow_node * GroupIdentifierArrowAction(const group_node * group, const arrow_node * arrow) {
	LogDebug("GroupIdentifierArrowAction(%p, %p)", group, arrow);
	arrow_node * a_node = (arrow_node *) calloc(1, sizeof(arrow_node));
	a_node->arrow_node = arrow;
	a_node->group_node = group;
	a_node->arrow_node_type = group_identifier_type;
	return a_node;
}

arrow_node * OutputEndArrowAction() {
	LogDebug("OutputEndArrowAction()");
	arrow_node * node = (arrow_node *) calloc(1, sizeof(arrow_node));
	node->arrow_node_type = output_identifier_type;
	return node;
}

arrow_node * IdentifierEndArrowAction(const text_t identifier, const new_line_arrow_node * newLineArrow) {
	LogDebug("IdentifierEndArrowAction(%s, %p)", identifier.text, newLineArrow);
	arrow_node * node = (arrow_node *) calloc(1, sizeof(arrow_node));
	node->identifier = identifier;
	node->new_line_arrow_node = newLineArrow;
	node->arrow_node_type = identifier_end_type;
	return node;
}
arrow_node * GroupIdentifierEndArrowAction(const group_node * group,const new_line_arrow_node * newLineArrow){
	LogDebug("GroupeIdentifierEndArrowAction(%p, %p)", group, newLineArrow);
	arrow_node * arrow = (arrow_node*) calloc(1, sizeof(arrow_node));
	arrow->arrow_node_type = group_identifier_end_type;
	arrow->new_line_arrow_node = newLineArrow;
	arrow->group_node= group;
	return arrow;
}
new_line_arrow_node * InputNewLineArrowAction(const arrow_node * arrow){
	LogDebug("InputNewLineArrowAction(%p)", arrow);
	new_line_arrow_node * new_line_arrow = (new_line_arrow_node*) calloc(1, sizeof(new_line_arrow_node*));
	new_line_arrow->new_line_arrow_node_type = input_new_line_type;
	new_line_arrow->arrow_node = arrow;
	return new_line_arrow;
}
new_line_arrow_node * IdentifierNewLineArrowAction(const text_t identifier, const arrow_node * arrow){
	LogDebug("IdentifierNewLineArrowAction(%p, %p)", identifier, arrow);
	new_line_arrow_node* new_line_arrow = (new_line_arrow_node*) calloc(1, sizeof(new_line_arrow_node*));
	new_line_arrow->new_line_arrow_node_type = input_argument_type;
	new_line_arrow->identifier = identifier;
	new_line_arrow->arrow_node = arrow;
	return new_line_arrow;
}
new_line_arrow_node * GroupIdentifierNewLineArrowAction(const group_node * group,const arrow_node * arrow){
	LogDebug("GroupIdentifierNewLineArrowAction(%p, %p)", group, arrow);
	new_line_arrow_node* new_line_arrow = (new_line_arrow_node*) calloc(1, sizeof(new_line_arrow_node));
	new_line_arrow->new_line_arrow_node_type = group_identifier_new_line_type;
	new_line_arrow->group_node=group;
	new_line_arrow->arrow_node=arrow;
	return new_line_arrow;
}
group_node * GroupDefinitionAction(const text_t identifier, const group_aux_node * groupAux){
	LogDebug("GroupDefinitionAction(%p, %p)", identifier, groupAux);
	group_node * group = (group_node*) calloc(1, sizeof(group_node));
	group->identifier = identifier;
	group->group_aux_node = groupAux;
	return group;
}
group_aux_node * GroupAuxDefinitionAction(const text_t identifier, const group_aux_node * groupAux){
	group_aux_node * group_aux = (group_aux_node*) calloc(1, sizeof(group_aux_node));
	group_aux->group_aux_node_type = common_group_aux_type;
	group_aux->group_aux_node = groupAux;
	group_aux->identifier = identifier;
	return group_aux;
}
group_aux_node * GroupAuxLastIdentifierAction(const text_t identifier){
	LogDebug("ProcessorAdditionAction(%p)", identifier);
	group_aux_node* group_aux = (group_aux_node*) calloc(1, sizeof(group_aux_node));
	group_aux->group_aux_node_type = last_group_aux_type;
	group_aux->identifier = identifier;
	return group_aux;
}
