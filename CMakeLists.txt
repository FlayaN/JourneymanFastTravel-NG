cmake_minimum_required(VERSION 3.26)

set(PROJECT_AUTHOR "FlayaN")
project(
  Journeyman
  VERSION 1.1.4
  LANGUAGES CXX)

# ##############################################################################
# # Source build guard
# ##############################################################################

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
message(
  FATAL_ERROR
    "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
)
endif()

# ##############################################################################
# # Add CommonLibNG plugin
# ##############################################################################

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(CLIBNGPlugin)
add_simple_commonlibsse_ng_plugin(AUTHOR "${PROJECT_AUTHOR}" LICENSE
                                "MIT License")
