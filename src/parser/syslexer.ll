%{
/**
 * $Id: syslexer.ll 1200 2014-06-23 16:14:07Z klugeflo $
 * MOSSCA WCET Simulator
 * \file syslexer.ll
 * \brief Flex lexer for mwsim system description files
 * \author Florian Kluge <kluge@informatik.uni-augsburg.de>
 * see http://www.gnu.org/software/bison/manual/html_node/Calc_002b_002b-Scanner.html
 */

#include <parser/sysscanner.h>
#include <iostream>

typedef mwsim::SysParser::token token;

/* define yyterminate as this instead of NULL */
// not needed anymore, we have a better EOF rule now
//#define yyterminate() return( token::END )

#define DBG_MATCH { \
  /*std::cout << "\tTokenised \"" << yytext << "\" @ " << *yylloc;*/ \
  /*std::cout << "\t\t"__FILE__":" << __LINE__ << "\n";*/ \
}

int sys_yycolumn = 1;
#define YY_USER_ACTION { \
  yylloc->begin.line = yylloc->end.line = yylineno; \
  yylloc->begin.column = sys_yycolumn; \
  yylloc->end.column = sys_yycolumn + yyleng - 1; \
  sys_yycolumn += yyleng; \
}
%}

%option debug
%option yylineno
%option noyywrap
%option c++
%option yyclass="SysScanner"
%option prefix="sys"



%%
 /* each yylex() */
%{
  yylloc->step();
  //std::cout << "LL:Current location: (" << yylloc->begin << "," << yylloc->end << ")" << std::endl;
%}

 /* remove comments */
#.*               { DBG_MATCH; sys_yycolumn = 1; yylloc->lines(yyleng); }

 /* commands */
system	            { DBG_MATCH; return token::SYSTEM; }
end		    { DBG_MATCH; return token::SYMB_END; }

 /* gobble up newlines */
[\n]+		    { 
		      DBG_MATCH;
		      sys_yycolumn = 1;
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

\=		    { DBG_MATCH; return token::ASSIGN; }
\-		    { DBG_MATCH; return token::MINUS; }

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

