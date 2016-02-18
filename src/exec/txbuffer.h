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
 * $Id: txbuffer.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file txbuffer.h
 * @brief Simulation of TX buffer behaviour
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <stdint.h>

namespace mwsim {

  class TxBuffer {
  public:
    TxBuffer(int _size, int _txPeriod);
    ~TxBuffer();

    /**
     * put one word into the buffer
     * @return the latency -- blocking times are emulated
     */
    int put(uint64_t now);

  private:
    void adjustLevel(uint64_t now);

    int size;
    int txPeriod;

    int level;

    uint64_t lastPut;
    int overFlow;
  };


} // NS
