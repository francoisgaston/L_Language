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

(?i:PROC)                                   { return ProcessorPatternAction(yytext); }

(?i:INPUT)                                  { return InputPatternAction(yytext); }

(?i:OUTPUT)                                 { return OutputPatternAction(yytext); }

(?i:TRUE)                                   { return TruePatternAction(yytext); }

(?i:FALSE)                                  { return FalsePatternAction(yytext); }

(?i:NOT) |
(?i:BUFF)                                   { return UnaryOperatorPatternAction(yytext, yyleng); }

(?i:AND) |
(?i:NAND) |
(?i:OR) |
(?i:NOR) |
(?i:XOR) |
(?i:XNOR)                                   { return BinaryOperatorPatternAction(yytext, yyleng); }

"="                                         { return AssignmentOperatorPatternAction(yytext); }

"->"                                        { return ArrowOperatorPatternAction(yytext); }

"("                                         { return OpenParenthesisPatternAction(yytext); }
")"                                         { return CloseParenthesisPatternAction(yytext); }

"{"                                         { return OpenBracesPatternAction(yytext); }
"}"                                         { return CloseBracesPatternAction(yytext); }

"["                                         { return OpenBracketPatternAction(yytext); }
"]"                                         { return CloseBracketPatternAction(yytext); }

","                                         { return CommaPatternAction(yytext); }
";"                                         { return SemicolonPatternAction(yytext); }

"$"                                         { return DollarSignPatternAction(yytext); }
"#"                                         { return HashSignPatternAction(yytext); }

{identifier}                                { return VariableIdentifierPatternAction(yytext, yyleng); }
{digit}+                                    { return IntegerPatternAction(yytext, yyleng); }

{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
