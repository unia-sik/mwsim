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
 * $Id: variablestore.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file variablestore.cpp
 * @brief Storage for program variable
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <tools/variablestore.h>

#include <iostream>

using namespace std;

namespace mwsim {

  VariableStore::VariableStore(const VariableStore *_parent)
    : parent(_parent)
  {
    //if (_parent == NULL)
    //  cerr << "Initialising VS without parent!" << endl;
  }

  void VariableStore::putVar(string var, int value) {
    data[var] = value;
  }

  int VariableStore::getVar(string var) const {
    if (data.find(var) != data.end()) {
      //return data[var];
      return data.at(var);
    }
    else {
      //cout << "VS: search for variable " << var << " in parent..." << endl;
      if (parent != NULL) {
	return parent->getVar(var);
      }
      else {
	// throw exception!
	cerr << "Could not find variable \"" << var << "\"" << endl;
	throw VariableNotFoundException(var);
      }
    }
  }

  
  void VariableStore::print() const {
    for (map<string, int>::const_iterator it = data.begin();
	 it != data.end(); ++it) {
      cout << it->first << "=" << it->second << endl;
    }
    if (parent != NULL) {
      cout << "Imported from parent:" << endl;
      parent->print();
    }
  }


} // NS
