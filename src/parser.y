%{
    #include "Token.h"
    #include "SyntaxPrinter.h"
    #include "GrammarRules.h"

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
    char * strVal;
    double doubleVal;
}

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

program : stmts { printf(">> Parsing complete\n"); }
        ;

stmts : stmts stmt  { PRINT_STMTS; }
      | /* Empty */ { PRINT_STMTS_E; }
      ;

stmt : expr SEMICOLON { PRINT_STMT_EXP; }
     | ifstmt { PRINT_STMT_IF; }
     | whilestmt { PRINT_STMT_WH; }
     | forstmt { PRINT_STMT_FOR; }
     | returnstmt { PRINT_STMT_RET; }
     | BREAK SEMICOLON { PRINT_STMT_BR; }
     | CONTINUE SEMICOLON { PRINT_STMT_CON; }
     | block { PRINT_STMT_BLO; }
     | funcdef { PRINT_STMT_FUNC; }
     | SEMICOLON { PRINT_STMT_SEMI; }
     | error SEMICOLON { yyerrok; yyclearin; }
     ;

expr : assignexpr { PRINT_EXPR_ASS; }
     | expr PLUS expr {PRINT_EXPR_PL; }
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

assignexpr : lvalue ASSIGN expr { PRINT_ASSIGN; }
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

methodcall : DOUBLE_DOT methodcall_id LEFT_PAR elist RIGHT_PAR { PRINT_METHODCALL; }
           ;

methodcall_id : ID { }
              ;

elist : expr comma_exprs { PRINT_ELIST; }
      | /* Empty */      { PRINT_ELIST_E; }
      ;

comma_exprs : comma_exprs comma_expr { PRINT_COM_EXPS; }
            | /* Empty */            { PRINT_COM_EXPS_E; }
            ;

comma_expr : COMMA expr { PRINT_COM_EXP; }
           ;

objectdef : LEFT_BRACKET objectdef_val RIGHT_BRACKET { PRINT_OBJDEF; }
          ;

objectdef_val : elist   { PRINT_OBJ_VAL_E; }
              | indexed { PRINT_OBJ_VAL_I; }
              ;

indexed : indexedelem comma_indexedelems { PRINT_INDEXED; }
        ;

comma_indexedelems : comma_indexedelems comma_indexedelem { PRINT_C_IND_ES; }
                   | /* Empty */                          { PRINT_C_IND_ES_E; }
                   ;

comma_indexedelem : COMMA indexedelem { PRINT_C_IND_E; }
                  ;

indexedelem : LEFT_BRACE expr COLON expr RIGHT_BRACE { PRINT_IND_E; }
            ;

block : LEFT_BRACE { } stmts RIGHT_BRACE { PRINT_BLOCK; }
      ;

funcdef : FUNCTION func_id LEFT_PAR { } idlist { } RIGHT_PAR funcblock { PRINT_FUNCDEF; }
        ;

funcblock : { } block { }
          ;

func_id : ID { PRINT_FUNCID; }
        | /* Empty */ { PRINT_FUNCID_E; }
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

comma_ids : comma_ids comma_id { PRINT_C_IDS; }
          | /* Empty */        { PRINT_C_IDS_E; }
          ;

comma_id : COMMA ID { PRINT_C_ID; }
         ;

ifstmt : ifcond stmt %prec NO_ELSE { PRINT_IF; }
       | ifcond stmt ELSE { PRINT_IF_ELSE; }
       ;

ifcond : IF LEFT_PAR expr RIGHT_PAR { }
       ;

whilestmt : whilestart whilecond stmt { PRINT_WHILE; }
          ;

whilestart : WHILE { }
           ;

whilecond : LEFT_PAR expr RIGHT_PAR { }
          ;

forstmt : forprefix elist RIGHT_PAR stmt { PRINT_FOR; }
        ;

forprefix : FOR LEFT_PAR elist SEMICOLON expr SEMICOLON { }
          ;

returnstmt : RETURN returnvalue SEMICOLON { PRINT_RET_S; }
           ;

returnvalue : expr { PRINT_RET_V; }
            | /* Empty */ { PRINT_RET_V_E; }
            ;

%%

int yyerror (char * yaccProvidedMessage) { }

int main(int argc, char ** argv) {

    /* The Bison parser */
    yyparse();

    std::for_each(tokenList.begin(), tokenList.end(), SyntaxPrinter("alpha_TokenList.txt"));

    return (EXIT_SUCCESS);
}
