cmake_minimum_required(VERSION 3.26)
project(Project)

set(CMAKE_CXX_STANDARD 17)

add_executable(main
        main.cpp
        Algorithms.cpp
)

target_link_libraries(main gdi32)