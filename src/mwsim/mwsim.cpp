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
 * $Id: mwsim.cpp 1643 2016-02-18 07:11:10Z klugeflo $
 * MOSSCA WCET Simulator
 * @file mwsim.cpp
 * @brief main function
 * @author Florian Kluge <kluge@informatik.uni-augsburg.de>
 */

#include <cstring>
#include <iostream>
#include <list>
using namespace std;

#include <parser/mwsdriver.h>
#include <parser/sysdriver.h>
#include <parser/gvdriver.h>
#include <exec/node.h>
#include <exec/system.h>

#include <exec/simulation.h>

using namespace mwsim;

const char *STD_INPUT = "../script.mws";

static string scriptPath = "";
//static string sysPath = "";

static list<string> sysPaths;
static list<string> gvPaths;

static int parseArgs(int argc, char *argv[]);



int main(int argc, char *argv[]) {
  /*
  const char *file = NULL;
  if (argc < 2) {
    cout << "Using standard file " << STD_INPUT << endl;
    file = STD_INPUT;
  }
  else {
    file = argv[1];
  }
  */

  if (parseArgs(argc, argv) != 0) {
    cerr << "Parsing arguments failed!" << endl;
    return -1;
  }
  
  /*
  if (sysPath != "") {
    cout << "Using system description file " << sysPath << endl;
  }
  */
  if (scriptPath == "") {
    //cerr << "You need to specify a script file!" << endl;
    //return -1;
    scriptPath = STD_INPUT;
  }
 
  //cout << "Using script file " << scriptPath << endl;

  map<string, int> parameters;
  for (list<string>::iterator it = sysPaths.begin();
       it != sysPaths.end(); ++it) {
    cout << "Reading system description from " << *it << "... ";
    SysDriver sdrv(parameters);
    sdrv.parse((*it).c_str());
    cout << "done!" << endl;
  }
  
  /*
  cout << "System Parameters:" << endl;
  for (map<string, int>::iterator it = parameters.begin();
       it != parameters.end(); ++it) {
    cout << "(" << it->first << ", " << it->second << ")" << endl;
  }
  */

  VariableStore globalVariables;
  for (list<string>::iterator it = gvPaths.begin();
       it != gvPaths.end(); ++it) {
    cout << "Reading global variables from " << *it << "... ";
    GVDriver gdrv(globalVariables);
    gdrv.parse((*it).c_str());
    cout << "done!" << endl;
    
  }
  globalVariables.print();

  cout << "Reading script from " << scriptPath << "... ";
  MWSDriver mwsdrv(parameters, globalVariables);
  mwsdrv.parse(scriptPath.c_str());
  cout << "done!" << endl;

  const list<NodeDescription *> &lnd = mwsdrv.getNodeDescriptions();
  cout << "Found " << lnd.size() << " node descriptions" << endl;

  //driver.addVariable("N_NODES", driver.getSystemParameters()["size"]);
  globalVariables.putVar("N_NODES", parameters["size"]);
  /*
  cout << "System Parameters:" << endl;
  for (map<string, int>::iterator it = parameters.begin();
       it != parameters.end(); ++it) {
    cout << "(" << it->first << ", " << it->second << ")" << endl;
  }
  */

  cout << "Now running simulation..." << endl;
  Simulation sim(lnd, parameters, &globalVariables);
  sim.run();

  return 0;
}


#define CHECK_SWITCH_PARM(message)		\
  if ( p+1 >= argc ) {				\
    cerr << (message) << endl;			\
    return -1;					\
  }

static int parseArgs(int argc, char *argv[]) {
  int p = 1; // argv[0] == command
  while (p < argc) {
    //cout << "checking argv[" << p << "]: " << argv[p] << endl;
    if (argv[p][0] == '-') {
      if (argv[p][1] == 's') {
	//cout << "-s detected" << endl;
	CHECK_SWITCH_PARM("Switch -s requires a path argument!");
	sysPaths.push_back(argv[p+1]);
	++p;
      }
      else if (argv[p][1] == 'g') {
	CHECK_SWITCH_PARM("Switch -g requires a path argument!");
	gvPaths.push_back(argv[p+1]);
	++p;
      }
    }
    else { // script file is given without a switch
      scriptPath = argv[p];
    }
    ++p;
  }
  return 0;
}
