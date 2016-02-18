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
 * $Id: txbuffer.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file txbuffer.cpp
 * @brief Simulation of TX buffer behaviour
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <exec/txbuffer.h>

namespace mwsim {

  TxBuffer::TxBuffer(int _size, int _txPeriod)
    : size(_size), txPeriod(_txPeriod), level(0), lastPut(0), overFlow(0) {
  }


  TxBuffer::~TxBuffer() {
  }


  int TxBuffer::put(uint64_t now) {
    int latency = 0;
    adjustLevel(now);

    if (level >= size) {
      // delay to send one entry
      latency += (txPeriod - overFlow);
      overFlow = 0;
    }
    level++;

    lastPut = now + latency;
    return latency;
  }

  void TxBuffer::adjustLevel(uint64_t now) {
    if (level <= 0)
      return;
    uint64_t dt = now - lastPut + overFlow;
    int sent = dt / txPeriod;
    overFlow = dt % txPeriod;
    level -= sent;
    if (level < 0)
      level = 0;
  }


} // NS
