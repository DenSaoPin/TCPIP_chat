set(PROJECT Console)

add_executable (${PROJECT} Console.cpp)

target_link_libraries (${PROJECT} LINK_PUBLIC TCPIP_CLIENT_DLL)

add_dependencies(${PROJECT} TCPIP_CLIENT_DLL)