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
 * $Id: mexpression.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file mexpression.cpp
 * @brief
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <repr/mexpression.h>

#include <cstdlib>
#include <iostream>
using namespace std;

namespace mwsim {

  MExpression::MExpression(VarParm* lop, VarParm* rop, const Operator _op)
    : leftOp(lop), rightOp(rop), op(_op)
  {
  }


  MExpression::MExpression(const MExpression& rhs)
    : op(rhs.op)
  {
    leftOp = new VarParm(*rhs.leftOp);
    rightOp = new VarParm(*rhs.rightOp);
  }

  MExpression::~MExpression() {
    delete leftOp;
    delete rightOp;
  }


  int MExpression::evaluate(VariableStore *vs) const {
    int lop = 0;
    int rop = 0;
    switch (leftOp->getType()) {
    case VarParm::VP_NAME:
      //cout << "\trop name \"" << leftOp->getName() << "\"" << endl;
      lop = vs->getVar(leftOp->getName());
      break;
    case VarParm::VP_NUMBER:
      lop = leftOp->getNumber();
      break;
    default:
      cerr << "Unknown VP type " << leftOp->getType() << endl;
      abort();
      break;
    }

    switch (rightOp->getType()) {
    case VarParm::VP_NAME:
      //cout << "\trop name \"" << rightOp->getName() << "\"" << endl;
      rop = vs->getVar(rightOp->getName());
      break;
    case VarParm::VP_NUMBER:
      rop = rightOp->getNumber();
      break;
    default:
      cerr << "Unknown VP type " << rightOp->getType() << endl;
      abort();
      break;
    }

    //cout << "\tlop: " << lop << " rop: " << rop << endl;

    int result = 0;

    switch (op) {
    case OP_PLUS:
      result = lop + rop;
      break;
    case OP_MINUS:
      result = lop - rop;
      break;
    case OP_MUL:
      result = lop * rop;
      break;
    case OP_DIV:
    default:
      cerr << "Unsupported operation " << op << endl;
      abort();
      break;
    }

    return result;
  }


} // NS
