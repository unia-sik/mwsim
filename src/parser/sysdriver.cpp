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
 * $Id: sysdriver.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file sysdriver.cpp
 * @brief Driver for parsing system description file
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <parser/sysdriver.h>

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;


#include <parser/sysscanner.h>
#include "sysparser.hpp"


namespace mwsim {

  class SysDriver::Private {
  public:
    Private() : scanner(NULL), parser(NULL) {};
    
    SysScanner *scanner;
    SysParser *parser;
  };
  
  
  SysDriver::SysDriver(map<string, int>& _parameters)
    : parameters(_parameters)
  {
    d = new mwsim::SysDriver::Private();
  }
  
  
  SysDriver::~SysDriver() {
    if (d->scanner != NULL)
      delete d->scanner;
    if (d->parser != NULL)
      delete d->parser;
    delete d;
  }
  
  
  void SysDriver::parse(const char *filename) {
    //cout << "Parsing system" << endl;
    assert( filename != NULL );
    std::ifstream in_file(filename);
    if (!in_file.good()) {
      cerr << "Opening input file " << filename << " failed, exiting!" << endl;
      exit(-1);
    }

    //cout << "\tScanner..." << endl;
    if (d->scanner != NULL) delete(d->scanner);
    try {
      d->scanner = new SysScanner(&in_file);
    }
    catch (std::bad_alloc &ba) {
      std::cerr << "Failed to allocate scanner: (" <<
	ba.what() << "), exiting!!\n";
      exit(-1);
    }
    
    //cout << "\tParser..." << endl;
    if (d->parser != NULL) delete(d->parser); 
    try {
      d->parser = new mwsim::SysParser( (*d->scanner) /* scanner */, 
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
  
  
  void SysDriver::setParameter(const string& key, int value) {
    //cout << "Driver: Setting parameter \"" << key << "\" to " << value << endl;
    parameters[key] = value;
  }


  int SysDriver::getParameter(const string& key) {
    return parameters[key];
  }


  const map<string, int>& SysDriver::getSystemParameters() const {
    return parameters;
  }


} // NS
