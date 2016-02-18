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
 * $Id: sysdriver.h 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file sysdriver.h
 * @brief Driver for parsing system description file
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#ifndef _PARSER_SYSDRIVER_H
#define _PARSER_SYSDRIVER_H 1

#include <list>
#include <map>
#include <string>


namespace mwsim {

  class SysDriver {
  public:
    /** Default constructor */
    SysDriver(std::map<std::string, int>& _parameters);
    /** D'tor */
    ~SysDriver();

    void parse(const char *filename);
    
    void setParameter(const std::string& key, int value);
    int getParameter(const std::string& key);
    const std::map<std::string, int>& getSystemParameters() const;


  private:
    class Private;
    Private *d;
    std::map<std::string, int>& parameters;
  };


} // NS

#endif /* !_PARSER_SYSDRIVER_H */
