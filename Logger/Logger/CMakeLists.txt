set (PROJECT Logger)
set (API_FOLDER	loggerAPI)

set (CPP_SOURCES
	BaseLogger.cpp
 	ConsoleLogger.cpp
 	ILogger.cpp
 	LoggerManager.cpp
	)  

if(WIN32)
               set(CPP_SOURCES   ${CPP_SOURCES}  dllmain.cpp)
endif(WIN32)


set (H_SOURCES
	BaseLogger.h
 	LoggerManager.h
 	ConsoleLogger.h
 	${API_FOLDER}/LoggerManager.h
 	${API_FOLDER}/ILogger.h
	)

add_library(${PROJECT} SHARED ${CPP_SOURCES} ${H_SOURCES})
target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
