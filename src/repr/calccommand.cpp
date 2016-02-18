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
 * $Id: calccommand.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file calccommand.cpp
 * @brief Calculation of internal variables
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/command.h>

namespace mwsim {

  CalcCommand::CalcCommand(const std::string& _var, MExpression* _expression)
    : Command(CT_CALC), var(_var), expression(_expression)
  {
  }


  CalcCommand::CalcCommand(const CalcCommand& rhs)
    : Command(CT_CALC), var(rhs.var)
  {
    expression = new MExpression(*rhs.expression);
  }


  CalcCommand::~CalcCommand() {
    delete expression;
  }


  Command* CalcCommand::clone() const {
    return new CalcCommand(*this);
  }


  const std::string& CalcCommand::getVar() const {
    return var;
  }


  const MExpression* CalcCommand::getExpression() const {
    return expression;
  }


} // NS
