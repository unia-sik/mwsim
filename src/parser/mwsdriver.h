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
 * $Id: mwsdriver.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file mwsdriver.h
 * @brief Driver for parsing script file
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _PARSER_MWSDRIVER_H
#define _PARSER_MWSDRIVER_H 1

#include <repr/nodedescription.h>
#include <tools/variablestore.h>

#include <list>
#include <map>
#include <string>

namespace mwsim {


  class MWSDriver {
  public:
    MWSDriver(std::map<std::string, int>& _parameters, VariableStore& _variables);
    /** D'tor */
    ~MWSDriver();

    void parse(const char *filename);
    
    void addNodeDescription(NodeDescription *nodeDescription);
    //void setSystemSize(int size);
    //int getSystemSize() const;
    const std::list<NodeDescription *> getNodeDescriptions() const;

    void setParameter(const std::string& key, int value);
    int getParameter(const std::string& key);
    std::map<std::string, int> getSystemParameters() const;

    void addVariable(const std::string &var, int value);
    const VariableStore * getVariables() const;
    

  private:
    class Private;
    Private *d;
    std::list<NodeDescription *> lNodeDescriptions;
    //int systemSize;
    std::map<std::string, int>& parameters;
    
    VariableStore& variables;
  };

}

#endif /* !_PARSER_MWSDRIVER_H */
