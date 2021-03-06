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
 * $Id: varparm.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file varparm.h
 * @brief Parameter for send/recv commands
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _REPR_VARPARM_H
#define _REPR_VARPARM_H 1

#include <string>
#include <tools/variablestore.h>

namespace mwsim {

  class VarParm {

  public:

    enum Type {
      VP_NAME = 0,
      VP_NUMBER
    };

    VarParm(const std::string& _name);
    VarParm(int _number);
    VarParm(const VarParm& rhs);
    
    Type getType() const { return type; }
    const std::string& getName() const;
    int getNumber() const;

    int resolve(const VariableStore* vs) const;

    class InvalidTypeException {
    public:
      InvalidTypeException(Type _type) : xtype(_type) { }
      Type xtype;
    };

  private:
    Type type;
    std::string name;
    int number;

  };

} // NS

#endif /* !_REPR_VARPARM_H */
