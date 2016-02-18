# $Id: CMakeLists.txt 1166 2014-05-28 13:29:13Z klugeflo $

set(mwsim_SOURCES
	"mwsim.cpp"
	)

add_executable(mwsim ${mwsim_SOURCES})

target_link_libraries(mwsim parser repr exec tools)

install(TARGETS mwsim DESTINATION ${BINDIR})

