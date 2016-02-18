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
 * $Id: nodedescription.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file nodedescription.h
 * @brief Input from parser
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _REPR_NODEDESCRIPTION_H
#define _REPR_NODEDESCRIPTION_H 1


#include <list>

#include <repr/command.h>
#include <repr/program.h>

namespace mwsim {

  class NodeDescription {
  public:
    /// takes ownership of parameters
    NodeDescription(std::list<int> *_nodes, Program* _program)
      : nodes(_nodes), program(_program) {}
    ~NodeDescription();

    const std::list<int> *getNodeList() const { return nodes; }
    const Program *getProgram() const { return program; }
    
  private:
    std::list<int> *nodes;
    Program *program;
    
  };

}


#endif /* !_REPR_NODEDESCRIPTION_H */
