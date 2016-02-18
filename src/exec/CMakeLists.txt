# $Id: CMakeLists.txt 1200 2014-06-23 16:14:07Z klugeflo $

set(exec_SOURCES
	context.cpp
	node.cpp
	simulation.cpp
	system.cpp
	txbuffer.cpp
#	variablestore.cpp
	)

#add_library(exec SHARED ${exec_SOURCES})
add_library(exec ${exec_SOURCES})

#install(TARGETS exec DESTINATION ${LIBDIR})

