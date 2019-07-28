#pragma once

#include <iostream>
#include <vector>

namespace Sudoku
{
	struct GridP;

	const int Rows = 9;
	const int Cols = 9;
	const int SubGridsX = 3;
	const int SubGridsY = 3;

	class Puzzle
	{
	public:
		static Puzzle PullFromHTTP();
		Puzzle(const std::string &PuzzleJson);
		Puzzle();

		void Output();
		void OutputSolution(std::vector<GridP> &Solution);

		bool SolveSingleNumberDeduction(std::vector<GridP> Grid0, std::vector<GridP> &Solution0);

		bool SolveRecursive(std::vector<GridP> Grid0, std::vector<GridP> &Solution0);

		bool CheckSolution(const std::vector<GridP> &Solution0);

		std::vector<GridP> Grid;

		bool Filled = false;
		int Iterations = 0;

	private:
		static std::vector<int> ListMissing(const std::vector<int> &Data);
		
		static std::vector<int> PairMissing(const std::vector<int> &missingInRow, const std::vector<int> &missingInCol, const std::vector<int> &missingInSubgrid);
	};

	struct GridP
	{
		GridP(int X, int Y, int Value)
		{
			this->X = X;
			this->Y = Y;
			this->Value = Value;
			this->Order = X + Y * Cols;

			SubGridX = 2;
			SubGridY = 2;
			if (X < 6) SubGridX = 1;
			if (Y < 6) SubGridY = 1;
			if (X < 3) SubGridX = 0;
			if (Y < 3) SubGridY = 0;
		}
		GridP(int Value, int Order)
		{
			this->Value = Value;
			this->Order = Order;
		}
		GridP() { }


		int X;
		int Y;
		int Value;

		int Order = -1;
		int SubGridX;
		int SubGridY;

		bool operator < (const GridP& gridp) const
		{
			return (Order < gridp.Order);
		}
	};
}
