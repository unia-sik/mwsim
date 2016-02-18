%{
/**
 * $Id: mwslexer.ll 1166 2014-05-28 13:29:13Z klugeflo $
 * MOSSCA WCET Simulator
 * \file mwslexer.ll
 * \brief Flex lexer for mwsim script files
 * \author Florian Kluge <kluge@informatik.uni-augsburg.de>
 * see http://www.gnu.org/software/bison/manual/html_node/Calc_002b_002b-Scanner.html
 */

#include <parser/mwsscanner.h>
#include <iostream>

typedef mwsim::MWSParser::token token;

/* define yyterminate as this instead of NULL */
// not needed anymore, we have a better EOF rule now
//#define yyterminate() return( token::END )

#define DBG_MATCH { \
  /*std::cout << "\tTokenised \"" << yytext << "\" @ " << *yylloc;*/ \
  /*std::cout << "\t\t"__FILE__":" << __LINE__ << "\n";*/ \
}

int yycolumn = 1;
#define YY_USER_ACTION { \
  yylloc->begin.line = yylloc->end.line = yylineno; \
  yylloc->begin.column = yycolumn; \
  yylloc->end.column = yycolumn + yyleng - 1; \
  yycolumn += yyleng; \
}
/*
yylloc.first_line = yyloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
    yycolumn += yyleng
*/
%}

%option debug
%option yylineno
%option noyywrap
%option c++
%option yyclass="MWSScanner"



%%
 /* each yylex() */
%{
  yylloc->step();
  //std::cout << "LL:Current location: (" << yylloc->begin << "," << yylloc->end << ")" << std::endl;
%}

 /* remove comments */
#.*               { DBG_MATCH; yycolumn = 1; yylloc->lines(yyleng); }

 /* commands */
system	            { DBG_MATCH; return token::SYSTEM; }
 /*size		    { DBG_MATCH; return token::SIZE; }*/
node		    { DBG_MATCH; return token::NODE; }
exec		    { DBG_MATCH; return token::EXEC; }
load		    { DBG_MATCH; return token::LOAD; }
send		    { DBG_MATCH; return token::SEND; }
recv		    { DBG_MATCH; return token::RECV; }
wait		    { DBG_MATCH; return token::WAIT; }
foreach		    { DBG_MATCH; return token::FOREACH; }
in		    { DBG_MATCH; return token::IN; }
end		    { DBG_MATCH; return token::SYMB_END; }
parwait		    { DBG_MATCH; return token::PARWAIT; }
from		    { DBG_MATCH; return token::FROM; }
as		    { DBG_MATCH; return token::AS; }
calc		    { DBG_MATCH; return token::CALC; }
variables	    { DBG_MATCH; return token::VARIABLES; }
 /*endforeach	    { DBG_MATCH; return token::ENDFOREACH; }*/

 /* gobble up newlines */
[\n]+		    { 
		      DBG_MATCH;
		      yycolumn = 1;
		      //std::cout << "newlines\n";
		      yylloc->lines(yyleng);
		      yylloc->step();
		    }

[0-9]+		    { DBG_MATCH;
		      yylval->number = atoi(yytext);
		      return token::NUMBER;
		    }
[a-zA-Z][a-zA-Z0-9_]* { DBG_MATCH;
		      yylval->string = strdup(yytext);
		      return token::WORD;
		    }

\[		    { DBG_MATCH; return token::OBRACKET; }
\]		    { DBG_MATCH; return token::EBRACKET; }
\,		    { DBG_MATCH; return token::KOMMA; }
\-		    { DBG_MATCH; return token::MINUS; }
\=		    { DBG_MATCH; return token::ASSIGN; }
\+		    { DBG_MATCH; return token::PLUS; }
\*		    { DBG_MATCH; return token::MUL; }

 /* Ensure that parser sees a newline on EOF */
<<EOF>>             { 
		      DBG_MATCH;
		      //std::cout << "\t\t[EOF]\n";
		      static int once = 0;
		      //return once++ ? token::END : token::NEWLINE;
		      return token::END;
		    }

 /* Default: ignore */
.		    {
		      DBG_MATCH;
		      //std::cout << "\t\t[default]\n";
		    }

%%

