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
 * $Id: eventmessage.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file eventmessage.h
 * @brief Message representation
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <exec/node.h>

#include <stdint.h>

namespace mwsim {

  class EventMessage {
  public:
  EventMessage(int _source, int _target, uint64_t _sendCallTime,
		 uint64_t _sendTimeFirst, uint64_t _sendTimeLast,
		 std::string _msg, int _size)//, uint64_t _arrivalTime=0)
    : source(_source), target(_target), sendCallTime(_sendCallTime),
      sendTimeFirst(_sendTimeFirst), sendTimeLast(_sendTimeLast),
      /*arrivalTime(_arrivalTime),*/ msg(_msg), size(_size)
    { }


    NodeId source;
    NodeId target;
    uint64_t sendCallTime;
    //uint64_t sendTime; /// @deprecated
    //uint64_t arrivalTime; /// @deprecated
    uint64_t sendTimeFirst;
    uint64_t sendTimeLast;
    uint64_t arrivalTimeFirst;
    uint64_t arrivalTimeLast;
    std::string msg;
    int size;
  };

} // NS
