cmake_minimum_required(VERSION 3.10)

# set project name
project(tsh
  VERSION 0.0.1
  DESCRIPTION "A soft-delete replacement for rm"
  LANGUAGES C)

# include utilities
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(EnableWarnings)

# configure requirements
set(CMAKE_C_STANDARD 11)

# setup target and directory names
set(TRASH_TARGET "tsh")
set(SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src")

# generate constants
configure_file("${SOURCE_DIRECTORY}/constants.h.in" "${SOURCE_DIRECTORY}/constants.h")

# add the main executable
add_executable(${TRASH_TARGET}
  #"${SOURCE_DIRECTORY}/dir.h"
  #"${SOURCE_DIRECTORY}/dir.c"
  #"${SOURCE_DIRECTORY}/file.h"
  #"${SOURCE_DIRECTORY}/file.c"
  "${SOURCE_DIRECTORY}/main.c"
  "${SOURCE_DIRECTORY}/option.h"
  "${SOURCE_DIRECTORY}/option.c"
  #"${SOURCE_DIRECTORY}/path.h"
  #"${SOURCE_DIRECTORY}/path.c"
  #"${SOURCE_DIRECTORY}/pattern.h"
  #"${SOURCE_DIRECTORY}/pattern.c"
  #"${SOURCE_DIRECTORY}/trash.h"
  #"${SOURCE_DIRECTORY}/trash.c"
  "${SOURCE_DIRECTORY}/utils.h" )

enable_warnings(${TRASH_TARGET})