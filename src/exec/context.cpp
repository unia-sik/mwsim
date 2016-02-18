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
 * $Id: context.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file context.cpp
 * @brief Execution Context
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include "context.h"

//#include <iostream>
using namespace std;

namespace mwsim {

  //const std::string Context::VAR_NODE_ID = "MY_NODE_ID";
  const char* Context::VAR_NODE_ID = "MY_NODE_ID";


  Context::Context(const Program &_program, const VariableStore *vsParent)
    : program(_program)
  {
    //cout << "creating context..." << endl;
    variables = new VariableStore(vsParent);
    pc = program.begin();
  }


  Context::Context(const Program &_program, NodeId nodeId, const VariableStore *vsParent)
    : program(_program)
  {
    //cout << "creating context for node " << nodeId << endl;
    variables = new VariableStore(vsParent);
    variables->putVar(VAR_NODE_ID, nodeId);
    //cout << "\t" << VAR_NODE_ID << " = " << variables->getVar(VAR_NODE_ID) << endl;
    pc = program.begin();
  }


  Context::~Context() {
    delete variables;
  }


  Command *Context::getCurrentCommand() {
    if (pc != program.end())
      return *pc;
    else
      return NULL;
  }


  bool Context::advancePC() {
    if (pc == program.end())
      return false;
    ++pc;
    if (pc == program.end())
      return false;
    else
      return true;
  }


  VariableStore *Context::getVariables() {
    return variables;
  }

} // NS
