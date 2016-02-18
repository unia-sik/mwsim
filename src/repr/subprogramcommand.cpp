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
 * $Id: subprogramcommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file subprogramcommand.cpp
 * @brief Abstract class for commands that have subprograms
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>
#include <repr/program.h>

namespace mwsim {

  SubProgramCommand::SubProgramCommand(CommandType _type, Program *_subProgram)
    : Command(_type), subProgram(_subProgram)
  {
  }


  SubProgramCommand::~SubProgramCommand() {
    if (subProgram != NULL) {
      while (subProgram->size() > 0) {
	Command * cmd = subProgram->front();
	subProgram->pop_front();
	delete cmd;
      }
      delete subProgram;
    }
  }


  SubProgramCommand::SubProgramCommand(const SubProgramCommand& rhs)
    : Command(rhs)
  {
    if ( (rhs.subProgram == NULL) || (rhs.subProgram->empty()) ) {
      subProgram = NULL;
    }
    else {
      subProgram = new Program;
      *subProgram = *rhs.subProgram;
    }
  }


  const Program* SubProgramCommand::getSubProgram() const {
    return subProgram;
  }


} // NS
