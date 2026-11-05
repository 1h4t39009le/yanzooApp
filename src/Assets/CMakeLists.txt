add_library(assetsLib INTERFACE)
target_sources(assetsLib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/app.qrc")

# ---------- Переводы ----------
set(I18N_TS_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/i18n/yanzoo_ru.ts"
)

# Qt6::lupdate / Qt6::lrelease — обычные импортированные исполняемые цели.
# Специально не используем qt_add_translations (Qt 6.2+), чтобы не привязывать
# сборку к версии Qt.
find_package(Qt6 QUIET COMPONENTS LinguistTools)

if(TARGET Qt6::lupdate AND TARGET Qt6::lrelease)
    # Ручное обновление .ts: cmake --build <build-dir> --target update_translations
    add_custom_target(update_translations
        COMMAND Qt6::lupdate
            -no-obsolete
            "${CMAKE_SOURCE_DIR}/src"
            -ts ${I18N_TS_FILES}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        COMMENT "Обновление .ts файлов переводов"
        VERBATIM
    )

    set(I18N_QM_FILES)
    foreach(ts_file IN LISTS I18N_TS_FILES)
        get_filename_component(qm_name "${ts_file}" NAME_WE)
        set(qm_file "${YANZOO_TRANSLATIONS_DIR}/${qm_name}.qm")

        add_custom_command(
            OUTPUT "${qm_file}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${YANZOO_TRANSLATIONS_DIR}"
            COMMAND Qt6::lrelease "${ts_file}" -qm "${qm_file}"
            DEPENDS "${ts_file}"
            COMMENT "Сборка ${qm_name}.qm"
            VERBATIM
        )

        list(APPEND I18N_QM_FILES "${qm_file}")
    endforeach()

    # Собирается вместе с проектом, чтобы .qm всегда были актуальны.
    add_custom_target(translations ALL DEPENDS ${I18N_QM_FILES})
else()
    message(STATUS
        "Qt LinguistTools не найден: цели update_translations/translations "
        "отключены. Добавьте qttools в vcpkg.json для работы с переводами.")
endif()
