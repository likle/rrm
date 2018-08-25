cmake_minimum_required(VERSION 3.9.2)

# set project name
project(rrm
  VERSION 0.1.0
  DESCRIPTION "A soft-delete replacement for rm"
  LANGUAGES C)

# include utilities
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(EnableWarnings)
include(CTest)
include(CreateTestList)

# configure requirements
set(CMAKE_C_STANDARD 11)

# setup target and directory names
set(TRASH_TARGET "rrm")
set(TEST_TARGET "rrmtest")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/test")

# generate constants
configure_file("${SOURCE_DIRECTORY}/constants.h.in" "${SOURCE_DIRECTORY}/constants.h")

# enable coverage if requested
if(ENABLE_COVERAGE)
  message("-- Coverage enabled")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

# add the main executable
add_executable(${TRASH_TARGET}
  "${SOURCE_DIRECTORY}/dump.c"
  "${SOURCE_DIRECTORY}/dump.h"
  "${SOURCE_DIRECTORY}/main.c"
  "${SOURCE_DIRECTORY}/option.c"
  "${SOURCE_DIRECTORY}/option.h"
  "${SOURCE_DIRECTORY}/status.c"
  "${SOURCE_DIRECTORY}/status.h"
  "${SOURCE_DIRECTORY}/trash.c"
  "${SOURCE_DIRECTORY}/trash.h"
  "${SOURCE_DIRECTORY}/utils.h")

# add tests
enable_testing()
create_test(DEFAULT option print)
create_test(DEFAULT option simple)
create_test(DEFAULT option simple_long)
create_test(DEFAULT option alias)
create_test(DEFAULT option unknown_short)
create_test(DEFAULT option unknown_long)
create_test(DEFAULT option combined)
create_test(DEFAULT option short_equal_value)
create_test(DEFAULT option long_equal_value)
create_test(DEFAULT option short_space_value)
create_test(DEFAULT option long_space_value)
create_test(DEFAULT option short_missing_value)
create_test(DEFAULT option long_missing_value)
create_test(DEFAULT option ending)
create_test(DEFAULT option mixed)
create_test(DEFAULT option complex)
create_test(DEFAULT trash open_and_create)
create_test(DEFAULT trash create_open)
create_test(DEFAULT trash create_fail_exists)
create_test(DEFAULT trash open_fail_no_entry)
create_test_list(DEFAULT "${TEST_DIRECTORY}/tests.h")

add_executable(${TEST_TARGET}
  "${TEST_DIRECTORY}/main.c"
  "${SOURCE_DIRECTORY}/status.c"
  "${SOURCE_DIRECTORY}/option.c"
  "${SOURCE_DIRECTORY}/trash.c"
  "${TEST_DIRECTORY}/option_test.c"
  "${TEST_DIRECTORY}/trash_test.c")
target_include_directories(${TEST_TARGET} PRIVATE "${SOURCE_DIRECTORY}")

# enable warnings
enable_warnings(${TRASH_TARGET})
