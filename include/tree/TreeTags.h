#ifndef _TREE_TAGS_H_
#define _TREE_TAGS_H_

#define AST_TAG_TYPE_KEY "type"

#define AST_TAG_PROGRAM "program"

#define AST_TAG_STMTS "stmts"
#define AST_TAG_STMT "stmt"

#define AST_TAG_ID "id"
#define AST_TAG_CHILD "child"

#define AST_TAG_EXPR "expr"

#define AST_TAG_IF "if"
#define AST_TAG_ELSE_STMT "else"
#define AST_TAG_CONDITION "condition"

#define AST_TAG_RETURN "return"
#define AST_TAG_RETURN_VALUE "return_value"

#define AST_TAG_WHILE "while"

#define AST_TAG_FOR "for"
#define AST_TAG_FOR_PRE_ELIST "pre_elist"
#define AST_TAG_FOR_POST_ELIST "post_elist"

#define AST_TAG_FUNCTION "func"
#define AST_TAG_FUNCTION_ID "func_id"

#define AST_TAG_FUNCTION_DEF "func_def"
#define AST_TAG_FUNCTION_FORMALS "func_formals"

#define AST_TAG_BLOCK "block"

#define AST_TAG_BREAK "break"

#define AST_TAG_CONTINUE "continue"

#define AST_TAG_EMPTY "empty"

#define AST_TAG_LVALUE "lvalue"
#define AST_TAG_RVALUE "rvalue"
#define AST_TAG_FIRST_EXPR "fexpr"
#define AST_TAG_SECOND_EXPR "sexpr"

#define AST_TAG_ASSIGN "="
#define AST_TAG_PLUS "+"
#define AST_TAG_MINUS "-"
#define AST_TAG_MUL "*"
#define AST_TAG_DIV "/"
#define AST_TAG_MODULO "%"
#define AST_TAG_GREATER ">"
#define AST_TAG_LESS "<"
#define AST_TAG_GEQUAL ">="
#define AST_TAG_LEQUAL "<="
#define AST_TAG_EQUAL "=="
#define AST_TAG_NEQUAL "!="
#define AST_TAG_AND "&&"
#define AST_TAG_OR "||"

#define AST_TAG_TERM "term"

#define AST_TAG_UMINUS "uminus"
#define AST_TAG_NOT "not"
#define AST_TAG_BPLUSPLUS "++_"
#define AST_TAG_APLUSPLUS "_++"
#define AST_TAG_BMINUSMINUS "--_"
#define AST_TAG_AMINUSMINUS "_--"

#define AST_TAG_PRIMARY "primary"

#define AST_TAG_ARGLIST "arglist"
#define AST_TAG_NAMED "named"
#define AST_TAG_NAMED_KEY "named_key"
#define AST_TAG_NAMED_VALUE "named_value"

#define AST_TAG_ELIST "elist"

#define AST_TAG_OBJECTDEFLIST "object_def_list"
#define AST_TAG_OBJECTDEFINDEX "object_def_index"
#define AST_TAG_INDEXES "indexes"

#define AST_TAG_CONST "const"

#define AST_TAG_VALUE "value"

#define AST_TAG_NUMBER "number"
#define AST_TAG_STRING "string"
#define AST_TAG_NIL "nil"
#define AST_TAG_TRUE "true"
#define AST_TAG_FALSE "false"

#define AST_TAG_LOCAL_ID "local_id"
#define AST_TAG_DOUBLECOLON_ID "doublecolon_id"
#define AST_TAG_DOLLAR_ID "dollar_id"
#define AST_TAG_DOLLAR_LAMBDA "dollar_lambda" //?!

#define AST_TAG_MEMBER "member"

#define AST_TAG_DOT "."
#define AST_TAG_BRACKET "[ ]"

#define AST_TAG_ARGUMENTS "arguments"

#define AST_TAG_CALL_SUFFIX "call_suffix"
#define AST_TAG_NORMAL_CALL "normal_call"
#define AST_TAG_METHOD_CALL "method_call"
#define AST_TAG_CALL "call"

#define AST_TAG_SUFFIX "suffix"

#define AST_TAG_COMMA_EXPRS "comma_exprs"

#define AST_TAG_OBJECT_DEF "object_def"
#define AST_TAG_OBJECT_DEF_VAL "object_def_val"

#define AST_TAG_INDEXED "indexed"
#define AST_TAG_INDEXED_ELEM "indexed_elem"
#define AST_TAG_COMMA_INDEXED_ELEMS "comma_indexed_elems"

#define AST_TAG_OBJECT_KEY "object_key"
#define AST_TAG_OBJECT_VALUE "object_value"

#define AST_TAG_ID_LIST "id_list"
#define AST_TAG_COMMA_IDS "comma_ids"
#define AST_TAG_FORMAL "formal"

#endif
