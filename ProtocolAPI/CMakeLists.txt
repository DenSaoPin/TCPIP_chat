
set(PROJECT ProtocolAPI)


set(SRC_FOLDER ProtocolAPI)

set(HEADER_FILES 
	${SRC_FOLDER}/Enums.h
	${SRC_FOLDER}/Message.h
	${SRC_FOLDER}/Protocol.h
 	${SRC_FOLDER}/DirectMessage.h
 	${SRC_FOLDER}/Defines.h
 	${SRC_FOLDER}/Protocol.h
 	${SRC_FOLDER}/targetver.h
 	${SRC_FOLDER}/Enums.h
 	${SRC_FOLDER}/stdafx.h
 	${SRC_FOLDER}/BroadcastMessage.h
 	${SRC_FOLDER}/Response.h
 	${SRC_FOLDER}/BaseMessage.h
 	${SRC_FOLDER}/NameRequestMessage.h
)

set(CPP_FILES
	${SRCFOLDER}/BaseMessage.cpp
	${SRCFOLDER}/BroadcastMessage.cpp
	${SRCFOLDER}/Response.cpp
	${SRCFOLDER}/Protocol.cpp
	${SRCFOLDER}/DirectMessage.cpp
	${SRCFOLDER}/stdafx.cpp
	${SRCFOLDER}/NameRequestMessage.cpp
)


add_library (${PROJECT} STATIC  ${CPP_FILES} ${HEADER_FILES})
target_include_directories (ProtocolAPI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
