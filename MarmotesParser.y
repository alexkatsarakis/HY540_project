%{
	#include <string>
	#include <cassert>
	#include <cstdio>
	#include <sstream>
	#include <stack>
	#include <iostream>

	// #include "SymTable.h"
	// #include "Quad.h"
	// #include "ManagerFunctions.h"

	// #include "ConstTables.h"
	// #include "Instruction.h"
	// #include "GeneratorFunctions.h"

	using namespace std;

	void SymError(string msg);
	void yyerror(const char* yaccProvidedMessage);
	int yylex();

	extern int yylineno;
	extern char* yytext;
	extern FILE* yyin;

	unsigned currScope = 0;
	extern stack<unsigned> scopeLimit; // na fygei!
	// aFunction only serves the Faceless God

	/* SymTable symTable;
	Quads quads;
	Instructions instructions;
	RealConstTable realConstTable;
	IntConstTable intConstTable;
	StringConstTable stringConstTable;
	LibFuncsTable libFuncsTable;
	UserFuncsTable userFuncsTable; */

%}

%union {
	int intValue;
	double realValue;
	string* stringValue;
	/* Stmt* stmtNode;
	Expr* exprNode;
	SymTableEntry* symbol;
	ForPrefix* forprefixValue;
	CallType* callType; */
}

%expect 1
%start program

%token  EQ NE LT GT GE LE
%token  PLUSPLUS MINUSMINUS DOTDOT COLONCOLON
%token '=' '+' '-' '*' '/' '%'
%token '{' '}' '[' ']' '(' ')' ';' ',' ':' '.'

%token  IF ELSE WHILE FOR FUNCTION RETURN BREAK CONTINUE
%token  AND NOT OR LOCAL TRUE FALSE NIL


%token  <stringValue>	STRING
%token  <intValue> 		INTEGER
%token  <realValue> 	REAL
%token  <stringValue>  	ID

/* %type <stmtNode> statements
%type <stmtNode> stmt
%type <stmtNode> ifstmt
%type <stmtNode> whilestmt
%type <stmtNode> forstmt
%type <stmtNode> returnstmt
%type <stmtNode> block
%type <exprNode> expr
%type <exprNode> exprBoolChecked
%type <exprNode> lvalue
%type <exprNode> member
%type <exprNode> primary
%type <exprNode> assignexpr
%type <exprNode> call
%type <exprNode> term
%type <exprNode> objectdef
%type <exprNode> const
%type <exprNode> elist
%type <exprNode> commaexpr
%type <exprNode> indexedelem
%type <exprNode> commaielem
%type <stringValue> funcname
%type <stmtNode> funcbody
%type <symbol> funcprefix
%type <symbol> funcdef
%type <intValue> ifprefix
%type <intValue> elseprefix
%type <stmtNode> loopstmt
%type <intValue> whilestart
%type <intValue> whilecond
%type <forprefixValue> forprefix
%type <intValue> N
%type <intValue> M
%type <stmtNode> break
%type <stmtNode> continue
%type <callType> callsuffix
%type <callType> normcall
%type <callType> methodcall */


// %destructor {cout << "Destructor called on token STRING" << endl; delete $$;} STRING
// %destructor {cout << "Destructor called on token ID" << endl; delete $$;} ID
//%destructor {cout << "Destructor called on type lvalue" << endl; delete $$;} lvalue


%right 		'='
%left 		OR
%left 		AND
%nonassoc	EQ NE
%nonassoc	LT GT GE LE
%left 		'+' '-'
%left		'*' '/' '%'
%right		NOT PLUSPLUS MINUSMINUS UMINUS
%left		'.' DOTDOT
%left		'[' ']'
%left		'(' ')'


%%
program: 		statements 					{ Manage_program(); }
				|/*empty*/
				;

statements: 	statements stmt 			{ Manage_statements_statements_stmt(&($$), $1, $2); }
				| stmt						{ Manage_statements_stmt(&($$), $1); }
				;

stmt:			expr ';'					{ Manage_stmt_expr(&($$), $1); }
				| ifstmt					{ Manage_stmt_ifstmt(&($$), $1); }
				| whilestmt					{ Manage_stmt_whilestmt(&($$), $1); }
				| forstmt					{ Manage_stmt_forstmt(&($$), $1); }
				| returnstmt				{ Manage_stmt_returnstmt(&($$), $1); }
				| break ';'					{ Manage_stmt_break(&($$), $1); }
				| continue ';'				{ Manage_stmt_continue(&($$), $1); }
				| block						{ Manage_stmt_block(&($$), $1); }
				| funcdef					{ Manage_stmt_funcdef(&($$));}
				| ';'						{ Manage_stmt_semicolon(&($$)); }
				| error ';'					{cout << "NOMANAGER_stmt-->ERROR ;" << endl; yyclearin;}
				| error ')'					{cout << "NOMANAGER_stmt-->ERROR )" << endl; yyclearin;}
				;

expr:			assignexpr					{ Manage_expr_assignexpr(&($$), $1); }
				| expr '+' expr				{ Manage_expr_expr1_arithop_expr2(&($$), $1, Iopcode::add, $3); }
				| expr '-' expr				{ Manage_expr_expr1_arithop_expr2(&($$), $1, Iopcode::sub, $3); }
				| expr '/' expr				{ Manage_expr_expr1_arithop_expr2(&($$), $1, Iopcode::div, $3); }
				| expr '*' expr				{ Manage_expr_expr1_arithop_expr2(&($$), $1, Iopcode::mul, $3); }
				| expr '%' expr				{ Manage_expr_expr1_arithop_expr2(&($$), $1, Iopcode::mod, $3); }
				| expr GT expr				{ Manage_expr_expr1_relop_expr2(&($$), $1, Iopcode::if_greater, $3); }
				| expr GE expr				{ Manage_expr_expr1_relop_expr2(&($$), $1, Iopcode::if_greatereq, $3); }
				| expr LT expr				{ Manage_expr_expr1_relop_expr2(&($$), $1, Iopcode::if_less, $3); }
				| expr LE expr				{ Manage_expr_expr1_relop_expr2(&($$), $1, Iopcode::if_lesseq, $3); }
				| expr EQ	{ Manage_Finish_Eval_Emition(&($1)); } expr { Manage_expr_expr1_relop_expr2(&($$), $1, Iopcode::if_eq, $4); }
				| expr NE	{ Manage_Finish_Eval_Emition(&($1));} expr { Manage_expr_expr1_relop_expr2(&($$), $1, Iopcode::if_noteq, $4); }
				| expr AND 	{ Manage_boolop_expr1(&($1)); } M expr { Manage_expr_expr1_AND_expr2(&($$), $1, $4, $5); }
				| expr OR 	{ Manage_boolop_expr1(&($1)); } M expr { Manage_expr_expr1_OR_expr2(&($$), $1, $4, $5); }
				| term						{ Manage_expr_term(&($$), $1); }
				;

term:			'(' expr ')'				{ Manage_Term_Parenthesis_Expr(&($$), $2);}
				| '-' expr %prec UMINUS		{ Manage_Term_Minus_Expr(&($$), $2);}
				| NOT expr					{ Manage_Term_Not_Expr(&($$), $2);}
				| PLUSPLUS lvalue			{ Manage_Term_Plusplus_Lvalue(&($$), $2); }
				| lvalue PLUSPLUS			{ Manage_Term_Lvalue_Plusplus(&($$), $1); }
				| MINUSMINUS lvalue			{ Manage_Term_Minusminus_Lvalue(&($$), $2);}
				| lvalue MINUSMINUS			{ Manage_Term_Lvalue_Minusminus(&($$), $1);}
				| primary					{ Manage_Term_Primary(&($$), $1);}
				;

assignexpr:		lvalue '=' expr				{ Manage_Assignexpr(&($$), $1, $3); }
				;

primary:		lvalue						{ Manage_Primary_Lvalue(&($$), $1); }
				| call						{ Manage_Primary_Call(&($$), $1); }
				| objectdef					{ Manage_Primary_Objectdef(&($$), $1); }
				| '(' funcdef ')'			{ Manage_Primary_Funcdef(&($$), $2);}
				| const						{ Manage_Primary_Const(&($$), $1); }
				;

lvalue:			ID							{ Manage_Lvalue_ID(&($$), $1); }
				| LOCAL ID					{ Manage_Lvalue_LOCAL_ID(&($$), $2); }
				| COLONCOLON ID				{ Manage_Lvalue_COLONCOLON_ID(&($$), $2); }
				| member					{ Manage_Lvalue_Member(&($$), $1); }
				;

member:			lvalue '.' ID				{ Manage_Member_Lvalue_Dot_ID(&($$), $1, $3); }
				| lvalue '[' expr ']'		{ Manage_Member_Lvalue_Brackets_Expr(&($$), $1, $3); }
				| call '.' ID				{ Manage_Member_Call_Dot_ID(&($$), $1, $3);}
				| call '[' expr ']'			{ Manage_Member_Call_Brackets_Expr(&($$), $1, $3);}
				;

call:			call '(' elist ')'				{ Manage_Call_Call_Elist(&($$), $1, $3);}
				| lvalue callsuffix				{ Manage_Call_Lvalue_Callsuffix(&($$), $1, $2);}
				| '(' funcdef ')' '(' elist ')'	{ Manage_Call_Funcdef_Elist(&($$), $2, $5);}
				;

callsuffix:		normcall					{ Manage_Callsuffix_Normcall(&($$), $1);}
				| methodcall				{ Manage_Callsuffix_Methodcall(&($$), $1);}
				;

normcall:		'(' elist ')'				{ Manage_Normcall(&($$), $2);}
				;

methodcall:		DOTDOT ID '(' elist ')'		{ Manage_Methodcall(&($$), $2, $4); }
				;

elist:			exprBoolChecked commaexpr	{ Manage_Elist_Expr_Commaexpr(&($$), $1, $2 ); }
				| /*empty*/					{ Manage_Elist_e(&($$)); }
				;

commaexpr:		',' exprBoolChecked commaexpr	{ Manage_Commaexpr_Comma_Expr_Commaexpr(&($$), $2, $3 ); }
				| /*empty*/						{ Manage_Commaexpr_e(&($$)); }
				;

objectdef:		'[' elist ']'						{ Manage_Objectdef_Elist(&($$), $2);}
				| '[' indexedelem commaielem ']'	{ Manage_Objectdef_Index(&($$), $2, $3);}
				;


commaielem:		',' indexedelem commaielem	{ Manage_Commaielem_Indexedelem_Commaielem(&($$), $2, $3); }
				| /*empty*/					{ Manage_Commaielem_e(&($$));}
				;

indexedelem:	'{' exprBoolChecked ':' exprBoolChecked'}'		{ Manage_Indexedelem(&($$), $2, $4);}
				;

exprBoolChecked: 	expr						{ Manage_ExprBoolChecked(&($$), $1); }
					;


block:			'{' {currScope++;} statements '}'{symTable.Hide(currScope--);
												Manage_block_statements(&($$), $3); }
				|'{' {currScope++;} '}'			{symTable.Hide(currScope--);
												Manage_block_empty(&($$)); }
				;

funcname:		ID 							{ Manage_funcname_ID(&($$), $1); }
				| /*empty*/					{ Manage_funcname_e(&($$)); }
				;

funcprefix:		FUNCTION funcname 			{ Manage_funcprefix(&($$), $2); }
				;

funcargs:		'(' idlist ')'				{ Manage_funcargs(); }
				;

funcbody:		block						{ Manage_funcbody(&($$), $1); }
				;

funcblockstart:	/*empty*/							{ Manage_funcblockstart(); }
				;

funcblockend:	/*empty*/							{ Manage_funcblockend(); }
				;

funcdef:		funcprefix funcargs funcblockstart funcbody funcblockend { Manage_funcdef(&($$), $1, $4); }
				;


const:			INTEGER						{ Manage_INTEGER(&($$), $1); }
				| REAL						{ Manage_REAL(&($$), $1); }
				| STRING					{ Manage_STRING(&($$), *($1)); }
				| NIL						{ Manage_NIL(&($$)); }
				| TRUE						{ Manage_TRUE(&($$)); }
				| FALSE						{ Manage_FALSE(&($$)); }
				;

idlist:			argId commaid				{ Manage_idList(); }
				| /*empty*/					{ Manage_idList_empty(); }
				;

argId:			ID							{ Manage_argID($1); }
				;

commaid:		',' argId commaid			{ Manage_commaid(); }
				| /*empty*/					{ Manage_commaid_empty(); }
				;

ifprefix:		IF '(' expr ')'				{ Manage_ifprefix_ifexpr(&($$), $3); }
				;

elseprefix:		ELSE						{ Manage_elseprefix_else(&($$)); }
				;

ifstmt:			ifprefix stmt					{ Manage_ifstmt_ifprefix_stmt(&($$), &($1), $2); }
				|ifprefix stmt elseprefix stmt	{ Manage_ifstmt_ifprefix_stmt1_elseprefix_stmt2(&($$), &($1), $2, &($3), $4); }
				;

loopstart:									{ Manage_loopstart(); }
				;

loopend:									{ Manage_loopend(); }
				;

loopstmt:		loopstart stmt loopend		{ Manage_loopstmt(&($$), $2); }
				;

whilestart:		WHILE						{ Manage_whilestart(&($$)); }
				;

whilecond:		'(' expr ')'				{ Manage_whilecond(&($$), $2); }
				;

whilestmt:		whilestart whilecond loopstmt	{ Manage_whilestmt(&($$), $1, $2, $3); }
				;

N:											{ Manage_N(&($$)); }
				;

M:											{ Manage_M(&($$)); }
				;

forprefix:		FOR '(' elist ';' M expr ';'		{ Manage_forprefix(&($$), $5, $6); }
				;

forstmt:		forprefix N elist ')' N loopstmt N	{ Manage_forstmt(&($$), $1, $2, $5, $6, $7); }
				;

break:			BREAK						{ Manage_break(&($$)); }
				;

continue:		CONTINUE					{ Manage_continue(&($$)); }
				;

returnstmt:		RETURN ';'					{ Manage_returnstmt(&($$)); }
				| RETURN expr ';'			{ Manage_returnstmt_expr(&($$), $2); }
				;

%%

void SymError (string msg) {
	cout << endl << "ERROR: " << msg << " at line "<< yylineno << ", before token " << yytext << endl <<endl;
	exit(1);
}

void yyerror(const char* yaccProvidedMessage) {
	cerr << yaccProvidedMessage << " at line "<< yylineno << ", before token " << yytext << endl;
	cerr << "INPUT NOT VALID" << endl;
	exit(1);
}

int main (int argc, char** argv) {
	if (argc != 2) {
		cerr << "Wrong number of arguments" << endl;
		return 1;
	}
	if (!(yyin = fopen(argv[1], "r"))) {
		cerr << "Error in opening input file" << endl;
		return 1;
	}

	symTable.AddLibFunc(new SymTableEntry("print", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("input", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("objectmemberkeys", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("objecttotalmembers", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("objectcopy", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("totalarguments", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("argument", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("typeof", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("strtonum", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("sqrt", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("cos", SymType::LIBFUNC, 0, 0 ));
	symTable.AddLibFunc(new SymTableEntry("sin", SymType::LIBFUNC, 0, 0 ));

	yyparse();

	// symTable.PrintSymTable();
	// cout << endl << endl;

	// quads.print();
	// cout << endl << endl;

	// generateTargetCode();

	// libFuncsTable.print();
	// userFuncsTable.print();
	// stringConstTable.print();
	// realConstTable.print();
	// intConstTable.print();

	// instructions.print();
	// cout << endl << endl;

	// instructions.toFile("../Phase5/target.xena");

	fclose(yyin);
	return 0;
}
