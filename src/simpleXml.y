%{
#include <stdio.h>
#include <string.h>
#include <stack>
#include <string>
#include <iostream>

	int yylex();
	int yyparse();

      void yyerror(const char *str)
      {
	  fprintf(stderr,"error: %s\n",str);
      }
		
      int yywrap()
      {
	return 1;
      } 

using namespace std;

stack<string> elnames;
bool openel=false;
int indent=0;

main()
{
   yyparse();
} 

%}


%union {
	int ival;
	char* sval;
}

%token <sval> WORD BEGINSEC ENDSEC EQUAL QUOTE


%%
lines: 
	| lines line

line:
       WORD BEGINSEC 
	{
		if (openel) cout << ">\n";
		for (int i=0; i < indent; i++) cout << "   ";
	 	cout <<"<" << $1;
		elnames.push(string($1));
		openel=true;
		indent++;
	}
	|
        WORD EQUAL WORD
        {
	     cout << " " <<$1 << "=\"" << $3 << "\"";  	
	}
	|
        WORD EQUAL QUOTE WORD QUOTE
        {
	     cout << " " <<$1 << "=\"" << $4 << "\"";  	
	}
	|
	ENDSEC
	{

		indent--;
		if (openel) {
		   cout << "/>\n";
         	} else {
		   for (int i=0; i < indent; i++) cout << "   ";
		   cout << "</" << elnames.top() <<  ">\n";
		 }

		openel=false;
		elnames.pop();
	}



%%
/*
node:
    attribute
    |
    element
    ;

attribute:
	 WORD EQUAL WORD
	 {
	 	printf("%s = %s\n", $1, $3);  	
	 }

*/


