%{
    #include "Object.h"
    #include "Token.h"
    #include "SyntaxPrinter.h"
    #include "SemanticActions.h"
    #include "TreeHost.h"
    #include "UnparseVisitor.h"
    #include "VisualizeVisitor.h"
    #include "SetParentTreeVisitor.h"
    #include "ValidityVisitor.h"
    #include "Deallocator.h"
    #include "Interpreter.h"

    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <string>
    #include <cstring>
    #include <cassert>
    #include <exception>

    /* Parser detected syntax or parse errors */
    int yyerror (char * yaccProvidedMessage);

    /* Recognizes tokens from the input stream and returns them to the parser */
    int yylex (void);

    void ProcessAST(Object * ast);

    /* Holds the text of the current token. It may be modified but not lengthened */
    extern char * yytext;

    /* The file from which FLEX reads */
    extern FILE * yyin;

    /* A list of tokens recognized by YACC */
    std::vector<Token> tokenList;

    /* Simple flag to indicate if a token list should be created or not */
    bool maintainTokenList = false;

    unsigned namelessFunctions = 0;

    TreeHost * unparseHost = nullptr;
    TreeHost * setParentTreeHost = nullptr;
    TreeHost * validityVisitor = nullptr;
    TreeHost * visualizeHost = nullptr;
    Interpreter * interpreter = nullptr;
%}

/* This is used so lexxer won't have to include an extra file.Basically we
 * remove a dependency from the lexxer and make the "parser.h" file valiid */
%code requires { #include "Object.h" }

%union {
    char * strVal;
    double doubleVal;
    Object * objectVal;
};

%type <objectVal> program;
%type <objectVal> stmts;
%type <objectVal> stmt;
%type <objectVal> expr;
%type <objectVal> term;
%type <objectVal> primary;
%type <objectVal> lvalue;
%type <objectVal> member;
%type <objectVal> call;
%type <objectVal> arglist;
%type <objectVal> named_arg;
%type <objectVal> comma_named_args;
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
%type <objectVal> optionals;
%type <objectVal> ifstmt;
%type <objectVal> whilestmt;
%type <objectVal> forstmt;
%type <objectVal> returnstmt;
%type <objectVal> breakstmt;
%type <objectVal> continuestmt;

%token <strVal> STRING;
%token <strVal> ID;
%token <strVal> DOLLAR_ID;
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
                  ProcessAST($$);
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
       | lvalue DOT DOLLAR_ID                   { $$ = ParseMember(ParseMemberDot($1, ParseDollarID($3))); }
       | lvalue LEFT_BRACKET expr RIGHT_BRACKET { $$ = ParseMember(ParseMemberBracket($1, $3)); }
       | call DOT ID                            { $$ = ParseMember(ParseMemberDot($1, ParseSimpleID($3)));; }
       | call DOT DOLLAR_ID                     { $$ = ParseMember(ParseMemberDot($1, ParseDollarID($3))); }
       | call LEFT_BRACKET expr RIGHT_BRACKET   { $$ = ParseMember(ParseMemberBracket($1, $3));; }
       ;

call : call LEFT_PAR arglist RIGHT_PAR                          { $$ = ParseCallCall($1, $3); }
     | lvalue LEFT_PAR arglist RIGHT_PAR                        { $$ = ParseLvalueCall($1, $3); }
     | lvalue DOUBLE_DOT ID LEFT_PAR arglist RIGHT_PAR          { $$ = ParseLvalueMethodCall($1, ParseSimpleID($3), $5); }
     | LEFT_PAR funcdef RIGHT_PAR LEFT_PAR arglist RIGHT_PAR    { $$ = ParseFuncdefCall($2, $5); }
     ;

arglist : expr comma_exprs                                  {  $$ = ParseArgList($1, $2);  }
        | named_arg comma_named_args                        {  $$ = ParseArgList($1, $2);  }
        | expr comma_exprs COMMA named_arg comma_named_args {  $$ = ParseMixedArgList(
                                                                        ParseArgList($1, $2),
                                                                        ParseArgList($4, $5)
                                                                        );
                                                            }
        | /* Empty */                                       {  $$ = ParseEmptyArgList();  }
        ;

comma_named_args : comma_named_args COMMA named_arg {  $$ = ParseCommaNamedArgs($1, $3);  }
                 | /* Empty */                      {  $$ = ParseEmptyArgList();  }
                 ;

named_arg : ID COLON expr   {  $$ = ParseNamedArg(ParseSimpleID($1), $3);  }
          ;

elist : expr comma_exprs    { $$ = ParseElist($1, $2); }
      | /* Empty */         { $$ = ParseEmptyElist(); }
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
      | NIL    { $$ = ParseConst(ParseNil()); }
      | TRUE   { $$ = ParseConst(ParseTrue()); }
      | FALSE  { $$ = ParseConst(ParseFalse()); }
      ;

idlist : ID comma_ids                                   { $$ = ParseIdList(ParseFormal($1), $2); }
       | ID ASSIGN expr optionals                       { $$ = ParseIdList(ParseAssign(ParseFormal($1), $3), $4); }
       | ID comma_ids COMMA ID ASSIGN expr optionals    { $$ = ParseMixedIdList(
                                                                ParseIdList(ParseFormal($1), $2),
                                                                ParseIdList(ParseAssign(ParseFormal($4), $6), $7)
                                                                );
                                                        }
       | /* empty */                                    { $$ = ParseEmptyIdlist(); }
       ;

comma_ids : comma_ids COMMA ID           { $$ = ParseCommaIds($1, ParseFormal($3)); }
          | /* empty */                  { $$ = ParseEmptyIdlist(); }
          ;

optionals : optionals COMMA ID ASSIGN expr  { $$ = ParseOptionals($1, ParseAssign(ParseFormal($3), $5)); }
          | /* empty */                     { $$ = ParseEmptyIdlist(); }
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
    std::cerr   << "\033[31;1m"     //RED
                << "Syntax Error: "
                << "\033[0m"
                << "Invalid syntax at line " << yylineno
                << " before " << yytext << std::endl;
    exit(EXIT_FAILURE);
}

void Usage(const std::string & str) {
    std::cout << "Usage: " << (str.empty() ? "interpreter.out" : str) << " [input_file]\n" << std::endl
              << "Options:" << std::endl
              << "    input_file:  The file containing the source code to be compiled" << std::endl;
}

void ProcessAST(Object * ast) {
    assert(ast && ast->IsValid());

    unparseHost->Accept(*ast);
    visualizeHost->Accept(*ast);
    setParentTreeHost->Accept(*ast);
    validityVisitor->Accept(*ast);
    interpreter->Execute(*ast);

#define AST_MEM_CLEANUP
#ifdef AST_MEM_CLEANUP
    TreeHost * h = new TreeHost(new DeallocateVisitor());
    h->Accept(*ast);
    delete h;
#endif
}

int main(int argc, char ** argv) {
    try {
        if (argc > 2) {
            Usage(argv[0]);
            return EXIT_FAILURE;
        }

        if (argc < 2) yyin = stdin;
        else if (!(yyin = fopen(argv[1], "r"))) {
            std::cerr << "Coult not read input file \"" << argv[1] << "\"" << std::endl;
            return EXIT_FAILURE;
        }

        unparseHost = new TreeHost(new UnparseVisitor());
        visualizeHost = new TreeHost(new VisualizeVisitor());
        setParentTreeHost = new TreeHost(new SetParentTreeVisitor());
        validityVisitor = new TreeHost(new ValidityVisitor());
        interpreter = new Interpreter();

        /* The Bison parser */
        yyparse();

        std::for_each(tokenList.begin(), tokenList.end(), SyntaxPrinter("alpha_TokenList.txt"));

        delete unparseHost;
        delete visualizeHost;
        delete setParentTreeHost;
        delete validityVisitor;
        delete interpreter;

        fclose(yyin);

    } catch(std::exception & e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}
