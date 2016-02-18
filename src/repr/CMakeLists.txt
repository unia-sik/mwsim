# $Id: CMakeLists.txt 1160 2014-05-21 13:07:35Z klugeflo $

set(repr_SOURCES
	command.cpp
	execcommand.cpp
	loadcommand.cpp
	sendcommand.cpp
	recvcommand.cpp
	waitcommand.cpp
	calccommand.cpp
	subprogramcommand.cpp
	foreachcommand.cpp
	parwaitcommand.cpp
	nodedescription.cpp
	program.cpp
	varparm.cpp
	mexpression.cpp
	)

#add_library(repr SHARED ${repr_SOURCES})
add_library(repr ${repr_SOURCES})

#install(TARGETS repr DESTINATION ${LIBDIR})

