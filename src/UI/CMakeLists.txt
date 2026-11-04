add_library(uiLib STATIC
    MainWindow.hpp
    MainWindow.cpp
    MainWindow.ui
)
target_include_directories(uiLib PRIVATE ${SRC_DIR})
target_link_libraries(uiLib PRIVATE
    Qt6::Widgets
	themeLib
)
