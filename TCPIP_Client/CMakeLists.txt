	cmake_minimum_required (VERSION 3.8.2)

	set(CMAKE_CXX_STANDARD 14)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)

	project(TCP_Client)

	set(CMAKE_C_FLAGS "-pthread")
	set(CMAKE_CXX_FLAGS "-pthread")


	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

	add_subdirectory (../ProtocolAPI ProtocolAPI)
	add_subdirectory (TCPIP_CLIENT_DLL)
	add_subdirectory (Console Console)
	add_subdirectory(QT_UI)
if(WIN32)
    	add_subdirectory (WPF_UI WPF_UI)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT WPF_UI)
endif(WIN32)

if (UNIX)
 	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Console)
endif (UNIX)

