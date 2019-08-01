set (PROJECT Logger)
set (API_FOLDER	loggerAPI)

set (CPP_SOURCES
	BaseLogger.cpp
 	ConsoleLogger.cpp
 	dllmain.cpp
 	ILogger.cpp
 	LoggerManager.cpp
	)  

set (H_SOURCES
	BaseLogger.h
 	LoggerManager.h
 	ConsoleLogger.h
 	${API_FOLDER}/LoggerManager.h
 	${API_FOLDER}/ILogger.h
	)

add_library(${PROJECT} SHARED ${CPP_SOURCES} ${H_SOURCES})
target_include_directories (${PROJECT} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
