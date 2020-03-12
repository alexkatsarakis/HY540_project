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

#ifndef YY_YY_SRC_PARSER_H_INCLUDED
# define YY_YY_SRC_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STRING = 258,
    ID = 259,
    NUMBER = 260,
    PLUS = 261,
    MINUS = 262,
    MUL = 263,
    DIV = 264,
    MODULO = 265,
    EQUAL = 266,
    NOT_EQUAL = 267,
    GREATER = 268,
    LESS = 269,
    GREATER_EQUAL = 270,
    LESS_EQUAL = 271,
    AND = 272,
    OR = 273,
    IF = 274,
    ELSE = 275,
    WHILE = 276,
    FOR = 277,
    FUNCTION = 278,
    RETURN = 279,
    BREAK = 280,
    CONTINUE = 281,
    NOT = 282,
    LOCAL = 283,
    TRUE = 284,
    FALSE = 285,
    NIL = 286,
    LEFT_BRACE = 287,
    RIGHT_BRACE = 288,
    LEFT_BRACKET = 289,
    RIGHT_BRACKET = 290,
    LEFT_PAR = 291,
    RIGHT_PAR = 292,
    SEMICOLON = 293,
    COMMA = 294,
    COLON = 295,
    DOUBLE_COLON = 296,
    DOT = 297,
    DOUBLE_DOT = 298,
    ASSIGN = 299,
    PLUS_PLUS = 300,
    MINUS_MINUS = 301,
    UMINUS = 302,
    NO_ELSE = 303
  };
#endif
/* Tokens.  */
#define STRING 258
#define ID 259
#define NUMBER 260
#define PLUS 261
#define MINUS 262
#define MUL 263
#define DIV 264
#define MODULO 265
#define EQUAL 266
#define NOT_EQUAL 267
#define GREATER 268
#define LESS 269
#define GREATER_EQUAL 270
#define LESS_EQUAL 271
#define AND 272
#define OR 273
#define IF 274
#define ELSE 275
#define WHILE 276
#define FOR 277
#define FUNCTION 278
#define RETURN 279
#define BREAK 280
#define CONTINUE 281
#define NOT 282
#define LOCAL 283
#define TRUE 284
#define FALSE 285
#define NIL 286
#define LEFT_BRACE 287
#define RIGHT_BRACE 288
#define LEFT_BRACKET 289
#define RIGHT_BRACKET 290
#define LEFT_PAR 291
#define RIGHT_PAR 292
#define SEMICOLON 293
#define COMMA 294
#define COLON 295
#define DOUBLE_COLON 296
#define DOT 297
#define DOUBLE_DOT 298
#define ASSIGN 299
#define PLUS_PLUS 300
#define MINUS_MINUS 301
#define UMINUS 302
#define NO_ELSE 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 31 "src/parser.y" /* yacc.c:1909  */

    char * strVal;
    double doubleVal;

#line 155 "src/parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_PARSER_H_INCLUDED  */
