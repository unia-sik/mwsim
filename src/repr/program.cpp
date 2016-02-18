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
 * $Id: program.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file program.cpp
 * @brief Program representation
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/program.h>

using namespace std;

namespace mwsim {

  Program::Program() {
  }


  Program::Program(const Program& rhs) : list<Command *>() {
    if (rhs.empty()) {
      return;
    }
    for (Program::const_iterator it = rhs.begin();
	 it != rhs.end(); ++it) {
      /*Command *cmd = (*it)->clone();
      cout << "\tCcloned cmd @ " << cmd << endl;
      push_back(cmd);*/
      push_back((*it)->clone());
    }
    //cout << "\tCcreated list, begin @ " << *begin() << " end @ " << *end() << " size " << size() << endl;
  }


  Program::~Program() {
    //cout << "Destroying program @ " << this << endl;
    clear();
  }


  Program& Program::operator= (const Program& rhs) {
    clear();
    if (rhs.empty()) {
      return *this;
    }
    for (Program::const_iterator it = rhs.begin();
	 it != rhs.end(); ++it) {
      /*Command *cmd = (*it)->clone();
      cout << "\t=cloned cmd @ " << cmd << endl;
      push_back(cmd);*/
      push_back((*it)->clone());
    }
    return *this;
  }


  void Program::clear() {
    /*
    cout << "\tClearing list, begin @ " << *begin()
	 << " end @ " << *end() << " size " << size() << endl;
    */
    for (Program::const_iterator it = begin();
	 it != end(); ++it) {
      //cout << "\tclearing cmd @ " << *it << endl;
      delete *it;
    }
    list::clear();
  }
  

  ostream & operator<<(std::ostream &os, const Program& program) {
    for (Program::const_iterator it = program.begin();
	 it != program.end(); ++it) {
      os << *(*it) << endl;
    }
    return os;
  }


} // NS
