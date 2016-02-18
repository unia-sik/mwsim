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
 * $Id: mwsdriver.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file mwsdriver.cpp
 * @brief Driver for parsing script file
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */


#include <parser/mwsdriver.h>

#include <cassert>
#include <cctype>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;


#include <parser/mwsscanner.h>
#include "mwsparser.hpp"

namespace mwsim {

  class MWSDriver::Private {
  public:
    Private() : scanner(NULL), parser(NULL) {};
    
    MWSScanner *scanner;
    MWSParser *parser;
  };
  
  
  MWSDriver::MWSDriver(map<string, int>& _parameters, VariableStore& _variables)
    : parameters(_parameters), variables(_variables)
  {
    d = new mwsim::MWSDriver::Private();
  }
  
  
  MWSDriver::~MWSDriver() {
    if (d->scanner != NULL)
      delete d->scanner;
    if (d->parser != NULL)
      delete d->parser;
    delete d;
    //std::list<NodeDescription *> lNodeDescriptions;
    while (lNodeDescriptions.size() > 0) {
    NodeDescription *desc = lNodeDescriptions.front();
    lNodeDescriptions.pop_front();
    delete desc;
    }
  }
  
  
  void MWSDriver::parse(const char *filename) {
    assert( filename != NULL );
    std::ifstream in_file(filename);
    if (!in_file.good()) {
      cerr << "Opening input file " << filename << " failed, exiting!" << endl;
      exit(-1);
    }
    
    delete(d->scanner);
    try {
      d->scanner = new MWSScanner(&in_file);
    }
    catch (std::bad_alloc &ba) {
      std::cerr << "Failed to allocate scanner: (" <<
	ba.what() << "), exiting!!\n";
      exit(-1);
    }
    
    delete(d->parser); 
    try {
      d->parser = new mwsim::MWSParser( (*d->scanner) /* scanner */, 
					(*this) /* driver */ );
    }
    catch (std::bad_alloc &ba) {
      std::cerr << "Failed to allocate parser: (" << 
	ba.what() << "), exiting!!\n";
      exit(-1);
    }
    const int accept( 0 );
    if(d->parser->parse() != accept) {
      std::cerr << "Parse failed!!\n";
    }
  }
  
  
  void MWSDriver::addNodeDescription(NodeDescription *nodeDescription) {
    lNodeDescriptions.push_back(nodeDescription);
    cout << "Added node description" << endl;
  }
  
  /*
  void MWSDriver::setSystemSize(int size) {
  systemSize = size;
  cout << "System size set to " << size << endl;
  }
  

  int MWSDriver::getSystemSize() const {
  return systemSize;
  }
  */
  
  const std::list<NodeDescription *> MWSDriver::getNodeDescriptions() const {
    return lNodeDescriptions;
  }


  void MWSDriver::setParameter(const string& key, int value) {
    parameters[key] = value;
  }


  int MWSDriver::getParameter(const string& key) {
    return parameters[key];
  }


  map<string, int> MWSDriver::getSystemParameters() const {
    return parameters;
  }


  void MWSDriver::addVariable(const std::string &var, int value) {
    cout << "Adding variable " << var << endl;
    variables.putVar(var, value);
  }


  const VariableStore * MWSDriver::getVariables() const {
    return &variables;
  }

} // NS
