/*
 * This file is part of MWSim.
 *
 * Copyright 2014 University of Augsburg
 *
 * MWSim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MWSim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MWSim.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * $Id: mwsscanner.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file mwsscanner.h
 * @brief Script file scanner
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _PARSER_MWSSCANNER_H
#define _PARSER_MWSSCANNER_H 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// Override the interface for yylex since we namespaced it
#undef YY_DECL
#define YY_DECL int mwsim::MWSScanner::yylex()

// Include Bison for types / tokens
#include "mwsparser.hpp"


namespace mwsim {
  
  class MWSScanner : public yyFlexLexer {
  public:
  
  MWSScanner(std::istream *in) : yyFlexLexer(in), yylval(NULL) {};
    
    int yylex(MWSParser::semantic_type *lval, mwsim::location *lloc) {
      yylval = lval;
      yylloc = lloc;
      return yylex();
    }
    
  protected:
    virtual void LexerError(const char *msg);
    
  private:
    /* hide this one from public view */
    int yylex();
    /* yyval ptr */
    MWSParser::semantic_type *yylval;
    location *yylloc;
  };
  
} // NS

#endif /* !_PARSER_MWSSCANNER_H */
