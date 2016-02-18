/**
 * $Id: sysparser.yy 1642 2016-02-18 06:46:15Z klugeflo $
 * MOSSCA WCET Simulator
 * @file sysparser.yy
 * @brief Parser for system description files
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

%skeleton "lalr1.cc"
%require  "2.4.1"
%debug 
%defines

%define api.namespace {mwsim}
%define parser_class_name {SysParser}

%code requires{
  namespace mwsim {
    class SysDriver;
    class SysScanner;
  }
}

%lex-param   { SysScanner  &scanner }
%lex-param   { SysDriver  &driver   }

%parse-param { SysScanner  &scanner }
%parse-param { SysDriver  &driver   }

%locations

%code{

#include <cstdio>

#include <iostream>
#include <cstdlib>
#include <fstream>

/* include for all driver functions */
#include <parser/sysdriver.h>
#include <parser/sysscanner.h>

/* this is silly, but I can't figure out a way around */

static int yylex(mwsim::SysParser::semantic_type *yylval,
       	   	 mwsim::location *yylloc,
                 mwsim::SysScanner  &scanner,
                 mwsim::SysDriver   &driver);

using namespace mwsim;
using namespace std;
}


%union {
  int number;
  char *string;
}


%token END    0     "end of file"
%token SYSTEM

%token <number> NUMBER
%token <string> WORD
%token ASSIGN
%token SYMB_END
%token MINUS

%type <number> integer

%%

system: SYSTEM systemdescription SYMB_END SYSTEM
	{
	  //std::cout << "found system description\n";
	}
	;

systemdescription:
	{
	  //std::cout << "NOTHING in systemdescription\n";
	}
	| systemdescription systemproperty
	{
	  //std::cout << "Recognised system property\n";
	}
	;

systemproperty: WORD ASSIGN integer
		{
		  //std::cout << "Setting sysprop \"" << $1 << "\" to " << $3 << "\n";
		  driver.setParameter($1, $3);
		  free($1);
		}

		;

integer: NUMBER
	 {
	   $$ = $1;
	 }
	 | MINUS NUMBER
	 {
	   $$ = -$2;
	 }

%%

void mwsim::SysParser::error(const location_type& loc, const std::string &err_message) {
   std::cerr << "Parser error: " << err_message << " @ (" << loc.begin << ","
             << loc.end << ")" << "\n"; 


}
 
 
/* include for access to scanner.yylex */

static int yylex(mwsim::SysParser::semantic_type *yylval,
       	   	 mwsim::location *yylloc,
       mwsim::SysScanner  &scanner,
       __attribute__((unused)) mwsim::SysDriver   &driver) {
   return(scanner.yylex(yylval, yylloc));
}
