cmake_minimum_required(VERSION 3.29)
project(Lab_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab3 menu/main_piecewise.cpp
        Lab_2/types.cpp
)

add_executable(SplineInterpolation spline_menu/main_spline.cpp
        Lab_2/types.cpp
)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(PiecewiseTests
        tests/testMain.cpp
        tests/testPiecewise.cpp
        tests/testsInterpolation.cpp
)

target_link_libraries(PiecewiseTests
        PRIVATE
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(PiecewiseTests)
