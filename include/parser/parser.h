/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_INCLUDE_PARSER_PARSER_H_INCLUDED
# define YY_YY_INCLUDE_PARSER_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 56 "src/parser.y" /* yacc.c:1909  */
 #include "Object.h" 

#line 47 "include/parser/parser.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STRING = 258,
    ID = 259,
    DOLLAR_ID = 260,
    NUMBER = 261,
    PLUS = 262,
    MINUS = 263,
    MUL = 264,
    DIV = 265,
    MODULO = 266,
    EQUAL = 267,
    NOT_EQUAL = 268,
    GREATER = 269,
    LESS = 270,
    GREATER_EQUAL = 271,
    LESS_EQUAL = 272,
    AND = 273,
    OR = 274,
    IF = 275,
    ELSE = 276,
    WHILE = 277,
    FOR = 278,
    FUNCTION = 279,
    RETURN = 280,
    BREAK = 281,
    CONTINUE = 282,
    NOT = 283,
    LOCAL = 284,
    TRUE = 285,
    FALSE = 286,
    NIL = 287,
    LEFT_BRACE = 288,
    RIGHT_BRACE = 289,
    LEFT_BRACKET = 290,
    RIGHT_BRACKET = 291,
    LEFT_PAR = 292,
    RIGHT_PAR = 293,
    SEMICOLON = 294,
    COMMA = 295,
    COLON = 296,
    DOUBLE_COLON = 297,
    DOT = 298,
    DOUBLE_DOT = 299,
    ASSIGN = 300,
    PLUS_PLUS = 301,
    MINUS_MINUS = 302,
    UMINUS = 303,
    NO_ELSE = 304
  };
#endif
/* Tokens.  */
#define STRING 258
#define ID 259
#define DOLLAR_ID 260
#define NUMBER 261
#define PLUS 262
#define MINUS 263
#define MUL 264
#define DIV 265
#define MODULO 266
#define EQUAL 267
#define NOT_EQUAL 268
#define GREATER 269
#define LESS 270
#define GREATER_EQUAL 271
#define LESS_EQUAL 272
#define AND 273
#define OR 274
#define IF 275
#define ELSE 276
#define WHILE 277
#define FOR 278
#define FUNCTION 279
#define RETURN 280
#define BREAK 281
#define CONTINUE 282
#define NOT 283
#define LOCAL 284
#define TRUE 285
#define FALSE 286
#define NIL 287
#define LEFT_BRACE 288
#define RIGHT_BRACE 289
#define LEFT_BRACKET 290
#define RIGHT_BRACKET 291
#define LEFT_PAR 292
#define RIGHT_PAR 293
#define SEMICOLON 294
#define COMMA 295
#define COLON 296
#define DOUBLE_COLON 297
#define DOT 298
#define DOUBLE_DOT 299
#define ASSIGN 300
#define PLUS_PLUS 301
#define MINUS_MINUS 302
#define UMINUS 303
#define NO_ELSE 304

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 58 "src/parser.y" /* yacc.c:1909  */

    char * strVal;
    double doubleVal;
    Object * objectVal;

#line 163 "include/parser/parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_INCLUDE_PARSER_PARSER_H_INCLUDED  */
