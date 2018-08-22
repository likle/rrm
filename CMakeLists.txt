cmake_minimum_required(VERSION 3.10)

# set project name
project(tsh
  VERSION 0.0.1
  DESCRIPTION "A soft-delete replacement for rm"
  LANGUAGES C)

# include utilities
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(EnableWarnings)
include(CTest)

# configure requirements
set(CMAKE_C_STANDARD 11)

# setup target and directory names
set(TRASH_TARGET "tsh")
set(TEST_TARGET "tshtest")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

# generate constants
configure_file("${SOURCE_DIRECTORY}/constants.h.in" "${SOURCE_DIRECTORY}/constants.h")

# enable coverage if requested
if(ENABLE_COVERAGE)
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g ")
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -O0")
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
  set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# add the main executable
add_executable(${TRASH_TARGET}
  "${SOURCE_DIRECTORY}/main.c"
  "${SOURCE_DIRECTORY}/option.h"
  "${SOURCE_DIRECTORY}/option.c"
  "${SOURCE_DIRECTORY}/utils.h")

# add tests
enable_testing()
add_executable(${TEST_TARGET} "${TEST_DIRECTORY}/main.c" "${TEST_DIRECTORY}/option_test.c")
target_include_directories(${TEST_TARGET} PRIVATE "${SOURCE_DIRECTORY}")

# enable warnings
enable_warnings(${TRASH_TARGET})
