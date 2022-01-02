cmake_minimum_required(VERSION 3.0)

project(globalsettings VERSION 0.1.0 LANGUAGES CXX)

option(WITH_PYTHON "Create python binding" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(globalsettings SHARED
  src/globalsettings.h src/globalsettings.cpp
)
set_target_properties(globalsettings PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} SOVERSION 1)
set_target_properties(globalsettings PROPERTIES PUBLIC_HEADER "src/globalsettings.h")

install(TARGETS globalsettings
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
