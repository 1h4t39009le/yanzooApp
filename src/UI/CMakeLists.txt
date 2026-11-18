set(LIBRARY_NAME uiLib)
add_library(${LIBRARY_NAME} STATIC)
target_include_directories(uiLib PRIVATE ${SRC_DIR} ${SRC_DIR}/UI)

add_subdirectory(Widgets)
add_subdirectory(Roles)
add_subdirectory(Main)

target_link_libraries(uiLib PRIVATE
    Qt6::Widgets
	coreUserSessionLib
	themeLib
	languageLib
)
