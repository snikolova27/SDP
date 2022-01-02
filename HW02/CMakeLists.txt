cmake_minimum_required(VERSION 3.0.0)
project(Homework02 VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(OfficeLib  src/implementation.cpp src/team.cpp)
target_include_directories(OfficeLib PUBLIC include/)

add_executable(Homework02 main.cpp)
target_link_libraries   (Homework02 OfficeLib)

add_executable(Tests tests.cpp)
target_link_libraries(Tests OfficeLib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
