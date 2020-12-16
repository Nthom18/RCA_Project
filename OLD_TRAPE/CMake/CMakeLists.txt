cmake_minimum_required(VERSION 3.0.0)
project(TRAPE VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)

include_directories(../include)

add_executable(TRAPE ../scr/main.cpp)

target_link_libraries(TRAPE ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
