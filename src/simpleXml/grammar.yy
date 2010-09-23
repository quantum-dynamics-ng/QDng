%{
#include <stdio.h>
#include <string.h>
#include <stack>
#include <string>

#include "tools/Exception.h"

using namespace std;
int yylex();
int yyparse();

FILE *_yyout = stdout;

void yyerror(const char *str)
{
   throw (QDLIB::EParamProblem(str));
}

extern "C" {
   int yywrap()
   {
   return 1;
   } 
}


stack<string> elnames;
bool openel=false;
int indent=0;

%}


%union {
	int ival;
	char* sval;
}

%token <sval> TOKID WORD BEGINSEC ENDSEC EQUAL QUOTE


%%
lines: 
	| lines line

line:
       TOKID BEGINSEC 
	{
		if (openel) fprintf(_yyout, ">\n");
		for (int i=0; i < indent; i++) fprintf(_yyout, "   ");
	 	fprintf(_yyout, "<%s", $1);
		elnames.push(string($1));
		openel=true;
		indent++;
	}
	|
        TOKID EQUAL TOKID
        {
	     fprintf(_yyout, " %s=\"%s\"", $1, $3);
	}
	|
        TOKID EQUAL QUOTE TOKID QUOTE
        {
	     fprintf(_yyout, " %s=\"%s\"", $1, $4);
	}
        |
        TOKID EQUAL QUOTE WORD QUOTE
        {
             fprintf(_yyout, " %s=\"%s\"", $1, $4);
        }
	|
	ENDSEC
	{

		indent--;
		if (openel) {
		   fprintf(_yyout, "/>\n");
         	} else {
		   for (int i=0; i < indent; i++) fprintf(_yyout, "   ");
		   fprintf(_yyout, "</%s>\n", elnames.top().c_str() );
		 }

		openel=false;
		elnames.pop();
	}
%%



