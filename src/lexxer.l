%{
#include "Object.h"  /* TODO: Why is this necessary? */
#include "parser.h"
#include "TokenTypes.h"
#include "Token.h"

#include <iostream>
#include <vector>
#include <cstring>

#include <iostream>

/* Counter for the number of opened block comments (at any time) */
int comment_count = 0;

extern std::vector<Token> tokenList;

extern bool maintainTokenList;

/* Gets the type and the attribute of a recognized token and if the appropriate
 * flag is raised it creates a token node and inserts it at the end of the token
 * list */
void HandleToken(const std::string & type, const std::string & attribute) {
  if (maintainTokenList)
    tokenList.push_back(Token(yylineno, tokenList.size() + 1, (type == C_COMMENT ? "" : yytext), type, attribute));
}

/* Same as the previous function but it is used only for strings since it
 * provides the option to determine the content of the token. This is done
 * because strings are modified and are not the same as yytext */
void HandleStringToken(const std::string & content) {
    if (maintainTokenList)
        tokenList.push_back(Token(yylineno, tokenList.size() + 1, content, C_STRING, A_NA));
}

%}

/* Read only one file */
%option noyywrap
/* Use global variable that holds line number */
%option yylineno

/* Regular expressions */
whitespace      [\n\t\r\v ]
integer         [0-9]+
real            [0-9]+\.[0-9]+
string          \"((\\.)|([^\\\"]))*\"
id              [a-zA-Z][a-zA-Z_0-9]*
line_comment    \/\/[^\n]*

/* Exclusive state for when a block comment starts */
%x COMMENT

%%

{whitespace} { /* Eat whitespace */ }

"{" { HandleToken(C_PUNCTUATION, A_LEFT_BRACE); return LEFT_BRACE;}
"}" { HandleToken(C_PUNCTUATION, A_RIGHT_BRACE); return RIGHT_BRACE; }
"[" { HandleToken(C_PUNCTUATION, A_LEFT_BRACKET); return LEFT_BRACKET; }
"]" { HandleToken(C_PUNCTUATION, A_RIGHT_BRACKET); return RIGHT_BRACKET; }
"(" { HandleToken(C_PUNCTUATION, A_LEFT_PARENTHESIS); return LEFT_PAR; }
")" { HandleToken(C_PUNCTUATION, A_RIGHT_PARENTHESIS); return RIGHT_PAR; }
";" { HandleToken(C_PUNCTUATION, A_SEMICOLON); return SEMICOLON; }
"," { HandleToken(C_PUNCTUATION, A_COMMA); return COMMA; }
":" { HandleToken(C_PUNCTUATION, A_COLON); return COLON; }
"::" { HandleToken(C_PUNCTUATION, A_COLON_COLON); return DOUBLE_COLON; }
"." { HandleToken(C_PUNCTUATION, A_DOT); return DOT; }
".." { HandleToken(C_PUNCTUATION, A_DOT_DOT); return DOUBLE_DOT; }

"if" { HandleToken(C_KEYWORD, A_IF); return IF; }
"else" { HandleToken(C_KEYWORD, A_ELSE); return ELSE; }
"while" { HandleToken(C_KEYWORD, A_WHILE); return WHILE; }
"for" { HandleToken(C_KEYWORD, A_FOR); return FOR; }
"function" { HandleToken(C_KEYWORD, A_FUNCTION); return FUNCTION; }
"return" { HandleToken(C_KEYWORD, A_RETURN); return RETURN; }
"break" { HandleToken(C_KEYWORD, A_BREAK); return BREAK; }
"continue" { HandleToken(C_KEYWORD, A_CONTINUE); return CONTINUE; }
"not" { HandleToken(C_KEYWORD, A_NOT); return NOT; }
"local" { HandleToken(C_KEYWORD, A_LOCAL); return LOCAL; }
"true" { HandleToken(C_KEYWORD, A_TRUE); return TRUE; }
"false" { HandleToken(C_KEYWORD, A_FALSE); return FALSE; }
"nil" { HandleToken(C_KEYWORD, A_NIL); return NIL; }

"+" { HandleToken(C_OPERATOR, A_PLUS); return PLUS; }
"-" { HandleToken(C_OPERATOR, A_MINUS); return MINUS; }
"*" { HandleToken(C_OPERATOR, A_MULTIPLY); return MUL; }
"/" { HandleToken(C_OPERATOR, A_DIVIDE); return DIV; }
"%" { HandleToken(C_OPERATOR, A_MODULO); return MODULO; }
"==" { HandleToken(C_OPERATOR, A_EQUAL); return EQUAL; }
"!=" { HandleToken(C_OPERATOR, A_NOT_EQUAL); return NOT_EQUAL; }
">" { HandleToken(C_OPERATOR, A_GREATER); return GREATER; }
"<" { HandleToken(C_OPERATOR, A_LESS); return LESS; }
">=" { HandleToken(C_OPERATOR, A_GREATER_EQUAL); return GREATER_EQUAL; }
"<=" { HandleToken(C_OPERATOR, A_LESS_EQUAL); return LESS_EQUAL; }
"and" { HandleToken(C_KEYWORD, A_AND); return AND; }
"or" { HandleToken(C_KEYWORD, A_OR); return OR; }

"=" { HandleToken(C_OPERATOR, A_ASSIGN); return ASSIGN; }
"++" { HandleToken(C_OPERATOR, A_PLUS_PLUS); return PLUS_PLUS; }
"--" { HandleToken(C_OPERATOR, A_MINUS_MINUS); return MINUS_MINUS; }

{integer} {
  yylval.doubleVal = atof(yytext);
  HandleToken(C_INTCONST, A_NA);
  return NUMBER;
}

{real} {
  yylval.doubleVal = atof(yytext);
  HandleToken(C_REALCONST, A_NA);
  return NUMBER;
}

{id} {
  HandleToken(C_IDENTIFIER, A_NA);
  yylval.strVal = strdup(yytext);
  return ID;
}

{string} {
  unsigned int i, pos;

  /* Allocate enough memory for the string. Make sure that it is set to zero to
   * avoid garbage in the string. This is essential when dealing with escape
   * characters since they change the size of the actual string */
#ifdef MALLOC_AND_MEMSET
  char * content = (char *) malloc(sizeof(char) * yyleng);
  memset(content, 0, sizeof(char) * yyleng);
#else
  char * content = (char *) calloc(yyleng, sizeof(char));
#endif

  /* Iterate through the entire string and copy the characters one after the
   * other. Escape characters are handled appropriately */
  for(i = 0, pos = 1; pos < yyleng-1; ++pos, ++i) {
    if (yytext[pos] != '\\') content[i] = yytext[pos];
    else if (yytext[pos+1] == 'n') {
      content[i] = '\n';
      pos++;
    }
    else if (yytext[pos+1] == '\"') {
      content[i] = '\"';
      pos++;
    }
    else if (yytext[pos+1] == 't') {
      content[i] = '\t';
      pos++;
    }
    else if (yytext[pos+1] == '\'') {
      content[i] = '\'';
      pos++;
    }
    else if (yytext[pos+1] == '\\') {
      content[i] = '\\';
      pos++;
    }
    else {
      content[i] = '\\';
      std::cerr << "Warning: Illegal escape character \\" << yytext[pos+1] << " found" << std::endl;
    }
  }

  /* Make sure that it is a NULL terminated string. Note that yytext alse
   * contains the quotes at the beginning and the end of the string. Thee are
   * discarded at the returned string so the new string size is yyleng-2 */
  content[yyleng-2] = '\0';
  yylval.strVal = content;

  HandleStringToken(content);

  return STRING;
}

{line_comment} { /* Eat lines comments */ HandleToken(C_COMMENT, A_LINE_COMMENT); }

"/*"  { BEGIN(COMMENT); comment_count++; }

<COMMENT>[^*/]* { /* Eat everything that is not the end of a comment */ }
<COMMENT>"*"+[^/] { /* Eat every star that is not followed by the dash */ }
<COMMENT>"/" { /* Eat line comments inside block comments */ }
<COMMENT>"/*" { comment_count++; }
<COMMENT>"*/" { comment_count--; if (comment_count == 0) BEGIN(INITIAL); HandleToken(C_COMMENT, A_BLOCK_COMMENT);}

"\"" {
  /* This rule is ONLY matched when a non-valid string exists. If the
   * string had both quotes at the beginning and the end it would match
   * the STRING rule */
  std::cerr << "Lexical error: EOF found with open string" << std::endl;
  return EOF;
}

. { std::cerr << "Undefined symbol [" << yytext << "] at line " << yylineno << std::endl; }

<<EOF>> {
  /* If the stack is not empty there is a block comment that was opened but not
   * closed and its start line is still inside the stack */
  if (comment_count != 0) std::cerr << "Lexical error: EOF found with open block comment" << std::endl;
  return EOF;
}
