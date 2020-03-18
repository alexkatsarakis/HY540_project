%{
    #include "Token.h"
    #include "SyntaxPrinter.h"
    #include "GrammarRules.h"
    #include "Object.h"

    #include <iostream>
    #include <vector>
    #include <algorithm>

    /* Parser detected syntax or parse errors */
    int yyerror (char * yaccProvidedMessage);

    /* Recognizes tokens from the input stream and returns them to the parser */
    int yylex (void);

    /* The  number of the current line */
    extern int yylineno;

    /* Holds the text of the current token. It may be modified but not lengthened */
    extern char * yytext;

    /* A list of tokens recognized by YACC */
    std::vector<Token> tokenList;

    /* Simple flag to indicate if a token list should be created or not */
    bool maintainTokenList = true;

    SyntaxPrinter rulesPrinter("alpha_GrammarRules.txt");
%}

%union {
    Object * objectVal;
    char * strVal;
    double doubleVal;
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

program : stmts { }
        ;

stmts : stmts stmt  { PRINT_STMTS; }
      | /* Empty */ { PRINT_STMTS_E; }
      ;

stmt : expr SEMICOLON { PRINT_STMT_EXP; }
     | ifstmt { PRINT_STMT_IF; }
     | whilestmt { PRINT_STMT_WH; }
     | forstmt { PRINT_STMT_FOR; }
     | returnstmt { PRINT_STMT_RET; }
     | breakstmt { PRINT_STMT_BR; }
     | continuestmt { PRINT_STMT_CON; }
     | block { PRINT_STMT_BLO; }
     | funcdef { PRINT_STMT_FUNC; }
     | SEMICOLON { PRINT_STMT_SEMI; }
     | error SEMICOLON { yyerrok; yyclearin; }
     ;

expr : lvalue ASSIGN expr { PRINT_EXPR_ASS; }
     | expr PLUS expr { PRINT_EXPR_PL; }
     | expr MINUS expr { PRINT_EXPR_MIN; }
     | expr MUL expr { PRINT_EXPR_MUL; }
     | expr DIV expr { PRINT_EXPR_DIV; }
     | expr MODULO expr { PRINT_EXPR_MOD; }
     | expr GREATER expr { PRINT_EXPR_GR; }
     | expr LESS expr { PRINT_EXPR_LE; }
     | expr GREATER_EQUAL expr { PRINT_EXPR_GR_E; }
     | expr LESS_EQUAL expr { PRINT_EXPR_LE_E; }
     | expr EQUAL expr { PRINT_EXPR_EQ; }
     | expr NOT_EQUAL expr { PRINT_EXPR_NEQ; }
     | expr AND expr { PRINT_EXPR_AND; }
     | expr OR expr { PRINT_EXPR_OR; }
     | term { PRINT_EXPR_TERM; }
     ;

term : LEFT_PAR expr RIGHT_PAR { PRINT_TERM_PAR; }
     | MINUS expr %prec UMINUS { PRINT_TERM_UM; }
     | NOT expr { PRINT_TERM_NOT; }
     | PLUS_PLUS lvalue { PRINT_TERM_P_L; }
     | lvalue PLUS_PLUS { PRINT_TERM_L_P; }
     | MINUS_MINUS lvalue { PRINT_TERM_M_L; }
     | lvalue MINUS_MINUS { PRINT_TERM_L_M; }
     | primary { PRINT_TERM_PRIM; }
     ;

primary : lvalue { PRINT_PRIMARY_LVAL; }
        | call { PRINT_PRIMARY_CALL; }
        | objectdef { PRINT_PRIMARY_OBJ; }
        | LEFT_PAR funcdef RIGHT_PAR { PRINT_PRIMARY_FUNC; }
        | const { PRINT_PRIMARY_CON; }
        ;

lvalue : ID { PRINT_LVALUE_ID; }
       | LOCAL ID { PRINT_LVALUE_LID; }
       | DOUBLE_COLON ID { PRINT_LVALUE_GID; }
       | member { PRINT_LVALUE_MEM; }
       ;

member : lvalue DOT ID { PRINT_MEMBER_LID; }
       | lvalue LEFT_BRACKET expr RIGHT_BRACKET { PRINT_MEMBER_LEXPR; }
       | call DOT ID { PRINT_MEMBER_CID; }
       | call LEFT_BRACKET expr RIGHT_BRACKET { PRINT_MEMBER_CEXPR; }
       ;

call : call LEFT_PAR elist RIGHT_PAR { PRINT_CALL; }
     | lvalue callsuffix { PRINT_CALL_LV; }
     | LEFT_PAR funcdef RIGHT_PAR LEFT_PAR elist RIGHT_PAR { PRINT_CALL_FUN; }
     ;

callsuffix : normcall { PRINT_CALLS_N; }
           | methodcall { PRINT_CALLS_M; }
           ;

normcall : LEFT_PAR elist RIGHT_PAR { PRINT_NORMCALL; }
	     ;

methodcall : DOUBLE_DOT ID LEFT_PAR elist RIGHT_PAR { PRINT_METHODCALL; }
           ;

elist : expr comma_exprs { PRINT_ELIST; }
      | /* Empty */      { PRINT_ELIST_E; }
      ;

comma_exprs : comma_exprs COMMA expr { PRINT_COM_EXPS; }
            | /* Empty */            { PRINT_COM_EXPS_E; }
            ;

objectdef : LEFT_BRACKET elist RIGHT_BRACKET { PRINT_OBJDEF; }
          | LEFT_BRACKET indexed RIGHT_BRACKET { PRINT_OBJDEF; }
          ;

indexed : indexedelem comma_indexedelems { PRINT_INDEXED; }
        ;

comma_indexedelems : comma_indexedelems COMMA indexedelem { PRINT_C_IND_ES; }
                   | /* Empty */                          { PRINT_C_IND_ES_E; }
                   ;

indexedelem : LEFT_BRACE expr COLON expr RIGHT_BRACE { PRINT_IND_E; }
            ;

block : LEFT_BRACE { } stmts RIGHT_BRACE { PRINT_BLOCK; }
      ;

funcdef : FUNCTION ID LEFT_PAR idlist RIGHT_PAR block { PRINT_FUNCDEF; }
        | FUNCTION LEFT_PAR idlist RIGHT_PAR block { PRINT_FUNCID_E; }
        ;

const : NUMBER { PRINT_CON_NU; }
      | STRING { PRINT_CON_S; }
      | NIL { PRINT_CON_NI; }
      | TRUE { PRINT_CON_T; }
      | FALSE { PRINT_CON_F; }
      ;

idlist : ID comma_ids { PRINT_IDLIST; }
       | /* Empty */  { PRINT_IDLIST_E; }
       ;

comma_ids : comma_ids COMMA ID { PRINT_C_IDS; }
          | /* Empty */        { PRINT_C_IDS_E; }
          ;

ifstmt : IF LEFT_PAR expr RIGHT_PAR stmt %prec NO_ELSE { PRINT_IF; }
       | IF LEFT_PAR expr RIGHT_PAR stmt ELSE stmt { PRINT_IF_ELSE; }
       ;

whilestmt : WHILE LEFT_PAR expr RIGHT_PAR stmt { PRINT_WHILE; }
          ;

forstmt : FOR LEFT_PAR elist SEMICOLON expr SEMICOLON elist RIGHT_PAR stmt { PRINT_FOR; }
        ;

returnstmt : RETURN expr SEMICOLON { PRINT_RET_S; }
           | RETURN /* Empty */ SEMICOLON { PRINT_RET_V_E; }
           ;

breakstmt : BREAK SEMICOLON { ; }
          ;

continuestmt : CONTINUE SEMICOLON { ; }
             ;

%%

int yyerror (char * yaccProvidedMessage) { }

int main(int argc, char ** argv) {

    /* The Bison parser */
    yyparse();

    std::for_each(tokenList.begin(), tokenList.end(), SyntaxPrinter("alpha_TokenList.txt"));

    return (EXIT_SUCCESS);
}
