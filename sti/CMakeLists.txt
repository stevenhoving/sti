
set(STI_CORE_SOURCES
    src/sti/image.hpp
    src/sti/color_image.hpp
    src/sti/color_image_info.hpp
)

source_group(core FILES ${STI_CORE_SOURCES})

set(STI_CODECS_SOURCES
    src/sti/codecs/codec_png.cpp
    src/sti/codecs/codec_png.h
    src/sti/codecs/detail/png_structs.hpp
    src/sti/codecs/detail/png_read_structs.hpp
    src/sti/codecs/detail/png_write_structs.hpp
)

source_group(codecs FILES ${STI_CODECS_SOURCES})

set(STI_CONVERT_SOURCES
    src/sti/convert/image.hpp
)

source_group(convert FILES ${STI_CONVERT_SOURCES})

add_library(sti STATIC
    ${STI_CORE_SOURCES}
    ${STI_CODECS_SOURCES}
    ${STI_CONVERT_SOURCES}
)

target_link_libraries(sti
    png_static
    aeon_common
    aeon_utility
    aeon_streams
)

target_include_directories(sti
    PUBLIC src
)

add_subdirectory(tests)
