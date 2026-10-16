cmake_minimum_required(VERSION 4.1)
project(Casino)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(casino_core
        src/Player.cpp
        src/RoundLogic.cpp
        src/Statistics.cpp)
target_include_directories(casino_core PUBLIC include)

add_executable(Casino src/main.cpp)
target_link_libraries(Casino PRIVATE casino_core)

include(CTest)

if (BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(
            casino_tests
            tests/PlayerTests.cpp
            tests/BettingTests.cpp
            tests/StatisticsTests.cpp
    )
    target_link_libraries(casino_tests PRIVATE casino_core GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(casino_tests)
endif ()
