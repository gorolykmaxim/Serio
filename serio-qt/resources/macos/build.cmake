function(BuildSerio resources)
    add_executable(Serio MACOSX_BUNDLE ${resources} src/main.cpp resources/macos/serio.icns)
    set_source_files_properties(resources/macos/serio.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(Serio PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/macos/Info.plist)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        BuildSerioRelease()
    endif()
endfunction()

function(BuildSerioRelease)
    find_program(DEPLOY_QT_EXECUTABLE macdeployqt)
    set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
    set(BUNDLE_DIR ${OUTPUT_DIR}.app)
    add_custom_command(TARGET Serio POST_BUILD
            COMMAND "${DEPLOY_QT_EXECUTABLE}" ${BUNDLE_DIR} -qmldir=${PROJECT_SOURCE_DIR} -dmg
            BYPRODUCTS ${BUNDLE_DIR} ${OUTPUT_DIR}.dmg
            COMMENT "Running macdeployqt...")
endfunction()