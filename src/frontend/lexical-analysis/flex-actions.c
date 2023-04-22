#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

void BeginBlockCommentPatternAction() {
	LogDebug("BeginBlockCommentPatternAction.");
}

void EndBlockCommentPatternAction() {
	LogDebug("EndBlockCommentPatternAction.");
}

void BeginInlineCommentPatternAction() {
    LogDebug("BeginInlineCommentPatternAction.");
}

void EndInlineCommentPatternAction() {
    LogDebug("EndInlineCommentPatternAction.");
}

token ProcessorPatternAction(const char * lexeme) {
    LogDebug("ProcessorPatternAction: '%s'.", lexeme);
    yylval.token = PROCESSOR;
    return PROCESSOR;
}

token InputPatternAction(const char * lexeme) {
    LogDebug("InputPatternAction: '%s'.", lexeme);
    yylval.token = INPUT;
    return INPUT;
}

token OutputPatternAction(const char * lexeme) {
    LogDebug("OutputPatternAction: '%s'.", lexeme);
    yylval.token = OUTPUT;
    return OUTPUT;
}

token TruePatternAction(const char * lexeme) {
    LogDebug("TruePatternAction: '%s'.", lexeme);
    yylval.token = TRUE;
    return TRUE;
}

token FalsePatternAction(const char * lexeme) {
    LogDebug("FalsePatternAction: '%s'.", lexeme);
    yylval.token = FALSE;
    return FALSE;
}

token UnaryOperatorPatternAction(const char * lexeme) {
    LogDebug("UnaryOperatorPatternAction: '%s'.", lexeme);
    yylval.token = UNARY_OP;
    return UNARY_OP;
}

token BinaryOperatorPatternAction(const char * lexeme) {
    LogDebug("BinaryOperatorPatternAction: '%s'.", lexeme);
    yylval.token = BINARY_OP;
    return BINARY_OP;
}

token AssignmentOperatorPatternAction(const char * lexeme) {
    LogDebug("AssignmentPatternAction: '%s'.", lexeme);
    yylval.token = ASSIGNMENT_OP;
    return ASSIGNMENT_OP;
}

token ArrowOperatorPatternAction(const char * lexeme) {
    LogDebug("ArrowOperatorPatternAction: '%s'.", lexeme);
    yylval.token = ARROW_OP;
    return ARROW_OP;
}

token OpenParenthesisPatternAction(const char * lexeme) {
    LogDebug("OpenParenthesisPatternAction: '%s'.", lexeme);
    yylval.token = OPEN_PARENTHESIS;
    return OPEN_PARENTHESIS;
}

token CloseParenthesisPatternAction(const char * lexeme) {
	LogDebug("CloseParenthesisPatternAction: '%s'.", lexeme);
	yylval.token = CLOSE_PARENTHESIS;
	return CLOSE_PARENTHESIS;
}

token OpenBracesPatternAction(const char * lexeme) {
    LogDebug("OpenBracesPatternAction: '%s'.", lexeme);
    yylval.token = OPEN_BRACES;
    return OPEN_BRACES;
}

token CloseBracesPatternAction(const char * lexeme) {
    LogDebug("CloseBracesPatternAction: '%s'.", lexeme);
    yylval.token = CLOSE_BRACES;
    return CLOSE_BRACES;
}

token CommaPatternAction(const char * lexeme) {
    LogDebug("CommaPatternAction: '%s'.", lexeme);
    yylval.token = COMMA;
    return COMMA;
}

token SemicolonPatternAction(const char * lexeme) {
    LogDebug("SemicolonPatternAction: '%s'.", lexeme);
    yylval.token = SEMICOLON;
    return SEMICOLON;
}

token DollarSignPatternAction(const char * lexeme) {
    LogDebug("DollarSignPatternAction: '%s'.", lexeme);
    yylval.token = DOLLAR_SIGN;
    return DOLLAR_SIGN;
}

token HashSignPatternAction(const char * lexeme) {
    LogDebug("HashSignPatternAction: '%s'.", lexeme);
    yylval.token = HASH_SIGN;
    return HASH_SIGN;
}

token VariableIdentifierPatternAction(const char * lexeme, const int length) {
    LogDebug("VariableIdentifierPatternAction: '%s' (length = %d).", lexeme, length);
    yylval.identifier = lexeme;
    return IDENTIFIER;
}

token IntegerPatternAction(const char * lexeme, const int length) {
    LogDebug("IntegerPatternAction: '%s' (length = %d).", lexeme, length);
    yylval.integer = atoi(lexeme);
    return INTEGER;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	LogDebug("IgnoredPatternAction: '%s' (length = %d).", lexeme, length);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
}

token UnknownPatternAction(const char * lexeme, const int length) {
    LogDebug("UnknownPatternAction: '%s' (length = %d).", lexeme, length);
    yylval.token = YYUNDEF;
    // Al emitir este token, el compilador aborta la ejecución.
    return YYUNDEF;
}
