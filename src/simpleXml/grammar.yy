%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>

#include "tools/Exception.h"

using namespace std;
int yylex();
int yyparse();

extern void rewind(int n);

FILE *_yyout = stdout;

/* Helper for line numbers */
/*typedef struct YYLTYPE {
   int first_line;
} YYLTYPE;*/

//extern YYLTYPE yylloc;

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


int indent=0;
#include "SyntacticTreeModel.h"


block *root = NULL;
block *level = NULL;
varstring *vstr = NULL;


char* EscapeString(const char* s)
{
	char* es = (char*) malloc(strlen(s)*6+1);
	char* scpy = es;
 	for(size_t i=0; i < strlen(s); i++){
 		switch(s[i]) {
 		 	case '<':
 		 	 		 *scpy = '&';scpy++;
 		 	 		 *scpy = 'l';scpy++;
 		 	 		 *scpy = 't';scpy++;
 		 	 		 *scpy = ';';scpy++;
 		 	 		 break;
 		 	case '>':
 		 	 		 *scpy = '&';scpy++;
 		 	 		 *scpy = 'g';scpy++;
 		 	 		 *scpy = 't';scpy++;
 		 	 		 *scpy = ';';scpy++;
 		 	 		 break;
 		 	case '&':
 		 	 		 *scpy = '&';scpy++;
 		 	 		 *scpy = 'a';scpy++;
 		 	 		 *scpy = 'm';scpy++;
 		 	 		 *scpy = 'p';scpy++;
 		 	 		 *scpy = ';';scpy++;
 		 	 		 break;
 		 	default:
    				*scpy = s[i];	
 					scpy++;
 		}
 	}
 	*scpy = 0;
 	return es;
}

void AppendStr(const char* s)
{
	if (vstr == NULL) vstr = new varstring(s);
	else vstr->AppendString(s);
}

void AppendVar(const char* s, const char* fmt = "%s")
{
	if (vstr == NULL) vstr = new varstring();
	vstr->AppendVar(s,fmt);
}

void CheckRoot()
{
      if (root == NULL) {
         root = new block();
         level = root;
      }
}

%}


%union {
	int ival;
	char* sval;
}

%token <sval> TOKID QSTRING BEGINSEC ENDSEC EQUAL QUOTE COMMENT VARDEF VAREXP FOR CONCAT FMT
%type <sval> strexp 
%left CONCAT

%%
lines: 
	| lines line
;
line:
   COMMENT { }
   |
   VARDEF EQUAL strexp
   {
      varlist[string($1)] = $3;
      delete vstr;
      vstr = NULL;
   }
   |
   FOR VARDEF EQUAL TOKID TOKID BEGINSEC
   {
      indent++;
      CheckRoot();
      double b,e;
      b = Str2Double($4);
      e = Str2Double($5);
      forloop *node = new forloop(b, e);
      node->LoopVar($2);
      level->AddChild(node);
      level = node;

   }
   |
   FOR VARDEF EQUAL TOKID TOKID TOKID BEGINSEC
   {
      indent++;
      CheckRoot();
      double b,e,s;
      b = Str2Double($4);
      e = Str2Double($5);
      s = Str2Double($6);
      forloop *node = new forloop(b, e, s);
      node->LoopVar($2);
      level->AddChild(node);
      level = node;
   }   
   |
   TOKID BEGINSEC 
	{
		indent++;
      CheckRoot();
		
		xmlnode *node = new xmlnode($1);
		level->AddChild(node);
		level = node; 
		
	}
	|
   TOKID EQUAL strexp
   {
      xmlnode *node = dynamic_cast<xmlnode*>(level);
      if (node == NULL){
         stringstream ss;
         string err;
         ss << "xmlsimple parser: Attribute definition not allowed here (line " << @1.first_line << ")";
         err = ss.str();
         yyerror(err.c_str());
      }
      varstring attr($1);
      node->AddAttribute(attr, *vstr);
      delete vstr;
      vstr = NULL;
   }
   |
	ENDSEC
	{        
             indent--;
             level = level->Parent();
             if (level == NULL) 
             	throw (QDLIB::EParamProblem("Too many closing braces!"));
             if (indent == 0){
                root->execute();
                delete root;
             }
	}
   | error
   {
      stringstream ss;
      string err;
      ss << "xmlsimple parser error at line " << @1.first_line <<endl;
      err = ss.str();
      throw (QDLIB::EParamProblem(err.c_str()));
   }
;
strexp:
	TOKID { AppendStr($1); }
	| QSTRING { char* es=EscapeString($1); AppendStr(es);  free(es);}
	| VAREXP { AppendVar(strdup(string($1).c_str())); }	
	| VAREXP FMT { AppendVar(strdup(string($1).c_str()),$2); }
	| strexp CONCAT strexp { }
;	
	
%%



