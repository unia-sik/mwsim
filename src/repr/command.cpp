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
 * $Id: command.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file command.cpp
 * @brief Command representation
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>

namespace mwsim {

  const char * Command::CMD_NAMES[] = {
    "EXEC",
    "LOAD",
    "SEND",
    "RECV",
    "WAIT",
    "CALC",
    "FOREACH",
    "PARWAIT",
  };



  Command::Command(CommandType _type)
    : type(_type)
  {
    
  }

  
  Command::~Command() {
  }
  

  Command::Command(const Command& rhs)
    : type(rhs.type)
  {
  }
    
  /*
  Command* Command::clone() const {
    return new Command(*this);
  }
  */

  Command::CommandType Command::getType() const {
    return type;
  }

  
  std::string Command::getCommand() const {
    return CMD_NAMES[type];
  }


  void Command::print(std::ostream &os) const {
    os << "CMD: " << Command::CMD_NAMES[type];
  }


  void Command::printLong(std::ostream &os) const {
    print(os);
  }


  std::ostream & operator<<(std::ostream &os, const Command& cmd) {
    cmd.print(os);
    return os;
  }


} // NS
