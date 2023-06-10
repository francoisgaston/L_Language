#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-parser.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea
void BeginBlockCommentPatternAction();
void EndBlockCommentPatternAction();

// Comentario inline
void BeginInlineCommentPatternAction();
void EndInlineCommentPatternAction();


// -------------------------- Patrones terminales del lenguaje diseñado --------------------------------

// Se ejecuta al analizar el token "PROC" o "proc"
token ProcessorPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "INPUT" o "input"
token InputPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "OUTPUT" o "output"
token OutputPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "TRUE" o "true"
token TruePatternAction(const char * lexeme);

// Se ejecuta al analizar el token "FALSE" o "false"
token FalsePatternAction(const char * lexeme);

// Se ejecuta al analizar un operador unario (NOT/not, BUFF/buff)
token UnaryOperatorPatternAction(const char * lexeme, const int length);

// Se ejecuta al analizar un operador binario (AND/and, OR/or, NAND/nand, NOR/nor, XOR/xor, XNOR/xnor)
token BinaryOperatorPatternAction(const char * lexeme, const int length);

// Se ejecuta al analizar el token "="
token AssignmentOperatorPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "->"
token ArrowOperatorPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "("
token OpenParenthesisPatternAction(const char * lexeme);

// Se ejecuta al analizar el token ")"
token CloseParenthesisPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "{"
token OpenBracesPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "}"
token CloseBracesPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "["
token OpenBracketPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "]"
token CloseBracketPatternAction(const char * lexeme);

// Se ejecuta al analizar el token ","
token CommaPatternAction(const char * lexeme);

// Se ejecuta al analizar el token ";"
token SemicolonPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "$"
token DollarSignPatternAction(const char * lexeme);

// Se ejecuta al analizar el token "#"
token HashSignPatternAction(const char * lexeme);

// Se ejecuta al analizar un token con formato de nombre de variable
token VariableIdentifierPatternAction(const char * lexeme, const int length);

// Se ejecuta al analizar un token con formato de número
token IntegerPatternAction(const char * lexeme, const int length);

// -------------------------- Patrones terminales del lenguaje diseñado --------------------------------

// Regla que no hace nada, permite ignorar parte de la entrada
void IgnoredPatternAction(const char * lexeme, const int length);

// Patrón desconocido, permite abortar debido a un error de sintaxis
token UnknownPatternAction(const char * lexeme, const int length);

#endif
