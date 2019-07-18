
set(PROJECT TCPIP_CLIENT_DLL)


set(SRC_FOLDER TCPIP_CLIENT_DLL)

set(HEADER_FILES 
	${SRC_FOLDER}targetver.h
	${SRC_FOLDER}stdafx.h
	${SRC_FOLDER}CallbacksHolder.h
)

set(CPP_FILES
	${SRCFOLDER} ChatClientAPI.cpp
 	${SRCFOLDER} stdafx.cpp
 	${SRCFOLDER} dllmain.cpp
 	${SRCFOLDER} CallbacksHolder.cpp
	${SRCFOLDER} TCPIP_CLIENT_DLL.cpp
)

target_link_libraries (TCPIP_CLIENT_DLL LINK_PUBLIC ProtocolAPI)

add_library (${PROJECT} SHARED  ${CPP_FILES} ${HEADER_FILES})
target_include_directories (TCPIP_CLIENT_DLL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
