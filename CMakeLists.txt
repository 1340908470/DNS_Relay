cmake_minimum_required(VERSION 3.17)
project(DNS C)

set(CMAKE_C_STANDARD 99)
link_libraries(ws2_32)
add_executable(DNS main.c struct.h include.h hashmap.h hashmap.c)