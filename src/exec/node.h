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
 * $Id: node.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file node.h
 * @brief Node that executes mwsim code
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _EXEC_NODE_H
#define _EXEC_NODE_H 1

#include <exec/context.h>
#include <tools/variablestore.h>

//#include "eventmessage.h"

#include <repr/program.h>

#include <stdint.h>
#include <list>
#include <stack>


namespace mwsim {
  
  class ManyCoreSystem;
  class EventMessage;

  class Node {
  public:

    enum NodeState {
      NS_UNKNOWN = 0, ///< something didn't work out
      NS_NEW, ///< Node was newly created
      NS_READY, ///< ready to execute code
      NS_WAITING, ///< waiting for event (message)
      NS_FINISHED ///< program execution is finished
    };

    enum ProgramState {
      PS_UNKNOWN = 0, ///< something didn't work out
      PS_EXECUTED, ///< advance to next instruction
      PS_WAITING, ///< wait for event (message), do not advance
      PS_SUBPROGRAM ///< subprogram created, do not advance
    };

    Node();
    /// takes ownership of program parameter
    Node(NodeId _id, Program *_program, const VariableStore *_variables);
    virtual ~Node();

    uint64_t getTime() const;
    uint64_t getTimeOverflows() const;
    uint64_t getWaitCycles() const;
    uint64_t getWaitSendBufLatencies() const;
    uint64_t getRecvLatencies() const;
    uint64_t getRecvWords() const;
    uint64_t getFirstWait() const;
    NodeState getState() const;
    const std::string& getWaitEvent() const;

    void setId(NodeId _id);
    NodeId getId() const;
    void setProgram(Program *_program);
    void setSystem(ManyCoreSystem *_system);

    NodeState execStep();
    void setMessage(EventMessage *msg);

  private:

    struct ExecState;

    // do not make copies of this object
    Node(const Node& rhs);

    ExecState execCmdExec(ExecCommand *cmd);
    ExecState execCmdLoad(LoadCommand *cmd);
    ExecState execCmdSend(SendCommand *cmd);
    ExecState execCmdRecv(RecvCommand *cmd);
    ExecState execCmdWait(WaitCommand *cmd);
    ExecState execCmdCalc(CalcCommand *cmd);
    ExecState execCmdForeach(ForeachCommand *cmd);
    ExecState execCmdParwait(ParwaitCommand *cmd);


    void advanceTime(uint64_t dt);
    void startWait();
    void advanceWaitCycles();
    void adjustBufferLevel(uint64_t now);

    NodeId id;
    NodeState state;
    std::string waitEvent; // the event the node is currently waiting for
    NodeId waitEventSource; // the ID of the sender node
    Program* program;
    uint64_t time;
    uint64_t timeOverflow;
    ManyCoreSystem *system;
    std::stack<Context *> contextStack;
    const VariableStore *globalVariables;
    VariableStore *variables; ///< only valid during execStep
    std::list<EventMessage*> inbox;
    EventMessage *nextRecvMessage; ///< this message will be handled by the next receive instruction
    uint64_t waitCycles;
    int64_t waitStart;
    uint64_t sendStartTime;
    uint64_t sendFinishedTime;
    uint64_t waitSendBufLatencies;

    int64_t bufferLevel;
    uint64_t bufferLevelAdjustTime;

    uint64_t recvLatencies;
    uint64_t recvWordsSum;
    uint64_t firstWait;

  };
  
} // NS

#endif /* !_EXEC_NODE_H */
