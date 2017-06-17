
set(STI_SOURCES
    src/core/filter.hpp
    src/core/histogram.hpp
    src/core/image.hpp
)

source_group(core FILES ${STI_SOURCES})

set(STI_DETAIL_SOURCES
    src/core/detail/codec_bmp_detail.hpp
    src/core/detail/codec_bmp_detail.cpp
    src/core/detail/codec_jpeg_detail.hpp
    src/core/detail/codec_jpeg_detail.cpp
    src/core/detail/image_color_detail.hpp
    src/core/detail/image_detail.hpp
    src/core/detail/image_resize_detail.hpp
    src/core/detail/memory_file.hpp
    src/core/detail/utility_detail.hpp
)

source_group(core\\detail FILES ${STI_DETAIL_SOURCES})

set(STI_FILTER_SOURCES
    src/core/filter/filter.hpp
    src/core/filter/filter_lowpass.hpp
    src/core/filter/filter_mean_shift.hpp
    src/core/filter/filter_threshold_adaptive_bradley.hpp
)

source_group(core\\filter FILES ${STI_FILTER_SOURCES})

set(STI_KERNEL_SOURCES
    src/core/kernel/kernel.hpp
    src/core/kernel/kernel_lowpass.hpp
)

source_group(core\\kernel FILES ${STI_KERNEL_SOURCES})

add_library(sti STATIC
    ${STI_SOURCES}
    ${STI_DETAIL_SOURCES}
    ${STI_FILTER_SOURCES}
    ${STI_KERNEL_SOURCES}
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
