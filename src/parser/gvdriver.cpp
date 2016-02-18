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
 * $Id: gvdriver.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file gvdriver.cpp
 * @brief Driver for parsing variable files
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <parser/gvdriver.h>

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;


#include <parser/gvscanner.h>
#include "gvparser.hpp"


namespace mwsim {

  class GVDriver::Private {
  public:
    Private() : scanner(NULL), parser(NULL) {};
    
    GVScanner *scanner;
    GVParser *parser;
  };
  
  
  GVDriver::GVDriver(VariableStore& _vs)
    : vs(_vs)
  {
    d = new mwsim::GVDriver::Private();
  }
  
  
  GVDriver::~GVDriver() {
    if (d->scanner != NULL)
      delete d->scanner;
    if (d->parser != NULL)
      delete d->parser;
    delete d;
  }
  
  
  void GVDriver::parse(const char *filename) {
    assert( filename != NULL );
    std::ifstream in_file(filename);
    if (!in_file.good()) {
      cerr << "Opening input file " << filename << " failed, exiting!" << endl;
      exit(-1);
    }
    
    delete(d->scanner);
    try {
      d->scanner = new GVScanner(&in_file);
    }
    catch (std::bad_alloc &ba) {
      std::cerr << "Failed to allocate scanner: (" <<
	ba.what() << "), exiting!!\n";
      exit(-1);
    }
    
    delete(d->parser); 
    try {
      d->parser = new mwsim::GVParser( (*d->scanner) /* scanner */, 
					(*this) /* driver */ );
    }
    catch (std::bad_alloc &ba) {
      cerr << "Failed to allocate parser: (" << 
	ba.what() << "), exiting!!\n";
      exit(-1);
    }
    const int accept( 0 );
    if(d->parser->parse() != accept) {
      cerr << "Parse failed!!\n";
    }
  }
  

  void GVDriver::addVariable(const std::string& var, int value) {
    vs.putVar(var, value);
  }


  int GVDriver::getVariable(const std::string& var) const {
    return vs.getVar(var);
  }


  const VariableStore& GVDriver::getVariables() const {
    return vs;
  }  

} // NS
