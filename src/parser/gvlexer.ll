%{
/**
 * $Id: gvlexer.ll 1642 2016-02-18 06:46:15Z klugeflo $
 * MOSSCA WCET Simulator
 * \file gvlexer.ll
 * \brief Flex lexer for mwsim global variables files
 * \author Florian Kluge <kluge@informatik.uni-augsburg.de>
 * see http://www.gnu.org/software/bison/manual/html_node/Calc_002b_002b-Scanner.html
 */

#include <parser/gvscanner.h>
#include <iostream>

typedef mwsim::GVParser::token token;

/* define yyterminate as this instead of NULL */
// not needed anymore, we have a better EOF rule now
//#define yyterminate() return( token::END )

#define DBG_MATCH { \
  /*std::cout << "\tTokenised \"" << yytext << "\" @ " << *yylloc;*/ \
  /*std::cout << "\t\t"__FILE__":" << __LINE__ << "\n";*/ \
}

int gv_yycolumn = 1;
#define YY_USER_ACTION { \
  yylloc->begin.line = yylloc->end.line = yylineno; \
  yylloc->begin.column = gv_yycolumn; \
  yylloc->end.column = gv_yycolumn + yyleng - 1; \
  gv_yycolumn += yyleng; \
}
%}

%option debug
%option yylineno
%option noyywrap
%option c++
%option yyclass="GVScanner"
%option prefix="gv"



%%
 /* each yylex() */
%{
  yylloc->step();
  //std::cout << "LL:Current location: (" << yylloc->begin << "," << yylloc->end << ")" << std::endl;
%}

 /* remove comments */
#.*               { DBG_MATCH; gv_yycolumn = 1; yylloc->lines(yyleng); }

 /* commands */
variables	    { DBG_MATCH; return token::VARIABLES; }
end		    { DBG_MATCH; return token::SYMB_END; }

 /* gobble up newlines */
[\n]+		    { 
		      DBG_MATCH;
		      gv_yycolumn = 1;
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

 /* Ensure that parser sees a newline on EOF */
<<EOF>>             { 
		      DBG_MATCH;
		      //std::cout << "\t\t[EOF]\n";
		      //static int once = 0;
		      //return once++ ? token::END : token::NEWLINE;
		      return token::END;
		    }

 /* Default: ignore */
.		    {
		      DBG_MATCH;
		      //std::cout << "\t\t[default]\n";
		    }

%%

