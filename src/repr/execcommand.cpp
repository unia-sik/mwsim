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
 * $Id: execcommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file execcommand.cpp
 * @brief Command representing sequential execution
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>

namespace mwsim {

  ExecCommand::ExecCommand(VarParm* _cyc)
    : Command(CT_EXEC), cycles(_cyc)
  {
  }


  ExecCommand::ExecCommand(const ExecCommand& rhs)
    : Command(CT_EXEC)//, cycles(rhs.cycles)
  {
    cycles = new VarParm(*rhs.cycles);
  }


  ExecCommand::~ExecCommand() {
    delete cycles;
  }


  Command* ExecCommand::clone() const {
    return new ExecCommand(*this);
  }

  
  const VarParm* ExecCommand::getCycles() const {
    return cycles;
  }

} // NS
