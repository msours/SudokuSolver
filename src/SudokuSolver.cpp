
#include "Puzzle.h"


int main()
{
	std::cout << "\n\n";

	Sudoku::Puzzle SudokuPuzzle = Sudoku::Puzzle::PullFromHTTP();

	if (!SudokuPuzzle.Filled) 
	{
		std::cout << "Failed pulling Sudoku Puzzle (hosted by University of Texas).\n\n";
		return 0;
	}
	std::cout << "Pulled Sudoku Puzzle (hosted by University of Texas):\n\n";
	SudokuPuzzle.Output();

	std::vector<Sudoku::GridP> Solution;
	std::vector<Sudoku::GridP> Grid = SudokuPuzzle.Grid;
	bool Success = SudokuPuzzle.SolveSingleNumberDeduction(Grid, Solution);

	std::cout << "\n\n";
	if (Success)
	{
		std::cout << "Solved by deducing a single solution for each grid point (no back tracking) \nin " << SudokuPuzzle.Iterations << " iterations.\n\n";
		SudokuPuzzle.OutputSolution(Solution);
	}
	else
	{
		std::cout << "Solved " << Solution.size() << " values in the puzzle by deducing a single solution for each point. \nSolving the rest of the puzzle will require another method.\n\n";
		SudokuPuzzle.OutputSolution(Solution);	
	}

	SudokuPuzzle.Iterations = 0;
	Solution.clear();
	Grid = SudokuPuzzle.Grid;

	Success = SudokuPuzzle.SolveRecursive(Grid, Solution);

	std::cout << "\n\n";
	if (Success)
	{
		std::cout << "Solved by recursive back tracking in " << SudokuPuzzle.Iterations << " iterations.\n\n";
		SudokuPuzzle.OutputSolution(Solution);
	}
	else
	{
		std::cout << "Could not solve puzzle. Perhaps this puzzle is invalid?\n\n";
	}

	return 0;
}
