cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("MeasureTime")

include_directories(include)
add_executable(Demo main.cc include/measure_time.h src/measure_time.cc)