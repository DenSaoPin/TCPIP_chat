set(PROJECT QT_UI)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

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
	"main.cpp"
)

qt5_wrap_cpp(QT_HED_MOC ${QT_HED})
qt5_wrap_ui (QT_UI_MOC  ${QT_UI})                                                           

add_executable(${PROJECT} ${SRC} ${QT_SRC} ${QT_HED} ${QT_HED_MOC} ${QT_UI_MOC})

source_group("HEADERS" FILES ${QT_HED})
source_group("SORCES" FILES ${SRC} ${QT_SRC})
source_group("UI" FILES ${QT_UI})

target_include_directories (${PROJECT} 
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR} 
    #PUBLIC ${TCPIP_CLIENT_DLL_INCLUDE_DIR}
)

target_link_libraries (${PROJECT} TCPIP_CLIENT_DLL Qt5::Core Qt5::Gui Qt5::Widgets)

add_dependencies(${PROJECT} TCPIP_CLIENT_DLL)

