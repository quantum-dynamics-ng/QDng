%{
#include <stdio.h>
#include <string.h>
#include <stack>
#include <vector>
#include <string>
#include <typeinfo>

#include "tools/Exception.h"

using namespace std;
int yylex();
int yyparse();

extern void rewind(int n);

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


int indent=0;
#include "SyntacticTreeModel.h"


block *root = NULL;
block *level = NULL;
varstring *vstr = NULL;

void AppendStr(char* s)
{
	if (vstr == NULL) vstr = new varstring(s);
	else vstr->AppendString(s);
}

void AppendVar(char* s, char* fmt = "%s")
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
		cout << "Open Section "<<$1<<"\n";
		
	}
	|
   TOKID EQUAL strexp
   {
      xmlnode *node = dynamic_cast<xmlnode*>(level);
      if (node == NULL){
         char mesg[]="Attribute definition not allowed here";
         yyerror(mesg);
      }
      varstring attr($1);
      node->AddAttribute(attr, *vstr);
      cout << "Add Attribute "<< $1 << " = " << vstr->GetString() << "\n";
      delete vstr;
      vstr = NULL;
   }
	|
	ENDSEC
	{        
             indent--;
             level = level->Parent();
             if (indent == 0){
                cout << "exec\n"; 
                root->execute();
                delete root;
             }
	}
;
strexp:
	TOKID { AppendStr($1); cout << "Append TOKID "<< $1 << "\n"; }
	| QSTRING { AppendStr($1); cout << "Append QWORD " << $1 << "\n";}
	| VAREXP { AppendVar(strdup(string($1).c_str())); cout << "Append VAREXP " << $1 << "\n"; }	
	| VAREXP FMT { AppendVar(strdup(string($1).c_str()),$2); cout << "Append VAREXP " << $1 <<" " <<$2<< "\n"; }
	| strexp CONCAT strexp { cout << " Concat " << $1 <<" "<< $3 << "\n"; }
;	
	
%%



