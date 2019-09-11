
set(PROJECT ProtocolAPI)

set(SRC_FOLDER ProtocolAPI)

set(HEADER_FILES 
	${SRC_FOLDER}/DirectMessage.h
 	${SRC_FOLDER}/Defines.h
	${SRC_FOLDER}/Enums.h
	${SRC_FOLDER}/BroadcastMessage.h
	${SRC_FOLDER}/Response.h
	${SRC_FOLDER}/BaseMessage.h
	${SRC_FOLDER}/NameRequestMessage.h
)

set(CPP_FILES
	${SRC_FOLDER}/BaseMessage.cpp
	${SRC_FOLDER}/BroadcastMessage.cpp
	${SRC_FOLDER}/Response.cpp
	${SRC_FOLDER}/DirectMessage.cpp
	${SRC_FOLDER}/NameRequestMessage.cpp
)


add_library (${PROJECT} STATIC  ${CPP_FILES} ${HEADER_FILES})
set_property(TARGET ${PROJECT} PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories (ProtocolAPI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
