%{
#include "grammar.h"
#include "stdio.h"
//[a-zA-Z0-9]+            yylval.sval=strdup(yytext); return TOKID;
%}

%%
^[ \t]*[#].*            yylval.sval=strdup(yytext);  return COMMENT;
[a-zA-Z0-9.-+:]+         yylval.sval=strdup(yytext); return TOKID;
[a-zA-Z0-9\-./_~+:;#]+  yylval.sval=strdup(yytext); return WORD;
\{		return BEGINSEC;
\}		return ENDSEC;
\=		return EQUAL;
\"		return QUOTE;
\n ;
[ \t] ;
%%


