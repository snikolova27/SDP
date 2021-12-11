cmake_minimum_required(VERSION 3.0.0)
project(HW01 VERSION 0.1.0)

include(CTest)
enable_testing()

# Store logic
add_library(StoreLib implementation.cpp)

# Console exe
add_executable(ConsoleStore main.cpp)
target_link_libraries(ConsoleStore StoreLib)

# Tests exe
add_executable(StoreTests tests.cpp)
target_link_libraries(StoreTests StoreLib)

# All tests
add_executable(MyTests  moreTests.cpp)
target_link_libraries(MyTests StoreLib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
