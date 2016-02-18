# $Id: CMakeLists.txt 1170 2014-05-30 13:36:44Z klugeflo $

bison_target(mwsparser mwsparser.yy ${CMAKE_CURRENT_BINARY_DIR}/mwsparser.cpp HEADER ${CMAKE_CURRENT_BINARY_DIR}/mwsparser.h)
flex_target(mwslexer mwslexer.ll ${CMAKE_CURRENT_BINARY_DIR}/mwslexer.cc)

bison_target(sysparser sysparser.yy ${CMAKE_CURRENT_BINARY_DIR}/sysparser.cpp HEADER ${CMAKE_CURRENT_BINARY_DIR}/sysparser.h)
flex_target(syslexer syslexer.ll ${CMAKE_CURRENT_BINARY_DIR}/syslexer.cc)

bison_target(gvparser gvparser.yy ${CMAKE_CURRENT_BINARY_DIR}/gvparser.cpp HEADER ${CMAKE_CURRENT_BINARY_DIR}/gvparser.h)
flex_target(gvlexer gvlexer.ll ${CMAKE_CURRENT_BINARY_DIR}/gvlexer.cc)

set(parser_SOURCES
	mwsdriver.cpp
	mwsscanner.cpp
	${BISON_mwsparser_OUTPUTS}
	${FLEX_mwslexer_OUTPUTS}
	sysdriver.cpp
	sysscanner.cpp
	${BISON_sysparser_OUTPUTS}
	${FLEX_syslexer_OUTPUTS}
	gvdriver.cpp
	gvscanner.cpp
	${BISON_gvparser_OUTPUTS}
	${FLEX_gvlexer_OUTPUTS}
	)

#add_library(parser SHARED ${parser_SOURCES})
add_library(parser ${parser_SOURCES})

#install(TARGETS parser DESTINATION ${LIBDIR})

