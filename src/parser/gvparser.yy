/**
 * $Id: gvparser.yy 1642 2016-02-18 06:46:15Z klugeflo $
 * MOSSCA WCET Simulator
 * @file gvparser.yy
 * @brief Parser for global variables files
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

%skeleton "lalr1.cc"
%require  "2.4.1"
%debug 
%defines

%define api.namespace {mwsim}
%define parser_class_name {GVParser}

%code requires{
  namespace mwsim {
    class GVDriver;
    class GVScanner;
  }
}

%lex-param   { GVScanner  &scanner }
%lex-param   { GVDriver  &driver   }

%parse-param { GVScanner  &scanner }
%parse-param { GVDriver  &driver   }

%locations

%code{

#include <cstdio>

#include <iostream>
#include <cstdlib>
#include <fstream>

/* include for all driver functions */
#include <parser/gvdriver.h>
#include <parser/gvscanner.h>

/* this is silly, but I can't figure out a way around */
static int yylex(mwsim::GVParser::semantic_type *yylval,
       	   	 mwsim::location *yylloc,
                 mwsim::GVScanner  &scanner,
                 mwsim::GVDriver   &driver);

using namespace mwsim;
using namespace std;
}


%union {
  int number;
  char *string;
}


%token END    0     "end of file"
%token VARIABLES

%token <number> NUMBER
%token <string> WORD
%token ASSIGN
%token SYMB_END

%%

variables: VARIABLES var_assignments SYMB_END VARIABLES
	   {
	     //std::cout << "found variables\n";
           }
	   ;

var_assignments:
	{
	}
	| var_assignments var_assignment
	{
	}
	;

var_assignment: WORD ASSIGN NUMBER
		{
		  driver.addVariable($1, $3);
		  free($1);
		}
		; 

%%

void mwsim::GVParser::error(const location_type& loc, const std::string &err_message) {
   std::cerr << "Parser error: " << err_message << " @ (" << loc.begin << ","
             << loc.end << ")" << "\n"; 


}
 
 
/* include for access to scanner.yylex */

static int yylex(mwsim::GVParser::semantic_type *yylval,
       	   	 mwsim::location *yylloc,
       mwsim::GVScanner  &scanner,
       __attribute__((unused)) mwsim::GVDriver   &driver) {
   return(scanner.yylex(yylval, yylloc));
}
