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
 * $Id: simulation.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file simulation.cpp
 * @brief Simulation
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <exec/simulation.h>

#include <cstdlib>
#include <cstring>

//#include <stdint.h>
#include <cstdint>

using namespace std;

namespace mwsim {

  const char* PFX_STAT = "=STAT= ";
  
  Simulation::Simulation(const list<NodeDescription *> &lnd,
			 map<string, int> parameters,
			 const VariableStore *variables)
    : nodeDescriptions(lnd), systemParameters(parameters)
  {
    size = parameters["size"];
    
    
    cout << "Creating " << size << " nodes..." << endl;
    //cout << "\tusing variables @ " << variables << endl;
    nodes = new Node*[size];
    memset(nodes, 0, size * sizeof(Node *));
    
    for (list<NodeDescription *>::const_iterator it = lnd.begin();
	 it != lnd.end(); ++it) {
      NodeDescription *nd = *it;
      const list<int>* descNodes = nd->getNodeList();
      for (list<int>::const_iterator in = descNodes->begin();
	   in != descNodes->end(); ++in) {
	int currentNode = *in;
	Program *prog = new Program;
	//cout << "Creating node " << currentNode << " with program:" << endl;
	//cout << *nd->getProgram();
	*prog = *nd->getProgram();
	//cout << "Copied program: " << endl << *prog;
	nodes[currentNode] = new Node(currentNode, prog, variables);
	/*nodes[currentNode].setId(currentNode);// = new Node(currentNode, prog);
	  nodes[currentNode].setProgram(prog);*/
      }
    }
    
    for (int i=0; i<size; ++i) {
      if ( (nodes[i] == NULL) || (nodes[i]->getId() < 0) ) {
	cerr << "Node " << i << " not defined!" << endl;
	abort();
      }
    }
    
    // construct system control object
    system = new ManyCoreSystem(parameters, nodes);
  }


  Simulation::~Simulation() {
    for (int i = 0; i < size; ++i) {
      delete nodes[i];
      //nodes[i] = NULL;
    }
    
    delete system;
    delete[] nodes;
  }


  void Simulation::run() {
    /*
    //while (!isFinished()) {
      for (int i = 0; i < size; ++i) {
	cout << endl << "Executing node " << i << endl;
	cout << "==================" << endl;
	while (nodes[i]->execStep() == Node::NS_READY) {
	  
	}
	// deliver messages just at end of round when all cores are waiting
	// to ensure arrival in correct order
	// FIXME: is this sufficient?
	// should possibly use another execution policy!
	// (1) 
	}
      //}
      */
    warmUp();
    cout << endl << "Warm-up finished!" << endl << endl;

    while (!isFinished()) {
      NodeId receiver = system->deliverMessage();
      if (receiver == NODE_ID_INVALID) {
	cout << "ATTENTION: EXECUTION TIMES MAY BE WRONG (HAD NO MORE MESSAGES)" << endl;
	cerr << "No more messages to deliver, exiting..." << endl;
	cerr << "\texecution times may be wrong/too low!" << endl;
	break;
      }
      while (nodes[receiver]->execStep() == Node::NS_READY);
    }

    cout << "Execution finished:" << endl;
    uint64_t maxCT = 0;
    uint64_t minCT = UINT64_MAX;
    uint64_t sumCT = 0;
    uint64_t sumWC = 0;
    for (int i = 0; i < size; ++i) {
      cout << PFX_STAT << "Node " << i << ": CT=" << nodes[i]->getTime()
	   << " W=" << nodes[i]->getWaitCycles()
	   << " CP=" << (nodes[i]->getTime() - nodes[i]->getWaitCycles())
	   << " OV=" << nodes[i]->getTimeOverflows()
	   << " RL=" << nodes[i]->getRecvLatencies()
	   << " RW=" << nodes[i]->getRecvWords()
	   << " FW=" << nodes[i]->getFirstWait()
	   << " W\\F=" << (nodes[i]->getWaitCycles() - nodes[i]->getFirstWait())
	   << " SW=" << nodes[i]->getWaitSendBufLatencies()
	   << endl;
      uint64_t t = nodes[i]->getTime();
      sumCT += t;
      if (t > maxCT)
	maxCT = t;
      if (t < minCT)
	minCT = t;
      sumWC += nodes[i]->getWaitCycles();
    }
    cout << PFX_STAT << "maxCT=" << maxCT << " minCT=" << minCT 
	 << " avgCT=" << (sumCT/size) << " waitCT=" << (sumWC/size) << endl;
  }

  
  bool Simulation::isFinished() const {
    for (int i = 0; i < size; ++i) {
      if (nodes[i]->getState() != Node::NS_FINISHED)
	return false;
    }
    return true;
  }

  /// advance each node to its first waiting point
  void Simulation::warmUp() {
    for (int i = 0; i < size; ++i) {
      cout << endl << "Warming up node " << i << endl;
      cout << "==================" << endl;
      while (nodes[i]->execStep() == Node::NS_READY) {
      }
    }
  }

  /// deliver one message, advance node to next waiting point
  void Simulation::step() {
  }

  
} // NS
