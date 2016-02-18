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
 * $Id: mexpression.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file mexpression.h
 * @brief Representation of mathemetical expressions
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _REPR_MEXPRESSION_H
#define _REPR_MEXPRESSION_H 1

#include <repr/varparm.h>
#include <tools/variablestore.h>

namespace mwsim {

  enum Operator {
    OP_PLUS = 0,
    OP_MINUS,
    OP_MUL,
    OP_DIV
  };

  class MExpression {

  public:
    MExpression(VarParm* lop, VarParm* rop, const Operator _op);
    MExpression(const MExpression& rhs);
    virtual ~MExpression();

    int evaluate(VariableStore *vs) const;

  private:
    VarParm *leftOp;
    VarParm *rightOp;
    Operator op;
  };

} // NS

#endif /* !_REPR_MEXPRESSION_H */
