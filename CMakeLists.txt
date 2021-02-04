cmake_minimum_required(VERSION 3.19)
project(cppcsv VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)


add_library(cppcsv INTERFACE)
target_include_directories(cppcsv INTERFACE .)
