cmake_minimum_required(VERSION 3.5)

project(matching_tool LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_FLAGS "-std=c++20 -g -Wall -Wextra")

file(GLOB_RECURSE curr_SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${curr_SOURCES})
FOREACH(subdir ${LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${subdir})
ENDFOREACH()   