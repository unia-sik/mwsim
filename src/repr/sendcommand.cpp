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
 * $Id: sendcommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file sendcommand.cpp
 * @brief Sending of a message
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>

namespace mwsim {

  SendCommand::SendCommand(VarParm* _target, std::string _event, VarParm* _bytes)
    : Command(CT_SEND), bytes(_bytes), target(_target), event(_event)
  {
  }

  SendCommand::SendCommand(const SendCommand& rhs)
    : Command(CT_SEND), event(rhs.event)//, bytes(rhs.bytes)
  {
    target = new VarParm(*rhs.target);
    bytes = new VarParm(*rhs.bytes);
  }

  SendCommand::~SendCommand() {
    delete target;
    delete bytes;
  }
  
  
  Command* SendCommand::clone() const {
    return new SendCommand(*this);
  }


  const VarParm* SendCommand::getBytes() const {
    return bytes;
  }


  const VarParm* SendCommand::getTarget() const {
    return target;
  }

  const std::string &SendCommand::getEvent() const {
    return event;
  }

}

