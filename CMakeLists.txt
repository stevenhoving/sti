cmake_minimum_required(VERSION 3.2.0)
project(sti)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/dep/cmake")

include(CompilerFlags)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE TYPE INTERNAL FORCE)

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_CXX_STANDARD 14)

include(CTest)
enable_testing()

if (MSVC)
    option(STI_ENABLE_MSVC_ITERATOR_DEBUGGING "Enable iterator debugging in Visual Studio" ON)
    if (NOT STI_ENABLE_MSVC_ITERATOR_DEBUGGING)
        message (STATUS "Disabling iterator debugging (_HAS_ITERATOR_DEBUGGING=0).")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D _HAS_ITERATOR_DEBUGGING=0")
    endif ()

    option(STI_ENABLE_MSVC_MP_BUILD "Use the /MP flag in Visual Studio to use multiple processes per cpp file." ON)
    if (STI_ENABLE_MSVC_MP_BUILD)
        message (STATUS "Using Visual Studio /MP flag.")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    endif ()
endif ()

add_subdirectory(dep)
add_subdirectory(sti)

set(STI_TEST_DATA_PATH ${CMAKE_SOURCE_DIR}/data )
configure_file(build_config.h.in ${CMAKE_BINARY_DIR}/build_config.h @ONLY)
