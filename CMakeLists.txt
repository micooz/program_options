cmake_minimum_required(VERSION 2.8.4)
project(program_options)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

# library sources
set(SOURCE_FILES
  src/program_options/error.cc
  src/program_options/generator.cc
  src/program_options/item.cc
  src/program_options/parser.cc
  src/program_options/row.cc
  src/program_options/subroutine.cc
  include/program_options.h
  include/program_options/error.h
  include/program_options/generator.h
  include/program_options/item.h
  include/program_options/parser.h
  include/program_options/row.h
  include/program_options/subroutine.h
  include/program_options/functions.h
  include/program_options/switch.hpp
)

# create a static link library
add_library(program_options STATIC ${SOURCE_FILES})

# gtest
add_subdirectory(googletest)
include_directories(googletest/include/)
enable_testing()

# create executables linked with libparser.a
add_executable(parser_test      test/parser_test.cc)
add_executable(generator_test   test/generator_test.cc)
add_executable(combination_test test/combination_test.cc)
add_executable(switch_test      test/switch_test.cc)

target_link_libraries(parser_test       program_options gtest)
target_link_libraries(generator_test    program_options gtest)
target_link_libraries(combination_test  program_options gtest)
target_link_libraries(switch_test  program_options gtest)