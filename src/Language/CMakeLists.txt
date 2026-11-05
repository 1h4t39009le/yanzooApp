add_library(languageLib
    STATIC
        LanguageManager.hpp
        LanguageManager.cpp
)

target_include_directories(languageLib PUBLIC ${SRC_DIR})

target_link_libraries(languageLib PUBLIC Qt6::Core)
