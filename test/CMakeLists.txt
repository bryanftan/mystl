include(FetchContent)

FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(vector_test test_vector.cpp)

target_link_libraries(vector_test
        PRIVATE
        GTest::GTest
        mystl::lib)

add_test(vector_gtests vector_test)