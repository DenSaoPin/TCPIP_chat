
set(PROJECT TCPIP_CLIENT_DLL)
                                
set(HEADER_FILES
	public/ChatClientAPI.h 
	CallbacksHolder.h
 	TCPIP_CLIENT_DLL.h
 	UIInterface.h
	Defines.h
	Exceptions.h
)

set(CPP_FILES
	ChatClientAPI.cpp
 	UIInterface.cpp
	TCPIP_CLIENT_DLL.cpp
 	CallbacksHolder.cpp
)

if(WIN32)
               set(CPP_FILES   ${CPP_FILES}  dllmain.cpp)
endif(WIN32)                                          

add_library (${PROJECT} SHARED  ${CPP_FILES} ${HEADER_FILES})

target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
                                                                
target_link_libraries (${PROJECT} LINK_PUBLIC ProtocolAPI)

add_dependencies(${PROJECT} ProtocolAPI)

source_group("HEADERS" FILES ${HEADER_FILES})
source_group("SOURCES" FILES ${CPP_FILES})

include(GNUInstallDirs)

INSTALL(TARGETS ${PROJECT} 
	ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 

)
install(FILES "public/ChatClientAPI.h" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT}")
