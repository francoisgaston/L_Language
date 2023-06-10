#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
int ProgramGrammarAction(const program_node* program_node);

// // Expresión.
// int AdditionExpressionGrammarAction(const int leftValue, const int rightValue);
// int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue);
// int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue);
// int DivisionExpressionGrammarAction(const int leftValue, const int rightValue);
// int FactorExpressionGrammarAction(const int columns);

// // Factores.
// int ExpressionFactorGrammarAction(const int columns);
// int ConstantFactorGrammarAction(const int columns);

// // Constantes.
// int IntegerConstantGrammarAction(const int columns);


//NOSOTROS
program_node * ProcessorAdditionAction(const processor_node * proc, const program_node * program);
program_node * ConnectionDefinitionAction(const input_node* input, const connection_node* conection);
processor_node * ProcessorDefinitionAction(const text_t identifier,const block_node* block_node);
block_node * MultiLineBlockDefinitionAction(const line_node* line_node,const block_node* block);
block_node * SingleLineBlockDefinitionAction(const line_node * line);
line_node * LocalVariableAssignmentAction(const text_t identifier,const operator_node* operator);
line_node * ExitVariableAssignmentAction(const exit_var_node* exit_var_node,const operator_node* operator);
exit_var_node * ExitVariableDefinitionAction(const number_t exit_var_number);
operator_node * BinaryOperationAction(const binary_operator_t binary_op, const argument_node * arg1, const argument_node * arg2);
operator_node * UnaryOperationAction(const unary_operator_t unary_op, const argument_node * arg);
argument_node * IdentifierArgumentAction(const text_t identifier);
argument_node * InputVariableArgumentAction(const number_t input_number);
argument_node * ConstantArgumentAction(const boolean_t value);
input_node * InputCountDefinitionAction(const number_t input_size);
connection_node * ConnectionBlockDefinitionAction(const arrow_node * arrow);
arrow_node * SingleIdentifierArrowAction(const text_t identifier, const arrow_node * arrow);
arrow_node * GroupIdentifierArrowAction(const group_node * group, const arrow_node * arrow);
arrow_node * OutputEndArrowAction();
arrow_node * IdentifierEndArrowAction(const text_t identifier, const new_line_arrow_node * newLineArrow);
arrow_node * GroupIdentifierEndArrowAction(const group_node * group,const new_line_arrow_node * newLineArrow);
new_line_arrow_node * InputNewLineArrowAction(const arrow_node * arrow);
new_line_arrow_node * IdentifierNewLineArrowAction(const text_t identifier, const arrow_node * arrow);
new_line_arrow_node * GroupIdentifierNewLineArrowAction(const group_node * group,const arrow_node * arrow);
group_node * GroupDefinitionAction(const text_t identifier, const group_aux_node * groupAux);
group_aux_node * GroupAuxDefinitionAction(const text_t identifier, const group_aux_node * groupAux);
group_aux_node * GroupAuxLastIdentifierAction(const text_t identifier);
#endif
