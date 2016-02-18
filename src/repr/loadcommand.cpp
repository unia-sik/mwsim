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
 * $Id: loadcommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file loadcommand.cpp
 * @brief Representation for loading large chunks from ROM memory into RAM
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>

namespace mwsim {

  LoadCommand::LoadCommand(VarParm *_bytes)
    : Command(CT_LOAD), bytes(_bytes)
  {
  }


  LoadCommand::LoadCommand(const LoadCommand& rhs)
    : Command(CT_LOAD)//, bytes(rhs.bytes)
  {
    bytes = new VarParm(*rhs.bytes);
  }


  LoadCommand::~LoadCommand() {
    delete bytes;
  }


  Command* LoadCommand::clone() const {
    return new LoadCommand(*this);
  }

  
  const VarParm* LoadCommand::getBytes() const {
    return bytes;
  }

} // NS
