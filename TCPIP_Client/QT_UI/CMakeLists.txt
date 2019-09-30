set(PROJECT QT_UI)

find_package (Qt5Widgets)

set(LIB_HED
	mainwindow.h 
)

set(LIB_CPP
	mainwindow.cpp
)

set(LIB_UI
	mainwindow.ui
)

set(BIN
	main.cpp
)

qt5_wrap_cpp(LIB_HED_MOC ${LIB_HED})
qt5_wrap_ui (LIB_UI_MOC  ${LIB_UI})

add_library (QT_DLL SHARED 
    ${LIB_CPP}
    ${LIB_HED_MOC}
    ${LIB_UI_MOC}
)

target_link_libraries (QT_DLL Qt5::Widgets)

add_executable(${PROJECT} ${BIN})
target_link_libraries (${PROJECT} QT_DLL)