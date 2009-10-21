PROJECT(nova-tt)
cmake_minimum_required(VERSION 2.6)

enable_testing()

include_directories (boost_lockfree)
include_directories (nova-tt)

find_library(pthread pthread)
add_subdirectory (testsuite)
