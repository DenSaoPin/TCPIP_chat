cmake_minimum_required (VERSION 3.8.2)
project(TCP_Client)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory (../ProtocolAPI ProtocolAPI)
add_subdirectory (TCPIP_CLIENT_DLL)          
add_subdirectory (WPF_UI WPF_UI)
add_subdirectory (Console Console)

