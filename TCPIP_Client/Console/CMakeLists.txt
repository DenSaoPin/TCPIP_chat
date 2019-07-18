add_executable (Console Console.cpp pch.cpp pch.h)

target_link_libraries (Console LINK_PUBLIC ../TCPIP_CLIENT_DLL)