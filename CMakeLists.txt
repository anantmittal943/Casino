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

enable_testing()
add_executable(CasinoTests tests/RoundLogicTests.cpp)
target_link_libraries(CasinoTests PRIVATE casino_core)
add_test(NAME CasinoTests COMMAND CasinoTests)
