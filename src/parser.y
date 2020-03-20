%{
    #include "Object.h"
    #include "Token.h"
    #include "SyntaxPrinter.h"
    #include "GrammarRules.h"
    #include "SemanticActions.h"
    #include "TreeHost.h"
    #include "VisualizeVisitor.h"
	#include "UnparseVisitor.h"

    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <string>
    #include <cstring>

    /* Parser detected syntax or parse errors */
    int yyerror (char * yaccProvidedMessage);

    /* Recognizes tokens from the input stream and returns them to the parser */
    int yylex (void);

    /* The  number of the current line */
    extern int yylineno;

    /* Holds the text of the current token. It may be modified but not lengthened */
    extern char * yytext;

    /* The file from which FLEX reads */
    extern FILE * yyin;

    /* A list of tokens recognized by YACC */
    std::vector<Token> tokenList;

    /* Simple flag to indicate if a token list should be created or not */
    bool maintainTokenList = true;

    unsigned namelessFunctions;

    SyntaxPrinter rulesPrinter("alpha_GrammarRules.txt");

    TreeHost * host = nullptr;
%}

%union {
    char * strVal;
    double doubleVal;
    Object * objectVal;
}

%type <objectVal> program;
%type <objectVal> stmts;
%type <objectVal> stmt;
%type <objectVal> expr;
%type <objectVal> term;
%type <objectVal> primary;
%type <objectVal> lvalue;
%type <objectVal> member;
%type <objectVal> call;
%type <objectVal> callsuffix;
%type <objectVal> normcall;
%type <objectVal> methodcall;
%type <objectVal> elist;
%type <objectVal> comma_exprs;
%type <objectVal> objectdef;
%type <objectVal> indexed;
%type <objectVal> comma_indexedelems;
%type <objectVal> indexedelem;
%type <objectVal> block;
%type <objectVal> funcdef;
%type <objectVal> const;
%type <objectVal> idlist;
%type <objectVal> comma_ids;
%type <objectVal> ifstmt;
%type <objectVal> whilestmt;
%type <objectVal> forstmt;
%type <objectVal> returnstmt;
%type <objectVal> breakstmt;
%type <objectVal> continuestmt;

%token <strVal> STRING;
%token <strVal> ID;
%token <doubleVal> NUMBER

/* Operators */
%token PLUS
%token MINUS
%token MUL
%token DIV
%token MODULO
%token EQUAL
%token NOT_EQUAL
%token GREATER
%token LESS
%token GREATER_EQUAL
%token LESS_EQUAL
%token AND
%token OR
/* Keywords */
%token IF
%token ELSE
%token WHILE
%token FOR
%token FUNCTION
%token RETURN
%token BREAK
%token CONTINUE
%token NOT
%token LOCAL
%token TRUE
%token FALSE
%token NIL
/* Punctuation */
%token LEFT_BRACE
%token RIGHT_BRACE
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token LEFT_PAR
%token RIGHT_PAR
%token SEMICOLON
%token COMMA
%token COLON
%token DOUBLE_COLON
%token DOT
%token DOUBLE_DOT
/* Misc */
%token ASSIGN
%token PLUS_PLUS
%token MINUS_MINUS

/* Precedence rules and Associative property */
%right ASSIGN
%left OR
%left AND
%nonassoc EQUAL NOT_EQUAL
%nonassoc GREATER GREATER_EQUAL LESS LESS_EQUAL
%left PLUS MINUS
%left MUL DIV MODULO
%right NOT PLUS_PLUS MINUS_MINUS UMINUS
%left DOT DOUBLE_DOT
%left LEFT_BRACKET RIGHT_BRACKET
%left LEFT_PAR RIGHT_PAR

/* Define the precedence of else and no_else so there won't be a conflict in the
 * ifstmt grammar rule */
%nonassoc NO_ELSE
%nonassoc ELSE

/* Define the start symbol for the grammar */
%start program

%%

program : stmts { $$ = ParseProgram($1);
                  host->Accept(*$$);
                }
        ;

stmts : stmts stmt  { $$ = ParseStmts($1, $2); }
      | /* Empty */ { $$ = ParseEmptyStmts(); }
      ;

stmt : expr SEMICOLON  { $$ = ParseStmt($1); }
     | ifstmt          { $$ = ParseStmt($1); }
     | whilestmt       { $$ = ParseStmt($1); }
     | forstmt         { $$ = ParseStmt($1); }
     | returnstmt      { $$ = ParseStmt($1); }
     | breakstmt       { $$ = ParseStmt($1); }
     | continuestmt    { $$ = ParseStmt($1); }
     | block           { $$ = ParseStmt($1); }
     | funcdef         { $$ = ParseStmt($1); }
     | SEMICOLON       { $$ = ParseSemicolon(); }
     | error SEMICOLON { yyerrok; yyclearin; }
     ;

expr : lvalue ASSIGN expr      { $$ = ParseExpr(ParseAssign($1, $3)); }
     | expr PLUS expr          { $$ = ParseExpr(ParsePlus($1, $3)); }
     | expr MINUS expr         { $$ = ParseExpr(ParseMinus($1, $3)); }
     | expr MUL expr           { $$ = ParseExpr(ParseMul($1, $3)); }
     | expr DIV expr           { $$ = ParseExpr(ParseDiv($1, $3)); }
     | expr MODULO expr        { $$ = ParseExpr(ParseModulo($1, $3)); }
     | expr GREATER expr       { $$ = ParseExpr(ParseGreater($1, $3)); }
     | expr LESS expr          { $$ = ParseExpr(ParseLess($1, $3)); }
     | expr GREATER_EQUAL expr { $$ = ParseExpr(ParseGreaterEqual($1, $3)); }
     | expr LESS_EQUAL expr    { $$ = ParseExpr(ParseLessEqual($1, $3)); }
     | expr EQUAL expr         { $$ = ParseExpr(ParseEqual($1, $3)); }
     | expr NOT_EQUAL expr     { $$ = ParseExpr(ParseNotEqual($1, $3)); }
     | expr AND expr           { $$ = ParseExpr(ParseAnd($1, $3)); }
     | expr OR expr            { $$ = ParseExpr(ParseOr($1, $3)); }
     | term                    { $$ = ParseExpr($1); }
     ;

term : LEFT_PAR expr RIGHT_PAR { $$ = ParseTerm($2); }
     | MINUS expr %prec UMINUS { $$ = ParseTerm(ParseUminus($2)); }
     | NOT expr                { $$ = ParseTerm(ParseNot($2)); }
     | PLUS_PLUS lvalue        { $$ = ParseTerm(ParsePlusPlusLvalue($2)); }
     | lvalue PLUS_PLUS        { $$ = ParseTerm(ParseLvaluePlusPlus($1)); }
     | MINUS_MINUS lvalue      { $$ = ParseTerm(ParseMinusMinusLvalue($2)); }
     | lvalue MINUS_MINUS      { $$ = ParseTerm(ParseLvalueMinusMinus($1)); }
     | primary                 { $$ = ParseTerm($1); }
     ;

primary : lvalue                     { $$ = ParsePrimary($1); }
        | call                       { $$ = ParsePrimary($1); }
        | objectdef                  { $$ = ParsePrimary($1); }
        | LEFT_PAR funcdef RIGHT_PAR { $$ = ParsePrimary($2); }
        | const                      { $$ = ParsePrimary($1); }
        ;

lvalue : ID              { $$ = ParseLvalue(ParseSimpleID($1)); }
       | LOCAL ID        { $$ = ParseLvalue(ParseLocalID($2)); } //Parse Simple Id
       | DOUBLE_COLON ID { $$ = ParseLvalue(ParseDoubleColonID($2)); } //Parse Simple Id
       | member          { $$ = ParseLvalue($1); }
       ;

member : lvalue DOT ID                          { $$ = ParseMember(ParseMemberDot($1, ParseSimpleID($3))); }
       | lvalue LEFT_BRACKET expr RIGHT_BRACKET { $$ = ParseMember(ParseMemberBracket($1, $3)); }
       | call DOT ID                            { $$ = ParseMember(ParseMemberDot($1, ParseSimpleID($3)));; }
       | call LEFT_BRACKET expr RIGHT_BRACKET   { $$ = ParseMember(ParseMemberBracket($1, $3));; }
       ;

call : call LEFT_PAR elist RIGHT_PAR                       { $$ = ParseCallCall($1, $3); }
     | lvalue callsuffix                                   { $$ = ParseLvalueCall($1, $2); }
     | LEFT_PAR funcdef RIGHT_PAR LEFT_PAR elist RIGHT_PAR { $$ = ParseFuncdefCall($2, $5); }
     ;

callsuffix : normcall   { $$ = ParseCallSuffix($1); }
           | methodcall { $$ = ParseCallSuffix($1); }
           ;

normcall : LEFT_PAR elist RIGHT_PAR { $$ = ParseNormCall($2); }
         ;

methodcall : DOUBLE_DOT ID LEFT_PAR elist RIGHT_PAR { $$ = ParseMethodCall(ParseSimpleID($2), $4); }
           ;

elist : expr comma_exprs { $$ = ParseElist($1, $2); }
      | /* Empty */      { $$ = ParseEmptyElist(); }
      ;

comma_exprs : comma_exprs COMMA expr { $$ = ParseCommaExprs($1, $3); }
            | /* Empty */            { $$ = ParseEmptyElist(); }
            ;

objectdef : LEFT_BRACKET elist RIGHT_BRACKET   { $$ = ParseObjectDef($2); }
          | LEFT_BRACKET indexed RIGHT_BRACKET { $$ = ParseObjectDef($2); }
          ;

indexed : indexedelem comma_indexedelems { $$ = ParseIndexed($1, $2); }
        ;

comma_indexedelems : comma_indexedelems COMMA indexedelem { $$ = ParseCommaIndexed($1, $3); }
                   | /* Empty */                          { $$ = ParseEmptyIndexed(); }
                   ;

indexedelem : LEFT_BRACE expr COLON expr RIGHT_BRACE { $$ = ParseIndexedElem($2, $4); }
            ;

block : LEFT_BRACE stmts RIGHT_BRACE { $$ = ParseBlock($2); }
      ;

funcdef : FUNCTION ID LEFT_PAR idlist RIGHT_PAR block { $$ = ParseFuncDef(ParseSimpleID($2), $4, $6); }
        | FUNCTION LEFT_PAR idlist RIGHT_PAR block    { $$ = ParseFuncDef(ParseSimpleID(strdup(std::string("$" + std::to_string(namelessFunctions++)).c_str())), $3, $5); }
        ;

const : NUMBER { $$ = ParseConst(ParseNumber($1)); }
      | STRING { $$ = ParseConst(ParseString($1)); }
      | NIL    { $$ = ParseConst(ParseNill()); }
      | TRUE   { $$ = ParseConst(ParseTrue()); }
      | FALSE  { $$ = ParseConst(ParseFalse()); }
      ;

idlist : ID comma_ids { $$ = ParseIdList(ParseSimpleID($1), $2); }
       | /* Empty */  { $$ = ParseEmptyIdlist(); }
       ;

comma_ids : comma_ids COMMA ID { $$ = ParseCommaIds($1, ParseSimpleID($3)); }
          | /* Empty */        { $$ = ParseEmptyIdlist(); }
          ;

ifstmt : IF LEFT_PAR expr RIGHT_PAR stmt %prec NO_ELSE { $$ = ParseIfStmt($3, $5); }
       | IF LEFT_PAR expr RIGHT_PAR stmt ELSE stmt     { $$ = ParseIfStmt($3, $5, $7); }
       ;

whilestmt : WHILE LEFT_PAR expr RIGHT_PAR stmt { $$ = ParseWhileStmt($3, $5); }
          ;

forstmt : FOR LEFT_PAR elist SEMICOLON expr SEMICOLON elist RIGHT_PAR stmt { $$ = ParseForStmt($3, $5, $7, $9); }
        ;

returnstmt : RETURN expr SEMICOLON        { $$ = ParseReturnStmt($2); }
           | RETURN /* Empty */ SEMICOLON { $$ = ParseReturnStmt(); }
           ;

breakstmt : BREAK SEMICOLON { $$ = ParseBreakStmt(); }
          ;

continuestmt : CONTINUE SEMICOLON { $$ = ParseContinueStmt(); }
             ;

%%

int yyerror (char * yaccProvidedMessage) {
    std::cerr << "Invalid syntax befor " << yytext << std::endl;
    exit(EXIT_FAILURE);
}

void Usage(const std::string & str) {
    std::cout << "Usage: " << (str.empty() ? "interpreter.out" : str) << "[input_file]" << std::endl
              << "Options:" << std::endl
              << "    input_file:  The file containing the source code to be compiled" << std::endl;
}

int main(int argc, char ** argv) {

    if (argc > 2) {
        Usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (argc < 2) yyin = stdin;
    else if (!(yyin = fopen(argv[1], "r"))) {
        std::cerr << "Coult not read input file \"" << argv[1] << "\"" << std::endl;
        return EXIT_FAILURE;
    }

    TreeHost *visualizeHost = new TreeHost(new VisualizeVisitor());
    TreeHost *unparseHost = new TreeHost(new UnparseVisitor());
    // host->visitor = new VisualizeVisitor();
    // host->InstallAllAcceptors();

    /* The Bison parser */
    yyparse();

    std::for_each(tokenList.begin(), tokenList.end(), SyntaxPrinter("alpha_TokenList.txt"));

    return (EXIT_SUCCESS);
}
