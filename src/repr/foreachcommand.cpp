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
 * $Id: foreachcommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file foreachcommand.cpp
 * @brief Command representing a foreach loop
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>
#include <repr/program.h>

#include <list>

using namespace std;

namespace mwsim {

  ForeachCommand::ForeachCommand(std::string _var, std::list<int>* _values, Program* _commands)
    : SubProgramCommand(CT_FOREACH, _commands), var(_var), values(_values),
      running(false)
  {
  }


  ForeachCommand::~ForeachCommand() {
    /*
    if (commands != NULL) {
      while (commands->size() > 0) {
	Command * cmd = commands->front();
	commands->pop_front();
	delete cmd;
      }
      delete commands;
    }
    */
    if (values != NULL)
      delete values;
  }


  ForeachCommand::ForeachCommand(const ForeachCommand& rhs)
    : SubProgramCommand(rhs), var(rhs.var), running(false)
  {
    /*
    cout << "Copying ForeachCommand with program @ " << rhs.commands << endl;
    if ( (rhs.commands == NULL) || (rhs.commands->empty()) ) {
      commands = NULL;
    }
    else {
      commands = new Program;
      *commands = *rhs.commands;
    }
    */
    if ( (rhs.values == NULL) || (rhs.values->empty()) ) {
      values = NULL;
    }
    else {
      values =new list<int>;
      /*
      for (list<int>::iterator iti = rhs.values->begin();
	   iti != rhs.values->end(); ++iti) {
	values->push_back(*iti);
      }
      */
      *values = *rhs.values;
    }
  }


  Command* ForeachCommand::clone() const {
    return new ForeachCommand(*this);
  }
  

  const std::string& ForeachCommand::getVar() const {
    return var;
  }

  const std::list<int>* ForeachCommand::getValues() const {
    return values;
  }
  /*
  const Program* ForeachCommand::getCommands() const {
    return commands;
  }
  */
  void ForeachCommand::printLong(std::ostream &os) const {
    //os << (Command) cmd << endl;
    print(os);
    os << endl;
    os << "\t" << var << " : [";
    list<int>::const_iterator it = values->begin();
    if (it != values->end()) {
      os << *it;
    }
    for ( ; it != values->end(); ++it) {
      os << "," << *it;
    }
    os << "]" << endl;
    for (Program::const_iterator itc = getSubProgram()->begin();
	 itc != getSubProgram()->end(); ++itc) {
      os << "\t" << **itc << endl;
    }
  }


  void ForeachCommand::init() {
    itExec = values->begin();
    running = true;
  }


  int ForeachCommand::current() {
    return *itExec;
  }


  int ForeachCommand::next() {
    int rv = *itExec;
    ++itExec;
    return rv;
  }


  bool ForeachCommand::isFinished() const {
    return (itExec == values->end());
  }


  bool ForeachCommand::isRunning() const {
    return running;
  }


  std::ostream & operator<<(std::ostream &os, const ForeachCommand& cmd) {
    cmd.printLong(os);
    return os;
  }

}


