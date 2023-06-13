#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../backend/code-generation/symbol-table.h"

int getInputProcesor(const block_node* block_node);
int getOutputProcesor(const block_node* block_node);
boolean checkInputOutput(const input_node* input, const connection_node* conection);
unsigned int checkNewLineArrow(const new_line_arrow_node * new_line_arrow_node, unsigned  int in, unsigned int originalInput);
unsigned int checkGroupAuxVar(const group_aux_var_node* group_aux_var_node);
unsigned int checkSingleIdentifier(text_t identifier, unsigned int input);
unsigned int checkGroupIdentifier(const group_node* group_aux_node, unsigned  int in);
unsigned int checkGroupAux(const group_aux_node* group_aux_node, unsigned int  in);


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

// int FactorExpressionGrammarAction(const int columns) {
// 	LogDebug("\tFactorExpressionGrammarAction(%d)", columns);
// 	return columns;
// }

// int ExpressionFactorGrammarAction(const int columns) {
// 	LogDebug("\tExpressionFactorGrammarAction(%d)", columns);
// 	return columns;
// }

// int ConstantFactorGrammarAction(const int columns) {
// 	LogDebug("\tConstantFactorGrammarAction(%d)", columns);
// 	return columns;
// }

// int IntegerConstantGrammarAction(const int columns) {
// 	LogDebug("\tIntegerConstantGrammarAction(%d)", columns);
// 	return columns;
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
    if(!checkInputOutput(input, conection)){
        LogError("No coinciden input y output");
        exit(1);
    }
	program_node* ans = (program_node*) calloc(1,sizeof(program_node));
	ans->program_node_type = connection_node_type;
	ans->connection_node = conection;
	ans-> input_node = input;
	return ans;	
}

processor_node * ProcessorDefinitionAction(const text_t identifier,const block_node* block_node){
	LogDebug("ProcessorDefinitionAction(%p,%p)",identifier, block_node);
	//Generar y check de la variable en la tabla
    if(exists_proc_symbol_table(identifier.text)){
        LogError("Variable %s ya fue inicializada\n", identifier.text);
        exit(1);
    }
    int input = getInputProcesor(block_node);
    int output = getOutputProcesor(block_node);
    add_proc_symbol_table(identifier.text, input, output);
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
    if(!exists_variable_symbol_table(identifier.text)){
        add_variable_symbol_table(identifier.text);
    }
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
    if(!exists_variable_symbol_table(identifier.text)){
        LogError("Variable %s no fue declarada\n", identifier.text);
        exit(1);
    }
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
    if(!exists_proc_symbol_table(identifier.text)){
        LogError("Procesador %s no inicializado\n", identifier.text);
        exit(1);
    }
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

arrow_node * IdentifierEndArrowAction(const var_identifier_node * identifier, const new_line_arrow_node * newLineArrow) {
	LogDebug("IdentifierEndArrowAction(%s, %p)", identifier->identifier.text, newLineArrow);
    arrow_node * node = (arrow_node *) calloc(1, sizeof(arrow_node));
	node->identifier = identifier->identifier;
	node->new_line_arrow_node = newLineArrow;
	node->arrow_node_type = identifier_end_type;
	return node;
}
arrow_node * GroupIdentifierEndArrowAction(const group_var_node * group,const new_line_arrow_node * newLineArrow){
	LogDebug("GroupeIdentifierEndArrowAction(%p, %p)", group, newLineArrow);
	arrow_node * arrow = (arrow_node*) calloc(1, sizeof(arrow_node));
	arrow->arrow_node_type = group_identifier_end_type;
	arrow->new_line_arrow_node = newLineArrow;
	arrow->group_var_node = group;
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
    if(!exists_variable_symbol_table(identifier.text)){
        LogError("Variable %s no inicializada\n", identifier.text);
        exit(1);
    }
	new_line_arrow_node* new_line_arrow = (new_line_arrow_node*) calloc(1, sizeof(new_line_arrow_node*));
	new_line_arrow->new_line_arrow_node_type = input_argument_type;
	new_line_arrow->identifier = identifier;
	new_line_arrow->arrow_node = arrow;
	return new_line_arrow;
}
new_line_arrow_node * GroupIdentifierNewLineArrowAction(const group_var_node * group,const arrow_node * arrow){
	LogDebug("GroupIdentifierNewLineArrowAction(%p, %p)", group, arrow);
	new_line_arrow_node* new_line_arrow = (new_line_arrow_node*) calloc(1, sizeof(new_line_arrow_node));
	new_line_arrow->new_line_arrow_node_type = group_identifier_new_line_type;
	new_line_arrow->group_var_node=group;
	new_line_arrow->arrow_node=arrow;
	return new_line_arrow;
}
group_var_node * GroupDefinitionVariablesAction(const text_t identifier, const group_aux_var_node * groupAux){
    LogDebug("GroupDefinitionAction(%p, %p)", identifier, groupAux);
    group_var_node * group = (group_var_node*) calloc(1, sizeof(group_var_node));
    if(!exists_variable_symbol_table(identifier.text)){
        LogError("Variable %s no inicializada\n", identifier.text);
        exit(1);
    }
    add_variable_symbol_table(identifier.text);
    group->identifier = identifier;
    group->group_aux_node = groupAux;
    return group;
}
group_aux_var_node * GroupAuxDefinitionVariablesAction(const text_t identifier, const group_aux_var_node * groupAux){
    group_aux_var_node * group_aux = (group_aux_var_node*) calloc(1, sizeof(group_aux_var_node));
    add_variable_symbol_table(identifier.text);
    group_aux->group_aux_node_type = common_group_aux_type;
    group_aux->group_aux_node = groupAux;
    group_aux->identifier = identifier;
    return group_aux;
}
group_aux_var_node * GroupAuxLastIdentifierVariableAction(const text_t identifier){
    LogDebug("GroupAuxLastIdentifierVariableAction(%p)", identifier);
    add_variable_symbol_table(identifier.text);
    group_aux_var_node* group_aux = (group_aux_var_node*) calloc(1, sizeof(group_aux_var_node));
    group_aux->group_aux_node_type = last_group_aux_type;
    group_aux->identifier = identifier;
    return group_aux;
}
group_node * GroupDefinitionAction(const text_t identifier, const group_aux_node * groupAux){
	LogDebug("GroupDefinitionAction(%p, %p)", identifier, groupAux);
	group_node * group = (group_node*) calloc(1, sizeof(group_node));
    if(!exists_proc_symbol_table(identifier.text)){
        LogError("Procesador %s no inicializado\n", identifier.text);
        exit(1);
    }
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
var_identifier_node * newVariableIdentifier(const text_t identifier){
    LogDebug("newVariableIdentifier(%p)", identifier);
    var_identifier_node* group_aux = (var_identifier_node*) calloc(1, sizeof(var_identifier_node));
    add_variable_symbol_table(identifier.text);
    group_aux->identifier = identifier;
    return group_aux;
}

//Funciones auxiliares para validacion de conexiones
int getInputProcesor(const block_node* block_node){
    unsigned int in = 0;
    struct block_node aux;
    aux = *block_node;
    while(aux.block_node_type != single_line_block_type){
        if(aux.line_node->operator_node->argument_node_1->argument_node_type == input_argument_type
            && in <= aux.line_node->operator_node->argument_node_1->input_variable.n){
                //correcion de +1 por index de array e index de $x, (caso test05)
                in = aux.line_node->operator_node->argument_node_1->input_variable.n+1;
        }
        if(aux.line_node->operator_node->operator_node_type == binary_operator_type && aux.line_node->operator_node->argument_node_2->argument_node_type == input_argument_type
            && in <= aux.line_node->operator_node->argument_node_2->input_variable.n){
                in = aux.line_node->operator_node->argument_node_2->input_variable.n+1;
        }
        aux = *aux.block_node;
    }
    if(aux.line_node->operator_node->argument_node_1->argument_node_type == input_argument_type
        && in <= aux.line_node->operator_node->argument_node_1->input_variable.n){
            in = aux.line_node->operator_node->argument_node_1->input_variable.n+1;
    }
    if(aux.line_node->operator_node->operator_node_type == binary_operator_type
        && aux.line_node->operator_node->argument_node_2->argument_node_type == input_argument_type
        && in <= aux.line_node->operator_node->argument_node_2->input_variable.n){
            in = aux.line_node->operator_node->argument_node_2->input_variable.n+1;
    }
    return in;
}

int getOutputProcesor(const block_node* block_node){
    unsigned int out = 0;
    struct block_node aux = *block_node;
    while(aux.block_node_type != single_line_block_type){
        if(aux.line_node->line_node_type == exit_assigment_type
            && out <= aux.line_node->exit_var_node->exit_var_index.n){
                out = aux.line_node->exit_var_node->exit_var_index.n+1;
            }
        aux = *aux.block_node;
    }
    if(aux.line_node->line_node_type == exit_assigment_type
        && out <= aux.line_node->exit_var_node->exit_var_index.n){
            out = aux.line_node->exit_var_node->exit_var_index.n+1;
    }
    return out;
}

boolean checkInputOutput(const input_node* input, const connection_node* conection){
    unsigned  int in = input->integer.n;
    arrow_node aux  = *conection->arrow_node;
    while(aux.arrow_node_type != output_identifier_type){
        switch (aux.arrow_node_type) {
            case single_identifier_type:
                in = checkSingleIdentifier(aux.identifier, in);
                aux = *aux.arrow_node;
                break;
            case group_identifier_type:
                in = checkGroupIdentifier(aux.group_node, in);
                aux = *aux.arrow_node;
                break;
            case output_identifier_type:
                //Nunca deberia llegar
                break;
            case identifier_end_type:
                set_input_output_var(aux.identifier.text, in);
                in = checkNewLineArrow(aux.new_line_arrow_node, in, input->integer.n);
                aux = *aux.new_line_arrow_node->arrow_node;
                break;
            case group_identifier_end_type:
                //Nunca deberia llegar
                break;
        }
    }
    return true;
}

unsigned int checkSingleIdentifier(text_t identifier, unsigned int input){
    if(input < get_input(identifier.text)){
        return false;
    }
    return get_output(identifier.text);
}

unsigned int checkGroupIdentifier(const group_node * group_aux_node, unsigned  int in){
    if(in < get_input(group_aux_node->identifier.text)){
        return false;
    }
    in = get_output(group_aux_node->identifier.text);
    in += checkGroupAux(group_aux_node->group_aux_node, in);
    return in;
}

unsigned int checkGroupAux(const group_aux_node* group_aux_node, unsigned int  in){
    unsigned int out = 0;
    while(group_aux_node->group_aux_node_type != last_group_aux_type){
        if(in < get_input(group_aux_node->identifier.text)){
            return false;
        }
        out += get_output(group_aux_node->identifier.text);
        group_aux_node = group_aux_node->group_aux_node;
    }
    if(in < get_input(group_aux_node->identifier.text)){
        return false;
    }
    out += get_output(group_aux_node->identifier.text);
    return out;
}

unsigned int checkNewLineArrow(const new_line_arrow_node* new_line_arrow_node, unsigned  int in, unsigned int originalInput){
    switch (new_line_arrow_node->new_line_arrow_node_type) {
        case input_new_line_type:
            in = originalInput;
            break;
        case single_identifier_new_line_type:
            //OJO puede haber un error
            in = get_output(new_line_arrow_node->identifier.text);
            break;
        case group_identifier_new_line_type:
            in = get_output(new_line_arrow_node->group_var_node->identifier.text);
            in += checkGroupAuxVar(new_line_arrow_node->group_var_node->group_aux_node);
            break;
    }
    return in;
}

unsigned int checkGroupAuxVar(const group_aux_var_node* group_aux_var_node){
    unsigned out = 0;
    while(group_aux_var_node->group_aux_node_type != last_group_aux_type){
        out += get_output(group_aux_var_node->identifier.text);
        group_aux_var_node = group_aux_var_node->group_aux_node;
    }
    out += get_output(group_aux_var_node->identifier.text);
    return out;
}
