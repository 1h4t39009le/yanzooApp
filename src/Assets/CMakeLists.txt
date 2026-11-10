add_library(assetsLib STATIC)

# ICONS 
set(ICON_FILES
    "icons/angle-right.svg"
    "icons/arrow-small-left.svg"
    "icons/elephant.svg"
    "icons/home.svg"
    "icons/paw.svg"
    "icons/settings-sliders.svg"
    "icons/sun.svg"
    "icons/user.svg"
)
foreach(file IN LISTS ICON_FILES)
    get_filename_component(name_we ${file} NAME_WE)
    set_source_files_properties(${file} PROPERTIES QT_RESOURCE_ALIAS ${name_we})
endforeach()
set_source_files_properties("icons/arrow-small-left.svg" PROPERTIES QT_RESOURCE_ALIAS "arrow-left")

qt_add_resources(assetsLib "assetsLib_icons"
    PREFIX "/icons"
    FILES ${ICON_FILES}
)

# THEMES
set(THEME_FILES
    "style/light.qss"
    "style/dark.qss"
)
foreach(file IN LISTS THEME_FILES)
    get_filename_component(name_we ${file} NAME_WE)
    set_source_files_properties(${file} PROPERTIES QT_RESOURCE_ALIAS ${name_we})
endforeach()

qt_add_resources(assetsLib "assetsLib_themes"
    PREFIX "/themes"
    FILES ${THEME_FILES}
)

# FONTS
set_source_files_properties("fonts/Fredoka-VariableFont.ttf" PROPERTIES QT_RESOURCE_ALIAS "fredoka")
set_source_files_properties("fonts/Roboto-Italic-VariableFont.ttf" PROPERTIES QT_RESOURCE_ALIAS "roboto-italic")
set_source_files_properties("fonts/Roboto-VariableFont.ttf" PROPERTIES QT_RESOURCE_ALIAS "roboto")

qt_add_resources(assetsLib "assetsLib_fonts"
    PREFIX "/fonts"
    FILES
        "fonts/Fredoka-VariableFont.ttf"
        "fonts/Roboto-Italic-VariableFont.ttf"
        "fonts/Roboto-VariableFont.ttf"
)

# TRANSLATIONS
# UPDATE TRANSLATIONS:
# 	cmake --build --preset=Debug --target update_translations
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
qt_add_translations(assetsLib
    TS_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/i18n/yanzoo_ru.ts"
    LUPDATE_OPTIONS
        -no-obsolete
    SOURCES
        "${SRC_DIR}"
)