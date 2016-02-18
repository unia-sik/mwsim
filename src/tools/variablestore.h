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
 * $Id: variablestore.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file variablestore.h
 * @brief Storage for program variable
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _TOOLS_VARIABLESTORE_H
#define _TOOLS_VARIABLESTORE_H 1

#include <map>
#include <string>


namespace mwsim { 
  class VariableStore {
  public:
    VariableStore(const VariableStore *_parent = NULL);
    //~VariableStore();

    int getVar(std::string var) const;
    void putVar(std::string var, int value);

    void print() const;

    class VariableNotFoundException {
    public:
    VariableNotFoundException(std::string _var) : var(_var) {}
      std::string var;
    };
    
  private:
    const VariableStore *parent;
    std::map<std::string, int> data;
    
    
  };
} // NS

#endif /* !_TOOLS_VARIABLESTORE_H */
