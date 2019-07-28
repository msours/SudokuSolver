
cmake_minimum_required(VERSION 3.9)

set(lib_dir ${CMAKE_BINARY_DIR}/lib)
set(bin_dir ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${lib_dir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

set(project_name SudokuSolver)
set(project_version 1.0.0)

project(${project_name} DESCRIPTION "Retrieves Sudoku puzzle from the web (hosted by University of Texas), and then attempts to solve the puzzle first with simple deduction. Then solves the puzzle through recursive back tracking.")

include_directories(./include /usr/include)

find_package(Boost REQUIRED COMPONENTS system thread)

set(Boost_USE_STATIC_LIBS     OFF)
set(Boost_USE_MULTITHREADED    ON)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(SudokuSolver
    src/Puzzle.cpp
    src/WebClient.cpp
    src/SudokuSolver.cpp
)

target_link_libraries(SudokuSolver ${Boost_LIBRARIES})
