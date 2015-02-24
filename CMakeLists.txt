cmake_minimum_required(VERSION 2.8.4)
project(parser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(CommandLineParser      main.cc src/commandline_parser.cc)
add_executable(CommandLineParser_test src/commandline_parser.cc test/test.cc)