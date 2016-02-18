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
 * $Id: parwaitcommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file parwaitcommand.cpp
 * @brief Parallel waiting for same event message from multiple nodes
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>
#include <repr/program.h>

#include <list>

using namespace std;

namespace mwsim {

  ParwaitCommand::ParwaitCommand(std::string _event, std::list<NodeId>* _nodeIds, std::string _var, Program* _commands)
    : SubProgramCommand(CT_PARWAIT, _commands), event(_event), var(_var), nodeIds(_nodeIds)
  {
    //cout << "Created new ParwaitCommand with program @ " << _commands << endl;
    //cout << "\twaiting for event " << event << endl;
  }


  ParwaitCommand::~ParwaitCommand() {
    if (nodeIds != NULL)
      delete nodeIds;
  }


  ParwaitCommand::ParwaitCommand(const ParwaitCommand& rhs)
    : SubProgramCommand(rhs), event(rhs.event), var(rhs.var)
  {
    if ( (rhs.nodeIds == NULL) || (rhs.nodeIds->empty()) ) {
      nodeIds = NULL;
    }
    else {
      nodeIds =new list<NodeId>;
      *nodeIds = *rhs.nodeIds;
    }
  }


  Command* ParwaitCommand::clone() const {
    return new ParwaitCommand(*this);
  }


  const std::string& ParwaitCommand::getEvent() const {
    return event;
  }


  const std::string& ParwaitCommand::getVar() const {
    return var;
  }


  const std::list<NodeId>* ParwaitCommand::getNodeIds() const {
    return nodeIds;
  }


  void ParwaitCommand::received(NodeId id) {
    nodeIds->remove(id);
    //cout << "ParwaitCommand removed " << id << " from list, now waiting for " << nodeIds->size() << " more messages." << endl;
  }

  void ParwaitCommand::printLong(std::ostream &os) const {
    //os << (Command) cmd << endl;
    print(os);
    os << endl;
    os << "\t" << var << " : [";
    list<NodeId>::const_iterator it = nodeIds->begin();
    if (it != nodeIds->end()) {
      os << *it;
    }
    for ( ; it != nodeIds->end(); ++it) {
      os << "," << *it;
    }
    os << "]" << endl;
    for (list<Command *>::const_iterator itc = getSubProgram()->begin();
	 itc != getSubProgram()->end(); ++itc) {
      os << "\t" << **itc << endl;
    }
  }


  std::ostream & operator<<(std::ostream &os, const ParwaitCommand& cmd) {
    cmd.printLong(os);
    return os;
  }

}


