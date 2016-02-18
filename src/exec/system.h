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
 * $Id: system.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file system.h
 * @brief Manycore System
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _EXEC_SYSTEM_H
#define _EXEC_SYSTEM_H 1

#include <list>
#include <map>
#include <string>

#include <exec/node.h>
#include <exec/eventmessage.h>


/**
   System parameters
   - wordsize: used when byte counts must be converted

   - loadlatency: latency when loading one word from ROM (used by Node)

   - sendinitlatency: fixed overhead of any send operation
   - sendlatency: latency experienced by the sender for sending one word
     over NoC (accumulates)
   - recvinitlatency: fixed overhead of any recv operation
   - recvlatency: latency experienced by the receiver for receiving one word
     from the NoC (accumulates)

   - txinitlatency: fixed overhead for any NoC transmission
   - txlatency: node-to-node latency for transmitting one word over
     NoC (accumulates)
 */

namespace mwsim {

  class ManyCoreSystem {
  public:
    ManyCoreSystem(const std::map<std::string, int> & _parameters, Node **_nodes);
    ~ManyCoreSystem();

    int getValue(const std::string& key);

    /*
    void sendMessage(int source, int target, uint64_t sendTime,
		     const std::string& msg, int size);
    */
    void sendMessage(EventMessage *message);
    bool hasMoreMessages();
    NodeId deliverMessage();

  private:
    //class Message;
    std::map<std::string, int> parameters;
    Node **nodes;
    
    int nNodes;
    //int nocLatency;
    std::list<EventMessage *> messageList; ///< keeps messages in increasing order of their #EventMessage::arrivalTime
  };


} // NS

#endif /* !_EXEC_SYSTEM_H */
