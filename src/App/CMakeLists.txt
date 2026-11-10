add_executable(${PROJECT_NAME}
    main.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})
target_link_libraries(
    ${PROJECT_NAME}
PRIVATE
    Qt6::Widgets
	coreUserSessionLib
    uiLib
    themeLib
    languageLib
    assetsLib
    Qt6::Svg
)

#auto deploy
if(WIN32)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND Qt6::windeployqt
			$<IF:$<CONFIG:Debug>,--debug,--release>
			--no-translations
			--no-compiler-runtime
			--dir "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
			"$<TARGET_FILE:${PROJECT_NAME}>"
		VERBATIM
	)
endif()

#cmake --build --preset=Debug --target=run
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_target(run
        COMMAND $<TARGET_FILE:${PROJECT_NAME}>
        DEPENDS ${PROJECT_NAME}
        COMMENT "Running Debug binary..."
    )
endif()