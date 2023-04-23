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
int ProgramGrammarAction(const int value);

// // Expresión.
// int AdditionExpressionGrammarAction(const int leftValue, const int rightValue);
// int SubtractionExpressionGrammarAction(const int leftValue, const int rightValue);
// int MultiplicationExpressionGrammarAction(const int leftValue, const int rightValue);
// int DivisionExpressionGrammarAction(const int leftValue, const int rightValue);
// int FactorExpressionGrammarAction(const int value);

// // Factores.
// int ExpressionFactorGrammarAction(const int value);
// int ConstantFactorGrammarAction(const int value);

// // Constantes.
// int IntegerConstantGrammarAction(const int value);


//NOSOTROS
int ProcessorAdditionAction(const int proc, const int program);
int ConnectionDefinitionAction(const int input, const int conection);
int ProcessorDefinitionAction(const char* identifier,const int block);
int MultiLineBlockDefinitionAction(const int line,const int block);
int SingleLineBlockDefinitionAction(const int line);
int LocalVariableAssignmentAction(const char* var,const int value);
int ExitVariableAssignmentAction(const int exit_var,const int value);
int ExitVariableDefinitionAction(const int exit_var_number);
int BinaryOperationAction(const int binary_op, const int arg1, const int arg2);
int UnaryOperationAction(const int unary_op, const int arg);
int IdentifierArgumentAction(const char* identifier);
int InputVariableArgumentAction(const int input_number);
int ConstantArgumentAction(const int value);
int InputCountDefinitionAction(const int input_size);
int ConnectionBlockDefinitionAction(const int arrow);
int SingleIdentifierArrowAction(const char* identifier, const int arrow);
int GroupIdentifierArrowAction(const int gruop, const int arrow);
int OutputEndArrowAction();
int IdentifierEndArrowAction(const char* identifier, const int newLineArrow);
int GroupeIdentifierEndArrowAction(const int gruop,const int newLineArrow);
int InputNewLineArrowAction(const int arrow);
int GroupDefinitionAction(const char* identifier, const int gruopAux);
int IdentifierNewLineArrowAction(const char* identifier, const int arrow);
int GroupIdentifierNewLineArrowAction(const int gruop,const int arrow);
int GroupDefinitionAction(const char* identifier, const int gruopAux);
int GroupLastIdentifierAction(const char* identifier);
#endif
