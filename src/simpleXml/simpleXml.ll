%{
#include "grammar.hh"
#include "stdio.h"

/* Helper for line numbers */
#define YY_USER_INIT yylloc.first_line=1;
extern int line_num;
%}

%%
[#].*$                    yylval.sval=strdup(yytext);  return COMMENT;
for                       yylval.sval=strdup(yytext);  return FOR;
$[a-zA-Z0-9_]+            {
		             yylval.sval=strdup(yytext+1);
		             yylval.sval[yyleng-1] = 0;  
			     return VARDEF;
			  }
$\([a-zA-Z0-9_]+\)        {
		             yylval.sval=strdup(yytext+2);
		             yylval.sval[yyleng-3] = 0;
			     return VAREXP;
			  }
\%[0-9.fds]+		  yylval.sval=strdup(yytext); return FMT;
[a-zA-Z0-9.\-_:]+         yylval.sval=strdup(yytext); return TOKID;
\"[a-zA-Z0-9\-.,/_+:;#°% \t~\<\>\^\&]+\"	 {  
		             yylval.sval=strdup(yytext+1);
		             yylval.sval[yyleng-2] = 0;			 
			     return QSTRING;
			 }
\{		return BEGINSEC;
\}		return ENDSEC;
\=		return EQUAL;
\"		return QUOTE;
\~		return CONCAT;
\n yylloc.first_line++; line_num++;
[ \t] ;
%%

