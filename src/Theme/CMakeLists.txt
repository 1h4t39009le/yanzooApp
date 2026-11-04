add_library(themeLib
STATIC
	ThemeManager.hpp
	ThemeManager.cpp
)

target_include_directories(themeLib PUBLIC ${SRC_DIR})

# ThemeManager.cpp использует QApplication/qApp, т.е. Qt6::Widgets
target_link_libraries(themeLib PUBLIC Qt6::Widgets)
