function(BuildSerio resources)
    add_executable(Serio WIN32 ${resources} src/main.cpp)
    set(WINDOWS_DEPENDENCIES_ZIP ${CMAKE_CURRENT_SOURCE_DIR}/resources/windows/dependencies.zip)
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        BuildSerioRelease(${WINDOWS_DEPENDENCIES_ZIP})
    else()
        BuildSerioDebug(${WINDOWS_DEPENDENCIES_ZIP})
    endif()
endfunction()

function(BuildSerioRelease windowsDependenciesZip)
    set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/output")
    find_program(DEPLOY_QT_EXECUTABLE windeployqt)
    set(OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/output")
    add_custom_command(TARGET Serio PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR})
    add_custom_command(TARGET Serio POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Serio> ${OUTPUT_DIR}
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${windowsDependenciesZip}
            COMMAND "${DEPLOY_QT_EXECUTABLE}" ${OUTPUT_DIR} -qmldir=${PROJECT_SOURCE_DIR}
            BYPRODUCTS ${OUTPUT_DIR}
            WORKING_DIRECTORY ${OUTPUT_DIR}
            COMMENT "Running windeployqt...")
    if (DEFINED ENV{NSIS_PATH})
        BuildNSISInstaller(${OUTPUT_DIR})
    else()
        SkipNSISInstallerBuild()
    endif ()
endfunction()

function(BuildNSISInstaller outputDir)
    find_program(NSIS_EXECUTABLE makensis PATHS $ENV{NSIS_PATH} REQUIRED)
    add_custom_command(TARGET Serio POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E env
            NSIS_INPUT_DIR=${outputDir}
            NSIS_OUTPUT_DIR=${outputDir}
            "${NSIS_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/resources/windows/installer.nsi"
            COMMENT "Compiling an installer...")
endfunction()

function(SkipNSISInstallerBuild)
    add_custom_command(TARGET Serio POST_BUILD COMMENT "NSIS executable was not found - skipping installer compilation.")
endfunction()

function(BuildSerioDebug windowsDependenciesZip)
    add_custom_command(TARGET Serio POST_BUILD COMMAND ${CMAKE_COMMAND} -E tar xzf ${windowsDependenciesZip})
endfunction()