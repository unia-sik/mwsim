/**
 * $Id: mwsparser.yy 1642 2016-02-18 06:46:15Z klugeflo $
 * MOSSCA WCET Simulator
 * @file mwsparser.yy
 * @brief Bison parser for mwsim script files
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 * @see http://www.gnu.org/software/bison/manual/html_node/Calc_002b_002b-Parser.html
 */

%skeleton "lalr1.cc"
%require  "2.4.1"
%debug 
%defines

%define api.namespace {mwsim}
%define parser_class_name {MWSParser}

%code requires{
  namespace mwsim {
    class MWSDriver;
    class MWSScanner;
    class Command;
    class Program;
    class VarParm;
    class MExpression;
  }
  #include <repr/mexpression.h>
  #include <list>
}

%lex-param   { MWSScanner  &scanner }
%lex-param   { MWSDriver  &driver   }

%parse-param { MWSScanner  &scanner }
%parse-param { MWSDriver  &driver   }

%locations

%code{

#include <cstdio>

#include <iostream>
#include <cstdlib>
#include <fstream>

/* include for all driver functions */
#include <parser/mwsdriver.h>
#include <parser/mwsscanner.h>

/* this is silly, but I can't figure out a way around */
static int yylex(mwsim::MWSParser::semantic_type *yylval,
       	   	 mwsim::location *yylloc,
                 mwsim::MWSScanner  &scanner,
                 mwsim::MWSDriver   &driver);


#include <repr/command.h>
#include <repr/nodedescription.h>
#include <repr/mexpression.h>
using namespace mwsim;

#include <list>
using namespace std;
}


%union {
  int number;
  char *string;
  mwsim::Command *cmd;
  Program *cmd_list;
  std::list<int> *list_list;
  VarParm *vp;
  std::string *ov;
  mwsim::MExpression *mexp;
  mwsim::Operator op;
}


%token END    0     "end of file"
%token SYSTEM
%token VARIABLES
%token SIZE
%token NODE
%token EXEC
%token LOAD
%token SEND
%token RECV
%token WAIT
%token FOREACH
%token IN
%token PARWAIT
%token FROM
%token AS
%token SYMB_END
%token ENDFOREACH
%token KOMMA
%token MINUS
%token OBRACKET
%token EBRACKET
%token ASSIGN
%token PLUS
%token MUL
%token CALC

%token <number> NUMBER
%token <string> WORD


%type <cmd> cmd_exec cmd_load cmd_send cmd_recv cmd_wait cmd_calc cmd_foreach cmd_parwait command commandline
%type <cmd_list> commandlines
%type <list_list> list listpfx listelem range
/*%type <ov> optvar*/
%type <vp> varparm
%type <mexp> expression
%type <op> operator

%type <number> integer

%%

/*file: commandlines END;*/
/*file: nodes END;*/


file: system variables nodes END;

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


/*
sys_nodes: SIZE NUMBER
       {
         //std::cout << "System has " << $2 << " nodes\n";
	 driver.setSystemSize($2);
       }
       ;
*/

nodes: 
       {
         //std::cout << "Empty nodes\n";
       }
       | nodes node
       {
         //std::cout << "Node found\n";
       }
       ;

node: NODE list commandlines SYMB_END NODE
      {
        //std::cout << "YY:Construct node!\n";
	driver.addNodeDescription(new NodeDescription($2, $3));
      }
      ;




commandlines:
	{
	  $$ = NULL;
	  //std::cout << "Empty commandline\n";
	}
	| commandlines commandline
	{
	  //std::cout << "Matched commandlines\n";
	  if ($1 == NULL) {
	    //std::cout << "no command list yet\n";
	    $$ = new Program;
 	  }
	  if ($2 != NULL) {
	    //std::cout << "Adding command \"" << *$2 << "\" to list\n";
	    $$->push_back($2);
	  }
	}
	;

commandline: command
	     {
	       //std::cout << "Matched commandline\n\n";
	       $$ = $1;
	     }
	     /*|
	     { $$ = NULL; }*/
	     ;

command: cmd_exec | cmd_load | cmd_send | cmd_recv | cmd_wait | cmd_calc | cmd_foreach | cmd_parwait;

cmd_exec: EXEC varparm
	  {
	    //std::cout << "Matched exec command with ET " << $2 << "\n\n";
	    $$ = new mwsim::ExecCommand($2);
	    //std::cout << *$$ << "\n\n";
	  }
	  ;

cmd_load: LOAD varparm
	  {
	    //std::cout << "Matched load command with size " << $2 << "\n\n";
	    $$ = new mwsim::LoadCommand($2);
	    //std::cout << *$$ << "\n\n";
	  }
	  ;

cmd_send: SEND varparm WORD varparm
	  {
	    //std::cout << "Matched send command with id " << $2 << "\n\n";
	    $$ = new mwsim::SendCommand($2, $3, $4);
	    free($3);
	    //std::cout << *$$ << "\n\n";
	  }
	  ;

cmd_recv: RECV varparm WORD
	  {
	    //std::cout << "Matched recv command with id " << $2 << "\n\n";
	    $$ = new mwsim::RecvCommand($2, $3);
	    free($3);
	    //std::cout << *$$ << "\n\n";
	  }
	  ;

/*cmd_wait: WAIT WORD optvar*/
cmd_wait: WAIT WORD FROM varparm
	  {
	    //std::cout << "Matched wait command\n\n";
	    $$ = new mwsim::WaitCommand($2, $4);
	    /*
	    if ($3 == NULL) {
	      $$ = new mwsim::WaitCommand($2);
	    }
	    else {
	      $$ = new mwsim::WaitCommand($2, *$3);
	      delete $3;
	    }
	    free($2);
	    */
	    //std::cout << *$$ << "\n\n";
	  }
	  ;

cmd_calc: CALC WORD ASSIGN expression
	  {
	    $$ = new mwsim::CalcCommand($2, $4);
	    free($2);
	  }

cmd_foreach: FOREACH WORD IN list commandlines SYMB_END FOREACH
	  {
	    //std::cout << "Matched foreach\n";	
	    $$ = new mwsim::ForeachCommand($2, $4, $5);
	    free($2);
	    //delete $4;
	    //delete $5;
	  }
	  ;

cmd_parwait: PARWAIT WORD FROM list AS WORD commandlines SYMB_END PARWAIT
	  {
	    $$ = new mwsim::ParwaitCommand($2, $4, $6, $7);
	    free($2);
	    free($6);
	  }
	  ;

expression: varparm operator varparm
	  {
	    $$ = new mwsim::MExpression($1, $3, $2);
	  }

operator: PLUS
	  {
	    $$ = mwsim::OP_PLUS;
	  }
	  |
	  MINUS
	  {
	    $$ = mwsim::OP_MINUS;
	  }
	  |
	  MUL
	  {
	    $$ = mwsim::OP_MUL;
	  }
	  ;

/*
optvar:
	{
	  $$ = NULL;
	}
	|
	FROM WORD
	{
	  $$ = new std::string($2);
	  free($2);
	}
	;
*/

varparm: NUMBER
	  {
	    //std::cout << "\tNumeric varparm " << $1 << std::endl;
	    $$ = new mwsim::VarParm($1);
	  }
	  |
	  WORD
	  {
	    //std::cout << "\tString varparm \"" << $1 << "\n";
	    $$ = new mwsim::VarParm($1);
	    free($1);
	  }
	  ;

/*
send_target: NUMBER
	  {
	    //std::cout << "\tSend target " << $1 << std::endl;
	  }
	  |
	  WORD
	  {
	    //std::cout << "\tSend target in var \"" << $1 << "\n";
	  }
	  ;
*/
/*
recv_target: 
	     |
	     NUMBER
	     {
	       //std::cout << "\tRecv target " << $1 << "\n";
	     }
*/

list: OBRACKET listpfx listelem EBRACKET
      {
        if ($2 == NULL) {
          //std::cout << "no list list yet\n";
          $$ = new std::list<int>;
        }
        else {
          $$ = $2;
	}
        $$->splice($$->end(), *$3);
	delete $3;
      }
      ;


listpfx: { $$ = NULL; }
	 | listpfx listelem KOMMA
	 {
	   //std::cout << "Matched commandlines\n";
	   if ($1 == NULL) {
	     //std::cout << "no list list yet\n";
	     $$ = new std::list<int>;
 	   }
	   if ($2 != NULL) {
	     //std::cout << "Adding command \"" << *$2 << "\" to list\n";
	     $$->splice($$->end(), *$2);
	     delete $2;
	   }
	 }
	 ;

listelem:
	NUMBER
	{
	//std::cout << "Found number " << $1 << std::endl;
	std::list<int> *lst = new std::list<int>;
	lst->push_back($1);
	$$ = lst;
	}
	| range
	;

range:
	NUMBER MINUS NUMBER
	{
	  if ($1 > $3) {
	    //error(yylloc, "incorrect list bounds");
	    error(@3, "incorrect list bounds");
   	  }
	  //std::cout << "Recognized range " << $1 << " to " << $3 << std::endl;
	  std::list<int> *lst = new std::list<int>;
	  int i;
	  for (i = $1; i <= $3; ++i) lst->push_back(i);
	  $$ = lst;
	}
	;


%%

void mwsim::MWSParser::error(const location_type& loc, const std::string &err_message) {
   std::cerr << "Parser error: " << err_message << " @ (" << loc.begin << ","
             << loc.end << ")" << "\n"; 


}
 
 
/* include for access to scanner.yylex */

static int yylex(mwsim::MWSParser::semantic_type *yylval,
       	   	 mwsim::location *yylloc,
       mwsim::MWSScanner  &scanner,
       __attribute__((unused)) mwsim::MWSDriver   &driver) {
   return(scanner.yylex(yylval, yylloc));
}
