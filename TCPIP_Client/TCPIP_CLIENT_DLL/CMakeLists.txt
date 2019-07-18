
set(PROJECT TCPIP_CLIENT_DLL)
                                
set(HEADER_FILES 
	targetver.h
	stdafx.h
	CallbacksHolder.h
)

set(CPP_FILES
	ChatClientAPI.cpp
 	stdafx.cpp
 	dllmain.cpp
 	CallbacksHolder.cpp
	TCPIP_CLIENT_DLL.cpp
)
                                                                
add_library (${PROJECT} SHARED  ${CPP_FILES} ${HEADER_FILES})
target_include_directories (TCPIP_CLIENT_DLL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
                                                                
target_link_libraries (TCPIP_CLIENT_DLL LINK_PUBLIC ProtocolAPI)
