#ifndef _GRAMMAR_RULES_H_
#define _GRAMMAR_RULES_H_

#ifndef SUPPRESS_GRAMMAR_OUTPUT
#define print_rule(x) rulesPrinter(x)
#else
#define print_rule(x) ;
#endif

#define PRINT_STMTS print_rule("stmts -> stmts stmt");
#define PRINT_STMTS_E print_rule("stmts -> e");

#define PRINT_STMT_EXP print_rule("stmt -> expr ;");
#define PRINT_STMT_IF print_rule("stmt -> ifstmt");
#define PRINT_STMT_WH print_rule("stmt -> whilestmt");
#define PRINT_STMT_FOR print_rule("stmt -> forstmt");
#define PRINT_STMT_RET print_rule("stmt -> returnstmt");
#define PRINT_STMT_BR print_rule("stmt -> break ;");
#define PRINT_STMT_CON print_rule("stmt -> continue ;");
#define PRINT_STMT_BLO print_rule("stmt -> block");
#define PRINT_STMT_FUNC print_rule("stmt -> funcdef");
#define PRINT_STMT_SEMI print_rule("stmt -> ;");

#define PRINT_EXPR_ASS print_rule("expr -> assignexpr");
#define PRINT_EXPR_PL print_rule("expr -> expr + expr");
#define PRINT_EXPR_MIN print_rule("expr -> expr - expr");
#define PRINT_EXPR_MUL print_rule("expr -> expr * expr");
#define PRINT_EXPR_DIV print_rule("expr -> expr / expr");
#define PRINT_EXPR_MOD print_rule("expr -> expr mod expr");
#define PRINT_EXPR_GR print_rule("expr -> expr > expr");
#define PRINT_EXPR_LE print_rule("expr -> expr < expr");
#define PRINT_EXPR_GR_E print_rule("expr -> expr >= expr");
#define PRINT_EXPR_LE_E print_rule("expr -> expr <= expr");
#define PRINT_EXPR_EQ print_rule("expr -> expr == expr");
#define PRINT_EXPR_NEQ print_rule("expr -> expr != expr");
#define PRINT_EXPR_AND print_rule("expr -> expr and expr");
#define PRINT_EXPR_OR print_rule("expr -> expr or expr");
#define PRINT_EXPR_TERM print_rule("expr -> term");

#define PRINT_TERM_PAR print_rule("term -> ( expr )");
#define PRINT_TERM_UM print_rule("term -> -expr");
#define PRINT_TERM_NOT print_rule("term -> not expr");
#define PRINT_TERM_P_L print_rule("term -> ++ lvalue");
#define PRINT_TERM_L_P print_rule("term -> lvalue ++");
#define PRINT_TERM_M_L print_rule("term -> -- lvalue");
#define PRINT_TERM_L_M print_rule("term -> lvalue --");
#define PRINT_TERM_PRIM print_rule("term -> primary");

#define PRINT_ASSIGN print_rule("assignexpr -> lvalue = expr");

#define PRINT_PRIMARY_LVAL print_rule("primary -> lvalue");
#define PRINT_PRIMARY_CALL print_rule("primary -> call");
#define PRINT_PRIMARY_OBJ print_rule("primary -> objectdef");
#define PRINT_PRIMARY_FUNC print_rule("primary -> ( funcdef )");
#define PRINT_PRIMARY_CON print_rule("primary -> const");

#define PRINT_LVALUE_ID print_rule("lvalue -> id");
#define PRINT_LVALUE_LID print_rule("lvalue -> local id");
#define PRINT_LVALUE_GID print_rule("lvalue -> ::id");
#define PRINT_LVALUE_MEM print_rule("lvalue -> member");

#define PRINT_MEMBER_LID print_rule("member -> lvalue.id");
#define PRINT_MEMBER_LEXPR print_rule("member -> lvalue[expr]");
#define PRINT_MEMBER_CID print_rule("member -> call.id");
#define PRINT_MEMBER_CEXPR print_rule("member -> call[expr]");

#define PRINT_CALL print_rule("call -> call (elist)");
#define PRINT_CALL_LV print_rule("call -> lvalue callsuffix");
#define PRINT_CALL_FUN print_rule("call -> (funcedf)(elist)");

#define PRINT_CALLS_N print_rule("callsuffix -> normcall");
#define PRINT_CALLS_M print_rule("callsuffix -> methodcall");

#define PRINT_NORMCALL print_rule("normcall -> (elist)");

#define PRINT_METHODCALL print_rule("methodcall -> ..id(elist)");

#define PRINT_ELIST print_rule("elist -> expr comma_exprs");
#define PRINT_ELIST_E print_rule("elist -> e");

#define PRINT_COM_EXPS print_rule("comma_exprs -> comma_exprs comma_expr");
#define PRINT_COM_EXPS_E print_rule("comma_exprs -> e");

#define PRINT_COM_EXP print_rule("comma_expr -> , expr");

#define PRINT_OBJDEF print_rule("objectdef -> [ objectdef_val ]");

#define PRINT_OBJ_VAL_E print_rule("objectdef_val -> elist");
#define PRINT_OBJ_VAL_I print_rule("objectdef_val -> indexed");

#define PRINT_INDEXED print_rule("indexed -> indexedelem comma_indexedelems");

#define PRINT_C_IND_ES print_rule("comma_indexedelems -> comma_indexedelems comma_indexedelem");
#define PRINT_C_IND_ES_E print_rule("comma_indexedelems -> e");

#define PRINT_C_IND_E print_rule("comma_indexedelem -> , indexedelem");

#define PRINT_IND_E print_rule("indexedelem -> [ expr : expr ]");

#define PRINT_BLOCK print_rule("block -> { stmts }");

#define PRINT_FUNCDEF print_rule("funcdef -> function func_id ( idlist ) block");

#define PRINT_FUNCID print_rule("func_id -> id");
#define PRINT_FUNCID_E print_rule("func_id -> e");

#define PRINT_CON_NU print_rule("const -> number");
#define PRINT_CON_S print_rule("const -> string");
#define PRINT_CON_NI print_rule("const -> nill");
#define PRINT_CON_T print_rule("const -> true");
#define PRINT_CON_F print_rule("const -> false");

#define PRINT_IDLIST print_rule("idlist -> id comma_ids");
#define PRINT_IDLIST_E print_rule("idlist -> id comma_ids");

#define PRINT_C_IDS print_rule("comma_ids -> comma_ids comma_id");
#define PRINT_C_IDS_E print_rule("comma_ids -> e");

#define PRINT_C_ID print_rule("comma_id -> , id");

#define PRINT_IF print_rule("ifstmt -> if ( expr ) stmt");
#define PRINT_IF_ELSE print_rule("ifstmt -> if ( expr ) stmt else stmt");

#define PRINT_WHILE print_rule("whilestmt -> while ( expr ) stmt");

#define PRINT_FOR print_rule("forstmt -> for ( elist ; expr ; elist ) stmt");

#define PRINT_RET_S print_rule("returnstmt -> return returnvalue ;");

#define PRINT_RET_V print_rule("returnvalue -> expr");
#define PRINT_RET_V_E print_rule("returnvalue -> e");

#endif
