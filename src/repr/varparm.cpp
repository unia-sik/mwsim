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
 * $Id: varparm.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file varparm.cpp
 * @brief Parameter for send/recv commands
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */


#include <repr/varparm.h>

#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

namespace mwsim {

  VarParm::VarParm(const string& _name) : type(VP_NAME), name(_name) { }


  VarParm::VarParm(int _number) : type(VP_NUMBER), number(_number) { }


  VarParm::VarParm(const VarParm& rhs)
    : type(rhs.type), name(rhs.name), number(rhs.number)
  {
  }


  const string& VarParm::getName() const {
    if (type != VP_NAME)
      throw InvalidTypeException(VP_NAME);
    else
      return name;
  }


  int VarParm::getNumber() const {
    if (type != VP_NUMBER)
      throw InvalidTypeException(VP_NUMBER);
    else
      return number;
  }

  int VarParm::resolve(const VariableStore* vs) const {
    int var = -1;
    if (type == VarParm::VP_NAME) {
      var = vs->getVar(name);
    }
    else if (type == VarParm::VP_NUMBER) {
      var = number;
    }
    else {
      cerr << "Invalid VarParm parameter type: " << type << endl;
      abort();
    }
    return var;
  }

} // NS
