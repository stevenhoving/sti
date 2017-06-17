include(Unittests)

add_unit_test_suite(
    NO_GTEST_MAIN
    TARGET test_sti
    SOURCES
        main.cpp
        test_filter.cpp
        test_histogram.cpp
        test_image.cpp
        stop_watch.h
    INCLUDES
        ${CMAKE_CURRENT_BINARY_DIR}
    LIBRARIES sti
    FOLDER tests
)
