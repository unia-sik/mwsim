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
 * $Id: program.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file program.h
 * @brief Program representation
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _REPR_PROGRAM_H
#define _REPR_PROGRAM_H 1

#include <iostream>
#include <list>


#include <repr/command.h>

namespace mwsim {

  class Program : public std::list<Command *> {
  public:
    Program();
    Program(const Program& rhs);
    virtual ~Program();

    Program& operator= (const Program& rhs);

    void clear();
  };


  std::ostream & operator<<(std::ostream &os, const Program& program);

} // NS

#endif /* !_REPR_PROGRAM_H */
