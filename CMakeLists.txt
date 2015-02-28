cmake_minimum_required(VERSION 2.8.4)
project(parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# library sources
set(SOURCE_FILES
    src/commandline_parser.cc
    src/usage_generator.cc
)

# create a static link library
add_library(parser STATIC ${SOURCE_FILES})

# create executables linked with libparser.a
add_executable(parser_test      test/parser_test.cc)
add_executable(generator_test   test/generator_test.cc)
add_executable(combination_test test/combination_test.cc)

target_link_libraries(parser_test       parser)
target_link_libraries(generator_test    parser)
target_link_libraries(combination_test  parser)
