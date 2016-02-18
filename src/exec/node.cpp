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
 * $Id: node.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file node.cpp
 * @brief Node that executes mwsim code
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <exec/node.h>

#include <cstdlib>
#include <iostream>

//#define NDEBUG
#include <assert.h>

#include <exec/system.h>

using namespace std;

namespace mwsim {

#define LOG cout << id << ": "

  struct Node::ExecState {
    NodeState nState;
    ProgramState pState;
    
    ExecState(NodeState _ns = NS_UNKNOWN, ProgramState _ps = PS_UNKNOWN)
      : nState(_ns), pState(_ps) { }
  };


  Node::Node() 
    : id(-1), state(NS_NEW), program(NULL), time(0), timeOverflow(0),
      globalVariables(NULL), variables(NULL), nextRecvMessage(NULL),
      waitCycles(0), waitStart(-1),
      sendStartTime(0), sendFinishedTime(0), waitSendBufLatencies(0),
      bufferLevel(0), bufferLevelAdjustTime(0),
      recvLatencies(0), recvWordsSum(0), firstWait(0)
  {
    cout << "Created empty node " << id << " @ " << this << " with program " << program << endl;
  }


  Node::Node(NodeId _id, Program* _program, const VariableStore *_variables) 
    : id(_id), state(NS_NEW), program(_program), time(0),
      globalVariables(_variables), variables(NULL), nextRecvMessage(NULL),
      waitCycles(0), waitStart(-1), sendStartTime(0), sendFinishedTime(0),
      waitSendBufLatencies(0), bufferLevel(0), bufferLevelAdjustTime(0),
      recvLatencies(0), recvWordsSum(0), firstWait(0)
  {
    Context* ctx = new Context(*program, id, globalVariables);
    contextStack.push(ctx);
    //cout << "initial context created @ " << ctx << endl;
    //cout << "Node " << id << " will execute program:" << endl;
    //cout << *program;
    cout << "Created node " << id << " @ " << this << " with program " << program << endl;
  }


  Node::Node(__attribute__((unused)) const Node& rhs) {
    cerr << "Copying Node objects is forbidden!" << endl;
    abort();
  }


  Node::~Node() {
    //cout << "Destroying node " << id << " @ " << this << " with program " << program << endl;
    if (program != NULL) {
      delete program;
    }
    while (!contextStack.empty()) {
      Context* ctx = contextStack.top();
      contextStack.pop();
      delete ctx;
    }
  }


  uint64_t Node::getTime() const {
    return time;
  }


  uint64_t Node::getTimeOverflows() const {
    return timeOverflow;
  }


  uint64_t Node::getWaitCycles() const {
    return waitCycles;
  }


  uint64_t Node::getWaitSendBufLatencies() const {
    return waitSendBufLatencies;
  }


  uint64_t Node::getRecvLatencies() const {
    return recvLatencies;
  }


  uint64_t Node::getRecvWords() const {
    return recvWordsSum;
  }


  uint64_t Node::getFirstWait() const {
    return firstWait;
  }


  Node::NodeState Node::getState() const {
    return state;
  }


  const string& Node::getWaitEvent() const {
    return waitEvent;
  }


  void Node::setId(int _id) {
    id = _id;
  }


  int Node::getId() const {
    return id;
  }


  void Node::setProgram(Program *_program) {
    program = _program;
  }


  void Node::setSystem(ManyCoreSystem *_system) {
    system = _system;
  }


  Node::NodeState Node::execStep() {

    Context *ctx = contextStack.top();

    // get next command from context
    Command *cmd = ctx->getCurrentCommand();
    if (cmd == NULL) { // nothing to do
      //cerr << "Execution of node " << id << " is already finished, returning" << endl;
      state = NS_FINISHED;
      return state;
    }
    variables = ctx->getVariables();
    //cout << "Node " << id << " executing command: " << *cmd << endl;
    // switch command.type
    //NodeState state = NS_UNKNOWN;
    ExecState eState;
    switch (cmd->getType()) {
    case Command::CT_EXEC:
      eState = execCmdExec(dynamic_cast<ExecCommand *>(cmd));
      break;

    case Command::CT_LOAD:
      eState = execCmdLoad(dynamic_cast<LoadCommand *>(cmd));
      //eState = execCmdLoad((LoadCommand*)cmd);
      break;

    case Command::CT_SEND:
      eState = execCmdSend(dynamic_cast<SendCommand *>(cmd));
      break;

    case Command::CT_RECV:
      eState = execCmdRecv(dynamic_cast<RecvCommand *>(cmd));
      break;

    case Command::CT_WAIT:
      eState = execCmdWait(dynamic_cast<WaitCommand *>(cmd));
      break;

    case Command::CT_CALC:
      eState = execCmdCalc(dynamic_cast<CalcCommand *>(cmd));
      break;

    case Command::CT_FOREACH:
      eState = execCmdForeach(dynamic_cast<ForeachCommand *>(cmd));
      break;

    case Command::CT_PARWAIT:
      eState = execCmdParwait(dynamic_cast<ParwaitCommand *>(cmd));
      break;

    default:
      eState = ExecState(NS_UNKNOWN, PS_UNKNOWN);
    }

    variables = NULL;

    switch (eState.pState) {
    case PS_EXECUTED:
      if (!ctx->advancePC()) {
	if (contextStack.size() == 1) {
	  state = NS_FINISHED;
	}
	else {
	  //cout << "Popping context from stack" << endl;
	  contextStack.pop();
	  delete ctx;
	  state = eState.nState;
	}
      }
      else {
	state = eState.nState;
      }
      break;

    case PS_WAITING:
      assert(eState.nState == NS_WAITING);
      state = eState.nState;
      break;

    case PS_SUBPROGRAM:
      assert(eState.nState == NS_READY);
      state = eState.nState;
      break;

    case PS_UNKNOWN:
    default:
      cerr << "Command execution returned with PS_UNKNOWN, exiting." << endl;
      abort();
    }
    return state;
  }

  /**
    This function will only be called if the node is in #NS_WAITING
    or #NS_FINISHED state.
  */
  void Node::setMessage(EventMessage *msg) {
    inbox.push_back(msg);
    // check whether node is currently waiting for this message
    // if so, change state to NS_READY
    if (waitEvent == msg->msg) {
      state = NS_READY;
    }
    // advance local time if necessary
    if (time < msg->arrivalTimeFirst)
      time = msg->arrivalTimeFirst;
    //cout << "\tNode " << id << " received message @ " << time << endl;
  }


  Node::ExecState Node::execCmdExec(ExecCommand *cmd) {
    int duration = cmd->getCycles()->resolve(variables);
    uint64_t now = time;
    advanceTime(duration);
    LOG << "EXEC " << now << "+" << duration << "=" << time << endl;
    return ExecState(NS_READY, PS_EXECUTED);
  }


  Node::ExecState Node::execCmdLoad(LoadCommand *cmd) {
    uint64_t now = time;

    /*
      load_memory_latency = 32
      load_bus_latency = 2
      load_loop_init_latency = 76
      load_loop_exec_latency = 13
    */

    int memlat = system->getValue("load_memory_latency");
    int buslat = system->getValue("load_bus_latency");
    int initlat = system->getValue("load_loop_init_latency");
    int execlat = system->getValue("load_loop_exec_latency");

    //int latency = system->getValue("loadlatency");
    int bytes = cmd->getBytes()->resolve(variables);
    int wordsize = system->getValue("wordsize");
    int words = (bytes + wordsize - 1) / wordsize;

    /*
    VarParm::Type stt = cmd->getBytes()->getType();
    if (stt == VarParm::VP_NAME) {
      size = variables->getVar(cmd->getBytes()->getName());
    }
    else if (stt == VarParm::VP_NUMBER) {
      size = cmd->getBytes()->getNumber();
    }
    else {
      cerr << "Invalid Load parameter type: " << cmd->getBytes()->getType() << endl;
      abort();
    }
    */
    //advanceTime(( (size + system->getValue("wordsize") - 1) / system->getValue("wordsize")) * latency);

    int latency = initlat + words * (memlat + buslat + execlat);
    advanceTime(latency);
    LOG << "LOAD " << now << "+" << latency << "=" << time << endl;
    return ExecState(NS_READY, PS_EXECUTED);
  }


  /// @todo a node can only transmit one message at any time!
  Node::ExecState Node::execCmdSend(SendCommand *cmd) {
    uint64_t now = time;
    int target = cmd->getTarget()->resolve(variables);

    //int bufSize = system->getValue("tx_buffer_size");
    int bytes = cmd->getBytes()->resolve(variables);
    int wordsize = system->getValue("wordsize");
    int words = (bytes + wordsize - 1) / wordsize;
    int txperiod = system->getValue("tx_period");
    int initlat = system->getValue("send_loop_init_latency");
    int execlat = system->getValue("send_loop_exec_latency");
    int txbuf = system->getValue("tx_buffer_size");
    int txbufWords = (txbuf + wordsize - 1) / wordsize;
    uint64_t ostf = 0;
    uint64_t ostl = 0;

    cout << "BUFSIZE " << txbuf << "/" << txbufWords << endl;

    // execute loop init and first run in any case
    uint64_t latency = initlat;// + execlat;

    if ( (txbuf == -1) || (txperiod < execlat) ) {
      // simply send with regular loop latency
      // first tx to NoC
      // FIXME: timing calculation might be problematic here:
      // does not care wether other (in previous calls written) data must
      // be transmitted first.
      ostf = latency + execlat + txperiod - 1;
      // last tx to NoC
      int txlat = max(txperiod, execlat);
      ostl = ostf + words * txlat;
      latency += words * execlat;
      //cout << "\tOSTF: " << ostf << endl;
      //cout << "\tOSTL: " << ostl << endl;
    }
    else {
      // need to manage buffer
      int sentWords = 0;
      adjustBufferLevel(time + latency);

      // calculate ostf!

      while (sentWords < words) {
	adjustBufferLevel(time + latency);
	int space = txbufWords - bufferLevel;
	cout << "\twords=" << words << " sentWords=" << sentWords
	     << " space=" << space << endl;
	if (space != 0) {
	  if (sentWords == 0) {
	    ostf = latency + execlat + bufferLevel * txperiod;
	    //cout << "\tOSTF: " << ostf << endl << "\t\t";
	  }
	  int sw = min(space, (words - sentWords));
	  bufferLevel += sw;
	  sentWords += sw;
	  latency += sw * execlat;
	  //cout << sw << " ";

	}
	else {
	  latency += txperiod;
	  waitSendBufLatencies += txperiod;
	  //cout << " -";
	}
      }
      adjustBufferLevel(time + latency);
      ostl = latency + bufferLevel * txperiod;
      //cout << "\n\tOSTL: " << ostl << endl;
    }
    // FIXME: handle overflows!
    assert(ostf <= ostl);
    
    /*
    // if another transmission is still in progress,
    // we have to wait until it is finished
    if (sendFinishedTime > time) {
      //cout << "\t" <<  id << " needs to wait for end of ongoing transmission" << endl;
      advanceTime(sendFinishedTime - time);
    }


    // first tx to NoC
    uint64_t ostf = initlat + execlat + txperiod - 1;
    // last tx to NoC
    uint64_t ostl = ostf + (words - 1) * txperiod;

    // send message via ManyCoreSystem
    //system->sendMessage(id, target, time, cmd->getEvent(), cmd->getBytes());
    */
    sendStartTime = time + ostf;
    sendFinishedTime = time + ostl;
    system->sendMessage(new EventMessage(id, target, now, sendStartTime, sendFinishedTime,
					 cmd->getEvent(), bytes));

    // determine local latency and advance time
    //int latency = initlat + words * execlat;
    advanceTime(latency);
    LOG << "SEND " << now << "+" << latency << "=" << time
	<< " " << cmd->getEvent() << " -> " << target << " @ "
	<< sendStartTime << "--" << sendFinishedTime
	<< endl;
    return ExecState(NS_READY, PS_EXECUTED);
  }


  Node::ExecState Node::execCmdRecv(RecvCommand *cmd) {
    uint64_t now = time;
    // can only receive a message that was waited for
    // -- command MUST be preceeded by 'wait' or 'parwait'
    // determine local latency and advance time

    //nextRecvMessage;
    int source;
    VarParm::Type stt = cmd->getSource()->getType();
    if (stt == VarParm::VP_NAME) {
      source = variables->getVar(cmd->getSource()->getName());
    }
    else if (stt == VarParm::VP_NUMBER) {
      source = cmd->getSource()->getNumber();
    }
    else {
      cerr << "Invalid recvSource parameter type: " << cmd->getSource()->getType() << endl;
      abort();
    }

    if (source != nextRecvMessage->source) {
      cerr <<  "Message source " << nextRecvMessage->source
	   << " does not match recv source " << source << endl;
      abort();
    }

    //cout << "Node " << id << " receiving " << nextRecvMessage->msg << " from node " << nextRecvMessage->source << endl;

    int wordsize = system->getValue("wordsize");
    int words = (nextRecvMessage->size + wordsize - 1) / wordsize;

    int initlat = system->getValue("recv_loop_init_latency");
    int execlat = system->getValue("recv_loop_exec_latency");
    int latency = initlat + execlat; // receive first word
    int txPeriod = system->getValue("tx_period");
    int recvWords = 1;

    // first take data from buffer
    while ( (recvWords < words) &&
	    (nextRecvMessage->arrivalTimeFirst + recvWords * system->getValue("tx_period") < time + latency) ) {
      ++recvWords;
      latency += execlat;
    }
    
    // add WCWT for next word
    latency += txPeriod - 1;

    // then receive those that are still underway
    int recvlatency = max(execlat, txPeriod);
    if (recvWords < words) {
      latency += recvlatency * (words - recvWords);
    }

    //assert( (time + latency) >= nextRecvMessage->arrivalTimeLast );
    // there is a problem with sending - we do not track the actual timing
    // of single messages. Thus it can happen that the above assertion fails.
    // we solve this by simply advancing to the last arrival time

    if ((time + latency) >= nextRecvMessage->arrivalTimeLast) {
      // use arrival time of last word and perform one loop iteration
      latency = nextRecvMessage->arrivalTimeLast - time + execlat;
    }

    //int latency = initlat + words * execlat; /// @todo ajdust for "message transmission still in progress"

    //int latency = nextRecvMessage->size * system->getValue("recvlatency");
    advanceTime(latency);

    LOG << "RECV " << now << "+" << latency << "=" << time
	<< " " << cmd->getEvent() << " <- " << nextRecvMessage->source
	<< endl;

    uint64_t msgLatency = time - nextRecvMessage->sendCallTime;
    recvLatencies += msgLatency;
    recvWordsSum += words;

    delete nextRecvMessage;
    nextRecvMessage = NULL;
    return ExecState(NS_READY, PS_EXECUTED);
  }


  Node::ExecState Node::execCmdWait(WaitCommand *cmd) {
    uint64_t now = time;
    waitEvent = cmd->getEvent();
    waitEventSource = cmd->getSource()->resolve(variables);
    // check whether message is already available
    list<EventMessage*>::iterator it = inbox.begin();
    while(it != inbox.end()) {
      if ( ((*it)->msg == waitEvent) && ((*it)->source == waitEventSource) )
	break;
      ++it;
    }
    if (it != inbox.end()) {
      //cout << "Wait received message \"" << waitEvent << "\" in cycle " << time << endl;
      nextRecvMessage = *it;
      LOG << "WAIT recv " << waitEvent << " <-" << nextRecvMessage->source << " @ " << time << endl;
      inbox.erase(it);
      /*
      if (cmd->getVar() != "") {
	//cout << "\tPutting source " << nextRecvMessage->source
	//     << " as \"" << cmd->getVar() << "\" into store" << endl;
	variables->putVar(cmd->getVar(), nextRecvMessage->source);
      }
      */
      advanceWaitCycles();
      return ExecState(NS_READY, PS_EXECUTED);
    }
    else {
      startWait();
      LOG << "WAIT @ " << now << " for " << waitEvent << endl;
      return ExecState(NS_WAITING, PS_WAITING);
    }
  }


  Node::ExecState Node::execCmdCalc(CalcCommand *cmd) {
    int result = cmd->getExpression()->evaluate(variables);
    cout << "\tCalculated variable \"" << cmd->getVar() << "\"= "
	 << result << endl;
    variables->putVar(cmd->getVar(), result);

    return ExecState(NS_READY, PS_EXECUTED);
  }


  Node::ExecState Node::execCmdForeach(ForeachCommand *cmd) {
    //VariableStore* vs = new VariableStore(contextStack.top()->getVariables());
    if (!cmd->isRunning()) {
      cmd->init();
    }
    if (cmd->isFinished()) {
      // return and signal advancement of PC
      return ExecState(NS_READY, PS_EXECUTED);
    }
    //cout << "creating new context..." << endl;
    Context* ctx = new Context(*cmd->getSubProgram(), variables);
    int next = cmd->next();
    ctx->getVariables()->putVar(cmd->getVar(), next);
    contextStack.push(ctx);
    LOG << "FOREACH " << cmd->getVar() << "=" << next << endl;
    // return and signal NO advancement of PC
    //return NS_READY; // actually, need to notify execStep to not advance PC
    return ExecState(NS_READY, PS_SUBPROGRAM);
  }


  Node::ExecState Node::execCmdParwait(ParwaitCommand *cmd) {
    uint64_t now = time;
    if (cmd->getNodeIds()->size() == 0) {
      LOG << "PARWAIT finished @ " << time << endl;
      return ExecState(NS_READY, PS_EXECUTED);
    }
    bool found = false;
    list<int>::const_iterator itNodeIds;
    list<EventMessage*>::iterator it;
    waitEvent = cmd->getEvent();

    // FIXME: check whether we need this message from the sender node at all
    // i.e. check whether this message id was not received before
    // FIXME: remove itNodeIds in this loop!

    it = inbox.begin();
    while ( (it != inbox.end()) && (!found) ) {
      cout << "Checking " << (*it)->msg << " from " << (*it)->source << endl;
      if ((*it)->msg != waitEvent) 
	continue;
      itNodeIds = cmd->getNodeIds()->begin();
      cout << "\tcheck node: ";
      while ( (itNodeIds != cmd->getNodeIds()->end()) && (!found) ) {
	cout << *itNodeIds << " ";
	if (*itNodeIds == (*it)->source) {
	  found = true;
	  break;
	}
	++itNodeIds;
      }
      cout << endl;
      if (found)
	break;
      else
	++it;
    }
    
    /*
    while ( (itNodeIds != cmd->getNodeIds()->end()) && (!found) ) {
      it = inbox.begin();
      while(it != inbox.end()) {
	if ((*it)->msg == waitEvent) // FIXME: && (*it)->source in senderlist &&
	  break;
	++it;
      }
      if (it != inbox.end()) {
	//cout << "parwait found source/event" << endl;
	found = true;
      }
      ++itNodeIds;
    }
    */

    if (found) {
      //--itNodeIds;
      nextRecvMessage = *it;
      inbox.erase(it);
      LOG << "PARWAIT recv " << waitEvent << " <-" << nextRecvMessage->source << " @ " << time << " (" << *itNodeIds << ")" << endl;
      //cout << "Parwait @ Node " << id << " received " << nextRecvMessage->msg << " from node " << nextRecvMessage->source << endl;
      //cout << "creating new context..." << endl;
      Context* ctx = new Context(*cmd->getSubProgram(), variables);
      ctx->getVariables()->putVar(cmd->getVar(), nextRecvMessage->source); //*itNodeIds);
      contextStack.push(ctx);
      cmd->received(*itNodeIds);
      advanceWaitCycles();
      return ExecState(NS_READY, PS_SUBPROGRAM);
    }
    else {
      LOG << "PARWAIT @ " << now << " for " << waitEvent
	  << " (SW=" << getWaitSendBufLatencies() << ")"
	  << endl;
      //cout << "Node " << id << " is waiting for \"" << waitEvent << "\"" << endl;
      startWait();
      return ExecState(NS_WAITING, PS_WAITING);
    }
  }


  void Node::advanceTime(uint64_t dt) {
    uint64_t oldTime = time;
    time += dt;
    //cout << "\tNode " << id << " executed " << dt << " cycles @ " << time << endl;
    if (time < oldTime) {
      ++timeOverflow;
    }
  }


  void Node::startWait() {
    waitStart = time;
  }


  void Node::advanceWaitCycles() {
    if (waitStart >= 0) {
      if ((uint64_t)waitStart < time) {
	uint64_t wc = time - waitStart;
	waitCycles += wc;
	LOG << "\twaited for " << (time-waitStart) << " cycles" << endl;
	if (firstWait == 0)
	  firstWait = wc;
      }
      waitStart = -1;
    }
  }


  void Node::adjustBufferLevel(uint64_t now) {
    if (bufferLevel <= 0)
      return;
    // FIXME: handle overflows of now?
    uint64_t dt = now - bufferLevelAdjustTime;
    int txPeriod = system->getValue("tx_period");
    int sent = dt / txPeriod;
    int overFlow = dt % txPeriod;
    bufferLevel -= sent;
    bufferLevelAdjustTime = now - overFlow;
    if (bufferLevel < 0)
      bufferLevel = 0;
  }

} // NS
