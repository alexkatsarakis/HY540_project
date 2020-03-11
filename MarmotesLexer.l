/*DEFINITIONS*/

/*user code*/

%{
	#include <iostream>
	#include <string>
	#include <cassert>
	#include <list>
	#include <cstdio>
	#include <stack>
	#include <queue>
	#include <sstream>

   	/*#define YY_DECL int alpha_yylex(void* yyval)*/

	using namespace std;
	enum class Category
	{
		KEYWORD_IF, KEYWORD_ELSE, KEYWORD_WHILE, KEYWORD_FOR, KEYWORD_FUNCTION,
		KEYWORD_RETURN, KEYWORD_BREAK, KEYWORD_CONTINUE, KEYWORD_AND, KEYWORD_NOT,
		KEYWORD_OR, KEYWORD_LOCAL, KEYWORD_TRUE, KEYWORD_FALSE, KEYWORD_NIL,
		OP_EQUAL, OP_PLUS, OP_MINUS, OP_MUL, OP_DIV, OP_PERCENT, OP_ASSIGN,
		OP_NOT_EQUAL, OP_PLUS_PLUS, OP_MINUS_MINUS, OP_G, OP_L, OP_GE, OP_LE,
		CONST_INT, CONST_REAL, STRINGCAT, PUNC_L_SQ_BRACKET, PUNC_R_SQ_BRACKET, //changed STRING->STRINGCAT
		PUNC_L_CU_BRACKET, PUNC_R_CU_BRACKET, PUNC_L_PAREN, PUNC_R_PAREN, PUNC_SEMICOLON,
		PUNC_COMMA, PUNC_COLON, PUNC_DOUBLE_COLON, PUNC_DOT, PUNC_DOUBLE_DOT, ERROR, ID,
		COMMENT_LINE, COMMENT_BLOCK, UNCLOSED_COMMENT, NESTED_COMMENT, UNDEFINED_CHAR,
		UNCLOSED_STRING, INVALID_ESC_CHAR
	};
	// #include "SymTable.h"
	// #include "Quad.h"
	#include "MarmotesParser.h"

	struct marmotes_token_t {
	public:
		unsigned int line_no;
		unsigned int token_no;
		string token_content;
		Category token_category;
		string token_print;

		marmotes_token_t() {
			this->line_no = 0;
			this->token_no = 0;
			this->token_content = "";
			this->token_category = Category::ERROR;
			this->token_print = "";

		}

		marmotes_token_t(unsigned int line_no, unsigned int token_no, const char* token_content, Category token_category, const char* token_print) {
			assert(line_no > 0);
			assert(token_no > 0);
			assert(token_content != NULL);
			assert(token_print != NULL);
			this->line_no = line_no;
			this->token_no = token_no;
			this->token_content = token_content;
			this->token_category = Category::ERROR;
			this->token_print = token_print;

		}

		void set_values(unsigned int line_no, const char* token_content, Category token_category, const char* token_print) {
			assert(line_no > 0);
			assert(token_content != NULL);
			assert(token_print != NULL);
			this->line_no = line_no;
			this->token_no = token_no;
			this->token_content = token_content;
			this->token_category = Category::ERROR;
			this->token_print = token_print;

		}

		void empty_values() {
			this->line_no = 0;
			this->token_no = 0;
			this->token_content = "";
			this->token_category = Category::ERROR;
			this->token_print = "";

		}

		~marmotes_token_t() {}

	};



	void print_list(list <marmotes_token_t> token_list, FILE* fout) {

		if(token_list.empty()) {
			fprintf(fout, "No Token was identified\n");
			//fout << "No Token was identified" << endl;
			return;
		}
		for (auto x : token_list) {
			fprintf(fout, "%u:\t#%u\t\"%s\" %s\n", x.line_no, x.token_no, x.token_content.c_str(), x.token_print.c_str());
		}
	}

%}

/*options*/
%option noyywrap
%option yylineno

/*regular expressions*/
undefined_chars		[\\!@#$\^\&`~'?|_]

white_space		[ \n\t\r]

integer			[0-9]+

real			{integer}\.{integer}((E|e){integer})?

id			[a-zA-Z][_a-zA-Z0-9]*

line_comment		"//".*


/*user conditions*/

/*RULES*/
%%

	static int comments_left = 0;
	static stack<unsigned int> left_coms;
	static queue<string> com_limits;
	static queue<unsigned int> com_start;

	if(comments_left){

			comments_left --;
			unsigned int start = com_start.front();
			string limits = com_limits.front();
			com_limits.pop();
			com_start.pop();
			Category comment_type = (!comments_left)?(Category::COMMENT_BLOCK):(Category::NESTED_COMMENT);
			string print = (!comments_left)?("COMMENT BLOCK_COMMENT  <-- enumerated"):("COMMENT NESTED_COMMENT  <-- enumerated");
			/*((marmotes_token_t*)yyval)->set_values(start, limits.c_str(), comment_type, print.c_str());
			return 1;*/
	}

"/*"	{
		left_coms.push(yylineno);
		int balance_count = 1;
		int c;
		int comment_count = 1;
		while(((c = yyinput()) != EOF) && (c !=0)){
			if(c == '*'){
				if ((c = yyinput()) == '/'){
					balance_count--;
					unsigned int start = left_coms.top();
					left_coms.pop();
					ostringstream oss;
					oss << start << " - " << yylineno;
					com_start.push(start);
					com_limits.push(oss.str());
					if(!balance_count)
						break; 	//found the end of outer comment
				}
				else
					unput(c);

			}
			else if(c == '/'){
				if((c = yyinput()) == '*'){
					balance_count ++;
					comment_count ++;
					left_coms.push(yylineno);
				}
				else
					unput(c);

			}
		}

		if(!balance_count){
			comments_left = comment_count -1;
			unsigned int start = com_start.front();
			string limits = com_limits.front();
			com_limits.pop();
			com_start.pop();
			Category comment_type = (!comments_left)?(Category::COMMENT_BLOCK):(Category::NESTED_COMMENT);
			/*string print = (!comments_left)?("COMMENT BLOCK_COMMENT  <-- enumerated"):("COMMENT NESTED_COMMENT  <-- enumerated");*/
			/*((marmotes_token_t*)yyval)->set_values(start, limits.c_str(), comment_type, print.c_str()); */
			/*return 1;*/

		}
		else{
			unsigned int start = left_coms.top();
			left_coms.pop();
			/*((marmotes_token_t*)yyval)->set_values(start, " - ", Category::UNCLOSED_COMMENT, "UNCLOSED COMMENT!");*/
			while(!left_coms.empty()) left_coms.pop();
			while(!com_limits.empty()) com_limits.pop();
			while(!com_start.empty()) com_start.pop();
			/*return 1;*/
			cerr<< "Unclosed comment in line " << start << endl;
			exit(EXIT_FAILURE);
		}
	}

"\"" {
	Category cat = Category::STRINGCAT;
	int my_line = yylineno;
	string err="";
	string errchar="";
	string msg;
	string str;
	int c = -1;
	while ((c = yyinput()) != EOF && c != 0 && c != '\"') {

		if (c == '\\') {
			switch (c = yyinput()) {
				case 'a': str.push_back('\a'); break;
				case 'b': str.push_back('\b'); break;
				case 'f': str.push_back('\f'); break;
				case 'n': str.push_back('\n'); break;
				case 'r': str.push_back('\r'); break;
				case 't': str.push_back('\t'); break;
				case 'v': str.push_back('\v'); break;
				case '\\': str.push_back('\\'); break;
				case '\'': str.push_back('\''); break;
				case '\?': str.push_back('\?'); break;
				case '\"': str.append("\\\""); break;
				case EOF: case 0: break;
				default: {
					str.push_back('\\');
					str.push_back((char)c);
					errchar.push_back('\\');
					errchar.push_back((char)c);
					cerr << "(string) Invalid escape character " << errchar << " in line: "<< my_line << endl;
					cat = Category::INVALID_ESC_CHAR;
					err += "INVALID_ESC_CHAR(" + errchar + ") ";
					//break;
					exit(EXIT_FAILURE);
				}

			}
		}
		else {
			str.push_back(c);
		}

	}

	if (c == EOF || c == 0) {
		cerr << "(string) Detected non-terminating string in line: "<< my_line << endl;
		cat = Category::UNCLOSED_STRING;
		err += "UNCLOSED ";
		msg = err + "STRING " + "\"" + str + "\" <-- std::string ";
		//((marmotes_token_t*)yyval)->set_values(my_line, str.c_str(), cat, msg.c_str());
		/*return 1;*/
		exit(EXIT_FAILURE);
	}
	//else string has ended
	msg = err + "STRING " + "\"" + str + "\" <-- std::string ";
	//((marmotes_token_t*)yyval)->set_values(my_line, str.c_str(), cat, msg.c_str());
	yylval.stringValue = new string(str); return STRING;
	/*return 1;*/
}

{white_space}	{}

{undefined_chars}	{ cerr << "Detected undefined chars\n"; exit(EXIT_FAILURE); }

{integer}	{ yylval.intValue = atoi(yytext); return INTEGER; }

{real}		{ yylval.realValue = atof(yytext); return REAL; }

{line_comment}		{ }


"if" { return IF;}

"else" { return ELSE; }

"while" { return WHILE;}

"for" { return FOR;}

"function" { return FUNCTION; }

"return" { return RETURN; }

"break" { return BREAK; }

"continue" { return CONTINUE; }

"and" { return AND; }

"not" { return NOT;}

"or" { return OR; }

"local" { return LOCAL; }

"true" { return TRUE; }

"false" { return FALSE; }

"nil" { return NIL; }

"=" { return '='; }

"+" { return '+'; }

"-" { return '-'; }

"\*" {return '*'; }

"\/" { return '/'; }

"\%" { return '%';  }

"==" { return EQ;  }

"!=" { return NE;  }

"++" { return PLUSPLUS; }

"--" { return MINUSMINUS;  }

">" { return GT; }

"<" { return LT; }

">=" { return GE;  }

"<=" { return LE;  }

"\{" { return '{'; }

"\}" { return '}';  }

"\[" {  return '['; }

"\]" { return ']';  }

"\(" { return '(';  }

"\)" { return ')';  }

";" { return ';';  }

"," { return ','; }

":" { return ':'; }

"::" { return COLONCOLON;  }

"." { return '.';  }

".." { return DOTDOT; }


{id}	{ yylval.stringValue = new string(yytext); return ID; }

%%

/*USER CODE(OPTIONAL)*/

/*int main(int argc, char** argv) {

	list<marmotes_token_t> token_list;
	marmotes_token_t* yyval = nullptr;

	if ((argc < 2) || (argc > 3)) {
		cerr << "Wrong number of arguments" << endl;
		return 1;
	}
	if (!(yyin = fopen(argv[1], "r"))) {
		cerr << "Error in opening input file" << endl;
		return 1;
	}
	if (argc == 3) {
		if (!(yyout = fopen(argv[2], "w"))) {
			cerr << "Error in opening input file" << endl;
			return 1;
		}
	}

	yyval = new marmotes_token_t();
	unsigned int token_count = 0;
	while (alpha_yylex(yyval) != 0 ) {
		yyval->token_no = ++token_count;
		token_list.push_back(*yyval);
		yyval->empty_values();
	}

	print_list(token_list, yyout);

	//Tidy up memory and file descriptors

	delete yyval; yyval = nullptr;
	fclose(yyin);
	fclose(yyout);

	return 0;
}*/
