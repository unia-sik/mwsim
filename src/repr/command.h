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
 * $Id: command.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file command.h
 * @brief Command representation
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _REPR_COMMAND_H
#define _REPR_COMMAND_H 1

#include <iostream>
#include <list>
#include <stdint.h>
#include <string>

#include <repr/mexpression.h>
#include <repr/varparm.h>

namespace mwsim {
  
  typedef int32_t NodeId;
#define NODE_ID_INVALID ((NodeId)-1)
  
  
  /**
     
   */
  class Command {
    
  public:
    
    /**
     * These constants are used by subclasses for identification.
     * If you add a new command type, make also sure to extend CMD_NAMES
     * in the right place!
     */
    enum CommandType {
      CT_EXEC = 0,
      CT_LOAD,
      CT_SEND,
      CT_RECV,
      CT_WAIT,
      CT_CALC,
      CT_FOREACH,
      CT_PARWAIT,
    };
    
    static const char * CMD_NAMES[];
    
  public:
    
    Command(Command::CommandType _type);
    virtual ~Command();
    Command(const Command& rhs);
    
    CommandType getType() const;
    std::string getCommand() const;
    
    /// MUST be overwritten by subclasses
    virtual Command *clone() const = 0;
    
    void print(std::ostream &os) const;
    void printLong(std::ostream &os) const;
    
	
  protected:
    
  private:
    CommandType type;
    
  };
  
  class Program;
  

  /**
   * Models sequential program execution.
   * Code represented by this program can be executed without any wait states.
   * The execution time is directly taken from your preferred WCET tool
   */
  class ExecCommand : public Command {
  public:
    /**
     * @param _cyc execution time
     */
    ExecCommand(VarParm* _cyc);
    ExecCommand(const ExecCommand& rhs);
    virtual ~ExecCommand();
    
    virtual Command *clone() const;
    
    virtual const VarParm* getCycles() const;
    
  private:
    VarParm* cycles;
  };
  

  /**
   * Represents loading from non-local memory, e.g. flash or SSD.
   * Parameters for latencies are specified in the system description.
   */
  class LoadCommand : public Command {
  public:
    /**
     * @param _bytes number of bytes that are loaded from memory
     */
    LoadCommand(VarParm* _bytes);
    LoadCommand(const LoadCommand& rhs);
    virtual ~LoadCommand();
    
    virtual Command *clone() const;
    
    virtual const VarParm* getBytes() const;
    
  private:
    VarParm* bytes;
  };
  
  
  /**
   * Send a message over the NoC.
   * Parameters for latencies are specified in the system description.
   */
  class SendCommand : public Command {
  public:
    /**
     * @param _target destination node of the message. SendCommand takes
     * ownership of this variable and automatically frees its memory.
     * @param _event a name/identifier for the message. This name is used
     * by the WaitCommand and RecvCommand.
     * @param _bytes messsage size in bytes
     */
    SendCommand(VarParm* _target, std::string _event, VarParm* _bytes);
    SendCommand(const SendCommand& rhs);
    virtual ~SendCommand();
    
    virtual Command *clone() const;
    
    virtual const VarParm* getBytes() const;
    virtual const VarParm* getTarget() const;
    virtual const std::string &getEvent() const;
    
  private:
    VarParm* bytes;
    VarParm* target;
    std::string event;
  };


  /**
   * Receive a message from the NoC.
   * Before receiving the message, the node must wait for its arrival
   * using the WaitCommand
   */
  class RecvCommand : public Command {
  public:
    /**
     * @param _source source node of the message. RecvCommand takes
     * ownership of this variable and automatically frees its memory.
     * @param _event Name/identifier of the message, as declared by the
     * corresponding SendCommand.
     */
    RecvCommand(VarParm* _source, std::string _event);
    RecvCommand(const RecvCommand& rhs);
    virtual ~RecvCommand();

    virtual Command *clone() const;

    const VarParm* getSource() const;
    const std::string & getEvent() const;

  private:
    VarParm* source;
    std::string event;
  };
  
  
  /**
   * Wait for the arrival of a message over the NoC.
   */
  class WaitCommand : public Command {
  public:
    /**
     * @param _event Name/identifier of the message, as declared by the
     * corresponding SendCommand.
     * @param _var if specified, the message sender's NodeId is put into the
     * program's variable space under this name
     */
    //WaitCommand(const std::string & _event, const std::string & _var = "");
    WaitCommand(const std::string & _event, VarParm* _source);
    WaitCommand(const WaitCommand& rhs);
    virtual ~WaitCommand();
    
    virtual Command *clone() const;
    
    const std::string & getEvent() const;
    const VarParm* getSource() const;
    //const std::string& getVar() const;
    
  private:
    std::string event;
    VarParm* source;
    //std::string var; // holds the sender of the message
  };

  
  /**
   * Use this command for internal calculation, e.g. of send/recv node ids.
   * This command does not introduce any execution time into the WCET,
   * it is only provided for comfort to ease writing of scripts.
   */
  class CalcCommand : public Command {
  public:
    /**
     * @param _var Variable affected by this command
     * @param _expression mathematical expression calculated by this command.
     */
    CalcCommand(const std::string& _var, MExpression* _expression);
    CalcCommand(const CalcCommand& rhs);
    virtual ~CalcCommand();

    virtual Command *clone() const;

    const std::string& getVar() const;
    const MExpression* getExpression() const;

  private:
    std::string var;
    MExpression* expression;
  };


  /**
   * Abstract superclass for commands that consist of a subprogram.
   */
  class SubProgramCommand : public Command {
  public:
    /**
     * @param _type Type of the command
     * @param _subProgram the actual subprogram
     */
    SubProgramCommand(CommandType _type, Program *_subProgram);
    SubProgramCommand(const SubProgramCommand& rhs);
    virtual ~SubProgramCommand();
    
    virtual Command *clone() const = 0;
    const Program* getSubProgram() const;
    
  private:
    Program *subProgram;
    
  };
  

  /**
   * Well-known foreach loop.
   */
  class ForeachCommand : public SubProgramCommand {
  public:
    /**
     * @param _var loop variable
     * @param _values list of values that is iterated
     * @param _commands the subprogram that is executed during each loop
     * iteration
     */
    ForeachCommand(std::string _var, std::list<int>* _values, Program* _commands);
    virtual ~ForeachCommand();
    ForeachCommand(const ForeachCommand& rhs);

    virtual Command *clone() const;

    const std::string& getVar() const;
    const std::list<int>* getValues() const;
    //const Program* getCommands() const;

    void printLong(std::ostream &os) const;

    // execution
    /**
     * initialise loop execution
     */
    void init();

    /**
     * @return current value of loop variable
     */
    int current();

    /**
     * Advance loop variable
     * @return old value of loop variable
     */
    int next();

    /**
     * Check whether iteration is finished.
     */
    bool isFinished() const;

    /**
     * Check whether there are still iterations to execute.
     */
    bool isRunning() const;

  private:
    std::string var;
    std::list<int> * values;
    //Program* commands;
    bool running;
    std::list<int>::iterator itExec;


  };


  /**
   * Waits for the same type of message from multiple nodes.
   */
  class ParwaitCommand : public SubProgramCommand {
  public:
    /**
     * @param _event Identifier of the message that the node waits for
     * @param _nodeIds a list of nodes that will send the specified message
     * @param _var Variable which holds the NodeId of a message sender
     * @param _commands subprogram to be executed for each message
     */
    ParwaitCommand(std::string _event, std::list<NodeId>* _nodeIds, std::string _var, Program* _commands);
    virtual ~ParwaitCommand();
    ParwaitCommand(const ParwaitCommand& rhs);
    
    virtual Command *clone() const;
    
    const std::string& getEvent() const;
    const std::string& getVar() const;
    const std::list<NodeId>* getNodeIds() const;
    
    // acknowledge receive
    void received(NodeId id);
    
    void printLong(std::ostream &os) const;
    
  private:
    std::string event;
    std::string var;
    std::list<NodeId>* nodeIds;
  };


  std::ostream & operator<<(std::ostream &os, const Command& cmd);
  
  std::ostream & operator<<(std::ostream &os, const ForeachCommand& cmd);
  std::ostream & operator<<(std::ostream &os, const ParwaitCommand& cmd);

} // NS

#endif /* !_REPR_COMMAND_H */
