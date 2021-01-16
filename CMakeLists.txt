cmake_minimum_required(VERSION 3.5)

message(${CMAKE_CURRENT_LIST_FILE})

project(novavision LANGUAGES CXX VERSION 0.0.1)
set(CMAKE_CXX_FLAGS "-std=c++20 -g")

add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wcast-align
    -Wcast-qual
    #-Wconversion
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-conversion
    -Wsign-promo
)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

option(MY_TESTING "Включить модульное тестирование" ON)
option(MY_COVERAGE "Включить измерение покрытия кода тестами" OFF)
option(MY_RUN_TESTS "run tests after building." ON)

find_package(OpenCV REQUIRED)

#............... libraries block ................

include_directories("${PROJECT_SOURCE_DIR}/libraries")

SUBDIRLIST(LIBRARIES "${PROJECT_SOURCE_DIR}/libraries")

FOREACH(subdir ${LIBRARIES})
    message(${subdir})
    file(GLOB_RECURSE ${subdir}_LIB_SOURCES "libraries/${subdir}/*.cpp")
    file(GLOB_RECURSE ${subdir}_TESTS "libraries/${subdir}/*.cxx")
    list (APPEND TEST_SRC ${${subdir}_TESTS})
    add_library(${subdir} SHARED ${${subdir}_LIB_SOURCES})
    target_link_libraries(${subdir} ${OpenCV_LIBS})
    install (TARGETS ${subdir} DESTINATION lib)
ENDFOREACH()

#............... samples block ................

SUBDIRLIST(SAMPLES "${PROJECT_SOURCE_DIR}/samples")

FOREACH(subdir ${SAMPLES})
    message(${subdir})
    add_subdirectory(samples/${subdir})
ENDFOREACH()

#............... unit tests block ................

if(NOT MY_TESTING)
    message(STATUS "Unit-tests off.")
else()
  message(STATUS "Unit-tests on.")
  file(GLOB_RECURSE test_SOURCES "test/*.cpp")

  find_package(Catch2 REQUIRED)

  add_executable(run_unit_tests ${test_SOURCES})
  target_sources(run_unit_tests PRIVATE ${TEST_SRC})
  target_link_libraries(run_unit_tests Catch2::Catch2 ${OpenCV_LIBS})

  FOREACH(subdir ${LIBRARIES})
      target_link_libraries(run_unit_tests ${subdir})
  ENDFOREACH()

  include(CTest)
  include(Catch)
  catch_discover_tests(run_unit_tests)
  if(MY_RUN_TESTS)
    add_custom_target(check ALL COMMAND run_unit_tests)
  endif()
endif()

#............... code coverage block ................
