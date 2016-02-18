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
 * $Id: system.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file system.cpp
 * @brief Manycore System
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include "system.h"

#include <iostream>

using namespace std;

namespace mwsim {


  ManyCoreSystem::ManyCoreSystem(const map<string, int>& _parameters, Node **_nodes)
    : parameters(_parameters), nodes(_nodes)
  {
    nNodes = parameters["size"];
    /*nocLatency = parameters["noclatency"];
    if (nocLatency == 0) {
      cout << "Using default NoC latency (size)" << endl;
      nocLatency = nNodes;
      }*/
    for (int i = 0; i < nNodes; ++i) {
      nodes[i]->setSystem(this);
    }
  }


  ManyCoreSystem::~ManyCoreSystem() {
    if (messageList.size() > 0) {
      // delete remaining messages
      for (list<EventMessage *>::iterator it = messageList.begin();
	   it != messageList.end(); ++it) {
	delete *it;
      }
      messageList.clear();
    }
  }


  int ManyCoreSystem::getValue(const string& key) {
    int value = parameters[key];
    if (value == 0) {
      cerr << "Using standard value for system property \"" << key << "\"" << endl;
      return 1;
    }
    else {
      return value;
    }
  }

  /*
  void ManyCoreSystem::sendMessage(int source, int target, uint64_t sendTime,
				   const string& msg, int size) {
    sendMessage(new EventMessage(source, target, sendTime, msg, size));
  }
  */

  /// @todo a node can only transmit one message at any time!
  void ManyCoreSystem::sendMessage(EventMessage *message) {
    uint64_t atf = message->sendTimeFirst + getValue("tx_wctt");
    message->arrivalTimeFirst = atf;
    uint64_t atl = message->sendTimeLast + getValue("tx_wctt");
    message->arrivalTimeLast = atl;

    /*
    uint64_t at = message->sendTime + getValue("txinitlatency")
      + ( (message->size + getValue("wordsize") - 1)
	  / getValue("wordsize")) * getValue("txlatency");
    message->arrivalTime = at;
    */
    // insert into list heeding order
    list<EventMessage*>::iterator it = messageList.begin();
    while (it != messageList.end()) {
      if ( (*it)->arrivalTimeFirst > message->arrivalTimeFirst )
	break;
      ++it;
    }
    messageList.insert(it, message);
    //cout << "Enqueued message " << message->msg << " for node " << message->target << endl;
    //cout << "\tmessage will arrive @ " << atf << " - " << atl << endl;
  }


  bool ManyCoreSystem::hasMoreMessages() {
    return (messageList.size() > 0);
  }


  NodeId ManyCoreSystem::deliverMessage() {
    if (messageList.size() == 0) {
      // no messages queued!
      return NODE_ID_INVALID;
    }
    EventMessage* msg = messageList.front();
    cout << endl << "NoC: " << msg->msg << " " << msg->source << " -> "
	 << msg->target << " @ " << msg->arrivalTimeFirst << "--" 
	 << msg->arrivalTimeLast << endl;
    //cout << "Delivering message from " << msg->source << " to " << msg->target
    // << " arriving in cycle " << msg->arrivalTimeFirst << " - " << msg->arrivalTimeLast << endl;
    nodes[msg->target]->setMessage(msg);
    messageList.pop_front();
    return msg->target;
  }


} // NS
