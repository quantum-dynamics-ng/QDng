%{
#include "y.tab.h"
#include "stdio.h"
%}

%%
[a-zA-Z0-9]+	yylval.sval=strdup(yytext); return WORD;
\{		return BEGINSEC;
\}		return ENDSEC;
\=		return EQUAL;
\"		return QUOTE;
\n ;
[ \t] ;
%%


