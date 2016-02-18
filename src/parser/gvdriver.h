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
 * $Id: gvdriver.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file gvdriver.h
 * @brief Driver for parsing variable files
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _PARSER_GVDRIVER_H
#define _PARSER_GVDRIVER_H 1

#include <tools/variablestore.h>

namespace mwsim {

  class GVDriver {
  public:
    /** Default constructor */
    GVDriver(VariableStore &_vs);
    /** D'tor */
    ~GVDriver();

    void parse(const char *filename);

    void addVariable(const std::string& var, int value);
    int getVariable(const std::string& var) const;
    const VariableStore& getVariables() const;


  private:
    class Private;
    Private *d;
    VariableStore& vs;
  };


} // NS

#endif /* !_PARSER_GVDRIVER_H */
