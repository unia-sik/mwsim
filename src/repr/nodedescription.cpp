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
 * $Id: nodedescription.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file nodedescription.cpp
 * @brief Description of one or multiple nodes, created by parser infrastructure
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */


#include <repr/nodedescription.h>

namespace mwsim {

  NodeDescription::~NodeDescription() {
    //std::list<int> *nodes;
    //std::list<Command*> *program;
    if (nodes != NULL)
      delete nodes;
    if (program != NULL) {
      while (program->size() > 0) {
	Command * cmd = program->front();
	program->pop_front();
	delete cmd;
      }
      delete program;
    }
  }


} // NS
