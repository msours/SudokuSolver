## SudokuSolver
Retrieves Sudoku puzzle from a [web service](http://www.cs.utep.edu/cheon/ws/sudoku/), and then attempts to solve the puzzle first with simple deduction. Then solves the puzzle through recursive back tracking.


### Building and Installing
Requirements:
* [Boost](https://www.boost.org/) (asio, system). Tested with version 1.70, but most likely any release will work.
#### Linux
After cloning the repo,
* `mkdir build`
* `cd build`
* `cmake ..`
* `make`

#### Windows
After cloning the repo,
* `mkdir build`
* `cd build`
* `cmake .. -G "Visual Studio 15 2017 Win64" -D Boost_INCLUDE_DIRS=<Path To Boost download>`
* `cmake --build . --config release`
