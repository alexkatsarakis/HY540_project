#ifndef _TOKENTYPES_H_
#define _TOKENTYPES_H_

/* The major categories of tokens */
#define C_KEYWORD "KEYWORD"
#define C_OPERATOR "OPERATOR"
#define C_INTCONST "INTCONST"
#define C_REALCONST "REALCONST"
#define C_STRING "STRING"
#define C_PUNCTUATION "PUNCTUATION"
#define C_IDENTIFIER "IDENTIFIER"
#define C_COMMENT "COMMENT"

/* Sub-categories of tokens */
#define A_IF "IF"
#define A_ELSE "ELSE"
#define A_WHILE "WHILE"
#define A_FOR "FOR"
#define A_FUNCTION "FUNCTION"
#define A_RETURN "RETURN"
#define A_BREAK "BREAK"
#define A_CONTINUE "CONTINUE"
#define A_AND "AND"
#define A_NOT "NOT"
#define A_OR "OR"
#define A_LOCAL "LOCAL"
#define A_TRUE "TRUE"
#define A_FALSE "FALSE"
#define A_NIL "NIL"
/* Operators */
#define A_ASSIGN "ASSIGN"
#define A_PLUS "PLUS"
#define A_MINUS "MINUS"
#define A_MULTIPLY "MULTIPLY"
#define A_DIVIDE "DIVIDE"
#define A_MODULO "MODULO"
#define A_EQUAL "EQUAL"
#define A_NOT_EQUAL "NOT_EQUAL"
#define A_PLUS_PLUS "PLUS_PLUS"
#define A_MINUS_MINUS "MINUS_MINUS"
#define A_GREATER "GREATER"
#define A_LESS "LESS"
#define A_GREATER_EQUAL "GREATER_EQUAL"
#define A_LESS_EQUAL "LESS_EQUAL"
/* Punctuation */
#define A_LEFT_BRACE "LEFT_BRACE"
#define A_RIGHT_BRACE "RIGHT_BRACE"
#define A_LEFT_BRACKET "LEFT_BRACKET"
#define A_RIGHT_BRACKET "RIGHT_BRACKET"
#define A_LEFT_PARENTHESIS "LEFT_PARENTHESIS"
#define A_RIGHT_PARENTHESIS "RIGHT_PARENTHESIS"
#define A_SEMICOLON "SEMICOLON"
#define A_COMMA "COMMA"
#define A_COLON "COLON"
#define A_COLON_COLON "COLON_COLON"
#define A_DOT "DOT"
#define A_DOT_DOT "DOT_DOT"
/* Comments */
#define A_BLOCK_COMMENT "BLOCK_COMMENT"
#define A_NESTED_BLOCK_COMMENT "NESTED_BLOCK_COMMENT"
#define A_LINE_COMMENT "LINE_COMMENT"
/* Identifiers */
#define A_NORMAL_ID "ID"
#define A_DOLLAR_ID "DOLLAR_ID"
/* Not applicable */
#define A_NA "N/A"

#endif
