cmake_minimum_required(VERSION 3.0.0)
project(ListFunc VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(Functional source/exceptions.cpp source/utilities.cpp source/commandTokens.cpp 
            source/commandReader.cpp source/expressionElements.cpp source/interpreter.cpp source/analyzer.cpp
            source/list.cpp)
target_include_directories(Functional PUBLIC include/)

add_executable(ListFunc main.cpp)
target_link_libraries(ListFunc Functional)

# add_executable(Tests tests/interpreterTests.cpp)
# target_link_libraries(Tests Functional)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)