cmake_minimum_required(VERSION 3.0.0)
project(Homework03 VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(Words src/implementation.cpp)
target_include_directories(Words PUBLIC include/)

add_executable(Homework03 main.cpp)
target_link_libraries(Homework03 Words)

add_executable(Tests tests/tests.cpp)
target_link_libraries(Tests Words)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
