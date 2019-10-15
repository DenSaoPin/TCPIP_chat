set(PROJECT QT_UI)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_path(TCPIP_CLIENT_DLL_INCLUDE_DIR NAMES "ChatClientAPI.h" PATHS ${CMAKE_BINARY_DIR}/TCPIP_CLIENT_DLL/public/)

file(GLOB QT_HED
	"qt/*.h" 
)

file(GLOB QT_SRC
	"qt/*.cpp" 
)

file(GLOB QT_UI
	"qt/*.ui" 
)

file(GLOB SRC
	"*.cpp"
)

qt5_wrap_cpp(QT_HED_MOC ${QT_HED})
qt5_wrap_ui (QT_UI_MOC  ${QT_UI})

add_library (QT_DLL SHARED 
    ${QT_SRC}
    ${QT_HED_MOC}
    ${QT_UI_MOC}
)                                                                 

add_executable(${PROJECT} ${SRC})

target_link_libraries (QT_DLL Qt5::Core Qt5::Gui Qt5::Widgets) 
target_link_libraries (${PROJECT} QT_DLL)     

target_link_libraries (${PROJECT} TCPIP_CLIENT_DLL)

target_include_directories (${PROJECT} PUBLIC ${TCPIP_CLIENT_DLL_INCLUDE_DIR})

if(CMAKE_C_FLAGS_DEBUG)
    set(BUILD_TYPE "/Debug")
elseif(CMAKE_BUILD_TYPE=Release)
    set(BUILD_TYPE "/Release") 
else()
    set(BUILD_TYPE "/Test")
endif()

set(INPUT_LIB ${CMAKE_BINARY_DIR}/lib/)
set(OUTPUT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

add_custom_command(TARGET ${PROJECT} PRE_BUILD        		# Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E robocopy /E /MIR                   # which executes "cmake - E copy_if_different..."
        ${INPUT_LIB}                                        # <--this is in-file
        ${OUTPUT_LIB}
       )                                      # <--this is out-file path  

message(" Project binary dir - " ${PROJECT_BINARY_DIR})
message(" Cmake current binary dir - " ${CMAKE_CURRENT_BINARY_DIR})
message(" Cmake current source dir - " ${CMAKE_CURRENT_SOURCE_DIR})
message(" Cmake binary dir - " ${CMAKE_BINARY_DIR})
message(" Cmake source dir - " ${CMAKE_SOURCE_DIR})
message(" Lib input binary dir - " ${INPUT_LIB})
message(" Lib output binary dir - " ${OUTPUT_LIB}) 