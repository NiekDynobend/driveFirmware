cmake_minimum_required(VERSION 3.20)
project (simulate C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(simulate 
src/simulate.c
src/frame.c)

target_include_directories(simulate PRIVATE 
include/)