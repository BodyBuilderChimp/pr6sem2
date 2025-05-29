cmake_minimum_required(VERSION 3.5.2)
project(poiskovik)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(pr6sem2part1 pr6sem2part1.c)
add_executable(pr6sem2part2 pr6sem2part2.c)

target_link_libraries(pr6sem2part1 Threads::Threads)
target_link_libraries(pr6sem2part2 Threads::Threads)
