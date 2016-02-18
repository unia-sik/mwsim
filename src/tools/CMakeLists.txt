# $Id: CMakeLists.txt 1166 2014-05-28 13:29:13Z klugeflo $

set(tools_SOURCES
	variablestore.cpp
	)

#add_library(tools SHARED ${tools_SOURCES})
add_library(tools ${tools_SOURCES})

#install(TARGETS tools DESTINATION ${LIBDIR})

