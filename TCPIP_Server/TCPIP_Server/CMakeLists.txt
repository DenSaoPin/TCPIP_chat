set (PROJECT TCPIP_Server)

set (CPP_SOURCES
 	ClientSet.cpp
	ServerClient.cpp
	Server.cpp
	TCPIP_Server.cpp
	)	

set (H_SOURCES
	ServerClient.h
	ClientSet.h
	Server.h
	)

add_executable (${PROJECT}  ${CPP_SOURCES} ${H_SOURCES})

target_link_libraries (${PROJECT} LINK_PUBLIC ProtocolAPI)
target_link_libraries (${PROJECT} LINK_PUBLIC Logger)

add_dependencies(${PROJECT} ProtocolAPI)
add_dependencies(${PROJECT} Logger)

