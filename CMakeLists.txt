cmake_minimum_required(VERSION 2.8.4)
project(parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/commandline_parser.cc
    src/usage_generator.cc
)

add_executable(parser           main.cc                 ${SOURCE_FILES})
add_executable(parser_test      test/parser_test.cc     ${SOURCE_FILES})
add_executable(generator_test   test/generator_test.cc  ${SOURCE_FILES})
