cmake_minimum_required(VERSION 3.0)

project(GlobalSettings VERSION 0.1.0)

option(WITH_PYTHON "Create python binding" OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library(GlobalSettings SHARED
  src/globalsettings.h src/globalsettings.cpp
)
set_target_properties(GlobalSettings PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} SOVERSION 1)
set_target_properties(GlobalSettings PROPERTIES PUBLIC_HEADER "src/globalsettings.h")

install(TARGETS GlobalSettings
    EXPORT "GlobalSettingsTargets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(WITH_PYTHON)
    message(STATUS "Build with python binding")
    find_package(PythonLibs REQUIRED)
    if ("${PYTHONPATH}" STREQUAL "")
        string(REGEX MATCH "libpython(.*).so" PYTHON_LIBRARY_FILENAME ${PYTHON_LIBRARY})
        string(REGEX REPLACE "(lib)(python.*)(.so)" "\\2" PYCODE ${PYTHON_LIBRARY_FILENAME})
        set(PYTHONPATH /usr/lib/${PYCODE}/site-packages)
    endif()
    install(FILES python/__init__.py DESTINATION ${PYTHONPATH}/globalsettings/)
endif(WITH_PYTHON)


# CMake Module
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/generated/GlobalSettingsConfigVersion.cmake" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "cmake/Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/generated/GlobalSettingsConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/GlobalSettingsConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/generated/GlobalSettingsConfigVersion.cmake"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)

install(
    EXPORT "GlobalSettingsTargets"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)
