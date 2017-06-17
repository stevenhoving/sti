
set(STI_CORE_SOURCES
    src/sti/dummy.cpp # To make cmake build this as a lib...
    src/sti/image.hpp
    src/sti/color_image.hpp
    src/sti/color_image_info.hpp
)

source_group(core FILES ${STI_CORE_SOURCES})

add_library(sti STATIC
    ${STI_CORE_SOURCES}
)

target_link_libraries(sti
    aeon_common
    aeon_utility
    aeon_streams
)

target_include_directories(sti
    PUBLIC src
)

add_subdirectory(tests)
