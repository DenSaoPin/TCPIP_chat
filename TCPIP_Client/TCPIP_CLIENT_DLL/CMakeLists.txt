
set(PROJECT TCPIP_CLIENT_DLL)
                                
set(HEADER_FILES
	public/ChatClientAPI.h 
	CallbacksHolder.h
 	TCPIP_CLIENT_DLL.h
 	UIInterface.h
 	targetver.h
 	stdafx.h
)

set(CPP_FILES
	ChatClientAPI.cpp
 	UIInterface.cpp
 	dllmain.cpp
 	TCPIP_CLIENT_DLL.cpp
 	stdafx.cpp
 	CallbacksHolder.cpp
)
                                                                
add_library (${PROJECT} SHARED  ${CPP_FILES} ${HEADER_FILES})
target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
                                                                
target_link_libraries (${PROJECT} LINK_PUBLIC ProtocolAPI)

add_dependencies(${PROJECT} ProtocolAPI)