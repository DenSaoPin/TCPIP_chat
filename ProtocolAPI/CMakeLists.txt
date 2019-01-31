
set(PROJECT ProtocolAPI)


set(SRC_FOLDER ProtocolAPI)

set(HEADER_FILES 
	${SRC_FOLDER}/Enums.h
	${SRC_FOLDER}/Message.h
	${SRC_FOLDER}/Protocol.h
	)


add_library (${PROJECT}  ${SRC_FOLDER}/Protocol.cpp ${HEADER_FILES})
target_include_directories (ProtocolAPI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
