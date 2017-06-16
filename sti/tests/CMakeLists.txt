include(Unittests)

add_unit_test_suite(
    NO_GTEST_MAIN
    TARGET test_sti
    SOURCES
        main.cpp
        test_filter.cpp
        test_histogram.cpp
        test_image.cpp
        test_png_codec.cpp
        stop_watch.h
        load_file_fixture.h
    INCLUDES
        ${CMAKE_BINARY_DIR}
    LIBRARIES sti
    FOLDER tests
)
