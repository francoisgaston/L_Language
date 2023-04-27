#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>

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
int ProgramGrammarAction(const int value) {
	LogDebug("\tProgramGrammarAction(%d)", value);
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
	// state.result = value;
	return value;
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

int ProcessorAdditionAction(const int proc, const int program){
	LogDebug("\tProcessorAdditionAction(%d, %d)", proc, program);
	return true;
}

int ConnectionDefinitionAction(const int input, const int conection){
	LogDebug("\tConnectionDefinitionAction(%d, %d)", input, conection);
	return true;
}

int ProcessorDefinitionAction(const int identifier,const int block){
	LogDebug("ProcessorDefinitionAction(%d,%d)",identifier, block);
	return true;
}
int MultiLineBlockDefinitionAction(const int line,const int block){
	LogDebug("MultiLineBlockDefinitionAction(%d, %d)", line, block);
	return true;
}
int SingleLineBlockDefinitionAction(const int line){
	LogDebug("SingleLineBlockDefinitionAction(%d)", line);
	return true;
}
int LocalVariableAssignmentAction(const int var,const int value){
	LogDebug("LocalVariableAssignmentAction(%d, %d)", var, value);
	return true;
}
int ExitVariableAssignmentAction(const int exit_var,const int value){
	LogDebug("ExitVariableAssignmentAction(%d, %d)", exit_var, value);
	return true;
}
int ExitVariableDefinitionAction(const int exit_var_number){
	LogDebug("ExitVariableDefinitionAction(%d)", exit_var_number);
	return true;
}
int BinaryOperationAction(const int binary_op, const int arg1, const int arg2){
	LogDebug("BinaryOperationAction(%d, %d, %d)", binary_op, arg1, arg2);
	return true;
}
int UnaryOperationAction(const int unary_op, const int arg){
	LogDebug("UnaryOperationAction(%d, %d)", unary_op, arg);
	return true;
}
int IdentifierArgumentAction(const int identifier){
	LogDebug("IdentifierArgumentAction(%d)", identifier);
	return true;
}
int InputVariableArgumentAction(const int input_number){
	LogDebug("InputVariableArgumentAction(%d)", input_number);
	return true;
}
int ConstantArgumentAction(const int value){
	LogDebug("ConstantArgumentAction(%d)", value);
	return true;
}
int InputCountDefinitionAction(const int input_size){
	LogDebug("InputCountDefinitionAction(%d)", input_size);
	return true;
}	
int ConnectionBlockDefinitionAction(const int arrow){
	LogDebug("ConnectionBlockDefinitionAction(%d)", arrow);
	return true;
}
int SingleIdentifierArrowAction(const int identifier, const int arrow){
	LogDebug("SingleIdentifierArrowAction(%d, %d)", identifier, arrow);
	return true;
}	
int GroupIdentifierArrowAction(const int gruop, const int arrow){
	LogDebug("GroupIdentifierArrowAction(%d, %d)", gruop, arrow);
	return true;
}
int OutputEndArrowAction(){
	LogDebug("OutputEndArrowAction()");
	return true;
}
int IdentifierEndArrowAction(const int identifier, const int newLineArrow){
	LogDebug("IdentifierEndArrowAction(%d, %d)", identifier, newLineArrow);
	return true;
}
int GroupeIdentifierEndArrowAction(const int gruop,const int newLineArrow){
	LogDebug("GroupeIdentifierEndArrowAction(%d, %d)", gruop, newLineArrow);
	return true;
}
int InputNewLineArrowAction(const int arrow){
	LogDebug("InputNewLineArrowAction(%d)", arrow);
	return true;
}
int IdentifierNewLineArrowAction(const int identifier, const int arrow){
	LogDebug("IdentifierNewLineArrowAction(%d, %d)", identifier, arrow);
	return true;
}
int GroupIdentifierNewLineArrowAction(const int gruop,const int arrow){
	LogDebug("GroupIdentifierNewLineArrowAction(%d, %d)", gruop, arrow);
	return true;
}
int GroupDefinitionAction(const int identifier, const int gruopAux){
	LogDebug("GroupDefinitionAction(%d, %d)", identifier, gruopAux);
	return true;
}
int GroupLastIdentifierAction(const int identifier){
	LogDebug("ProcessorAdditionAction(%d)", identifier);
	return true;
}
