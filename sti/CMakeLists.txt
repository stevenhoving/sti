
set(STI_CORE_SOURCES
    src/sti/image.hpp
    src/sti/plane.hpp
    src/sti/interleaved_image.hpp
    src/sti/interleaved_image_info.hpp
    src/sti/utility.hpp
    src/sti/image_detail.hpp
    src/sti/pixel_values.hpp
    src/sti/generator.hpp
    src/sti/atlas.hpp
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
    src/sti/convert/convert_image.hpp
)

source_group(convert FILES ${STI_CONVERT_SOURCES})

set(STI_FILTER_SOURCES
    src/sti/filter/filter.hpp
    src/sti/filter/filter_lowpass.hpp
    src/sti/filter/filter_mean_shift.hpp
    src/sti/filter/filter_threshold_adaptive_bradley.hpp
)

source_group(filter FILES ${STI_FILTER_SOURCES})

set(STI_KERNEL_SOURCES
    src/sti/kernel/kernel.hpp
    src/sti/kernel/kernel_lowpass.hpp
    src/sti/kernel/kernel_gaussian.hpp
    src/sti/kernel/kernel_edge.hpp
    src/sti/kernel/kernel_emboss.hpp
    src/sti/kernel/kernel_sharpen.hpp
    src/sti/kernel/kernel_sobel.hpp
)

source_group(kernel FILES ${STI_KERNEL_SOURCES})

set(STI_MATH_SOURCES
    src/sti/math/math_constants.hpp
)

source_group(math FILES ${STI_MATH_SOURCES})

add_library(sti STATIC
    ${STI_CORE_SOURCES}
    ${STI_CODECS_SOURCES}
    ${STI_CONVERT_SOURCES}
    ${STI_FILTER_SOURCES}
    ${STI_KERNEL_SOURCES}
    ${STI_MATH_SOURCES}
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
