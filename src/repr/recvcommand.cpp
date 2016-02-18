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
 * $Id: recvcommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file recvcommand.cpp
 * @brief Representation of message receipt (after executing mwsim::WaitCommand
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>

namespace mwsim {

  RecvCommand::RecvCommand(VarParm* _source, std::string _event)
    : Command(CT_RECV), source(_source), event(_event)
  {
  }


  RecvCommand::RecvCommand(const RecvCommand& rhs)
    : Command(CT_RECV), event(rhs.event)
  {
    source = new VarParm(*rhs.source);
  }


  RecvCommand::~RecvCommand() {
    delete source;
  }


  Command* RecvCommand::clone() const {
    return new RecvCommand(*this);
  }


  const VarParm* RecvCommand::getSource() const {
    return source;
  }

  const std::string & RecvCommand::getEvent() const {
    return event;
  }

}

