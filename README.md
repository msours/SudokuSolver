# SudokuSolver
Retrieves Sudoku puzzle from web service, and then attempts to solve the puzzle first with simple deduction. Then solves the puzzle through recursive back tracking.

Dependencies: Boost (asio, system) is used to create the web client that retrieves a randomly generated Sudoku puzzle from a web service hosted by University of Texas:
http://www.cs.utep.edu/cheon/ws/sudoku/

## To build on Linux:
1. mkdir build
2. cd build
3. cmake ..
4. make

## To build on Windows (tested with Visual Studio 2017):
1. Download (probably any) boost release (tested with 1.70): https://www.boost.org/
2. mkdir build
3. cd build
4. cmake .. -G "Visual Studio 15 2017 Win64" -D Boost_INCLUDE_DIRS=\<Path To Boost download>
5. cmake --build . --config <debug/release>
