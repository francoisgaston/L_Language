%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x BLOCK_COMMENT
%x INLINE_COMMENT

/* Patrones reutilizables. */
digit			[0-9]
identifier      [a-zA-Z][a-zA-Z0-9_-]*
endline			[\n]
whitespace		[ \f\n\r\t\v]

%%

"/*"										{ BEGIN(BLOCK_COMMENT); BeginBlockCommentPatternAction(); }
<BLOCK_COMMENT>.							{ ; }
<BLOCK_COMMENT>{endline}					{ ; }
<BLOCK_COMMENT>"*/"							{ EndBlockCommentPatternAction(); BEGIN(INITIAL); }

"//"                                        { BEGIN(INLINE_COMMENT); BeginInlineCommentPatternAction(); }
<INLINE_COMMENT>.                           { ; }
<INLINE_COMMENT>{endline}                   { EndInlineCommentPatternAction(); BEGIN(INITIAL); }

"proc" |
"PROC"                                      { return ProcessorPatternAction(yytext); }

"input" |
"INPUT"                                     { return InputPatternAction(yytext); }

"output" |
"OUTPUT"                                    { return OutputPatternAction(yytext); }

"true" |
"TRUE"                                      { return TruePatternAction(yytext); }

"false" |
"FALSE"                                     { return FalsePatternAction(yytext); }

"not" |
"NOT" |
"buff" |
"BUFF"                                      { return UnaryOperatorPatternAction(yytext); }

"and" |
"AND" |
"nand" |
"NAND" |
"or" |
"OR" |
"nor" |
"NOR" |
"xor" |
"XOR" |
"xnor" |
"XNOR"                                      { return BinaryOperatorPatternAction(yytext); }

"="                                         { return AssignmentOperatorPatternAction(yytext); }

"->"                                        { return ArrowOperatorPatternAction(yytext); }

"("                                         { return OpenParenthesisPatternAction(yytext); }
")"                                         { return CloseParenthesisPatternAction(yytext); }

"{"                                         { return OpenBracesPatternAction(yytext); }
"}"                                         { return CloseBracesPatternAction(yytext); }

","                                         { return CommaPatternAction(yytext); }
";"                                         { return SemicolonPatternAction(yytext); }

"$"                                         { return DollarSignPatternAction(yytext); }
"#"                                         { return HashSignPatternAction(yytext); }

{identifier}                                { return VariableIdentifierPatternAction(yytext, yyleng); }
{digit}+                                    { return IntegerPatternAction(yytext, yyleng); }

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
