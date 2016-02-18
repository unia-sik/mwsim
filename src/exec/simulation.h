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
 * $Id: simulation.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file simulation.h
 * @brief Simulation
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/nodedescription.h>

#include <iostream>
#include <list>
#include <map>

#include <exec/node.h>
#include <exec/system.h>

namespace mwsim {

  class Simulation {
  public:
    Simulation(const std::list<NodeDescription *> &lnd,
	       std::map<std::string, int> parameters,
	       const VariableStore *variables);
    virtual ~Simulation();

    void run();
    bool isFinished() const;

  private:

    void warmUp();
    void step();

    const std::list<NodeDescription *> &nodeDescriptions;
    std::map<std::string,int> systemParameters;

    int size;
    Node **nodes;
    ManyCoreSystem *system;
  };

} // NS
