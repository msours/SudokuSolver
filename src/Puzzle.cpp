
#include "Puzzle.h"
#include "json.hpp"
#include "WebClient.h"
#include <algorithm>

namespace Sudoku
{
	Puzzle::Puzzle(const std::string &PuzzleJson)
	{
		nlohmann::json puzzleJson = nlohmann::json::parse(PuzzleJson);

		for (int k = 0; k < puzzleJson["squares"].size(); k++) 
		{
			const nlohmann::json &gridP = puzzleJson["squares"][k];
			this->Grid.push_back(GridP(gridP["x"], gridP["y"], gridP["value"]));
		}	

		for (int k = 0; k < Rows ; k++)
		{
			for (int j = 0; j < Cols; j++) 
			{
				const int Order = j + k * Cols;
				std::vector<GridP>::iterator itr = std::find_if(Grid.begin(), Grid.end(), [x = Order](const GridP & val) {if (val.Order == x) return true; return false;  });

				if (itr != Grid.end()) continue;

				Grid.push_back(GridP(j, k, -1));
			}
		}

		std::sort(Grid.begin(), Grid.end());
		this->Filled = true;
	}
	Puzzle::Puzzle()
	{
		
	}
	Puzzle Puzzle::PullFromHTTP()
	{
		std::string Response;
		bool Success = false;
		try
		{
			Success = WebClient::HTTP::GET("www.cs.utep.edu", "/cheon/ws/sudoku/new/?size=9?level=3", &Response);
		}
		catch (char *e)
		{
			return Puzzle();
		}

		if (!Success) return Puzzle();
		return Puzzle(Response);
	}
	bool Puzzle::SolveSingleNumberDeduction(std::vector<GridP> Grid0, std::vector<GridP> &Solution0)
	{
		std::vector<GridP> UnsolvedP;
		for (int k = 0; k < Grid0.size(); k++)
		{
			if (Grid0[k].Value < 0)
			{
				UnsolvedP.push_back(Grid0[k]);
			}
		}

		while (true) 
		{
			if (UnsolvedP.size() == 0) return true;		

			std::vector<int> SolvedP;
			for (int k = 0; k < UnsolvedP.size(); k++)
			{
				Iterations++;

				std::vector<int> RowV;
				std::vector<int> ColV;
				std::vector<int> SubgridV;
				for (int j = 0; j < Grid0.size(); j++)
				{
					if (Grid0[j].X == UnsolvedP[k].X) ColV.push_back(Grid0[j].Value);
					if (Grid0[j].Y == UnsolvedP[k].Y) RowV.push_back(Grid0[j].Value);
					if (UnsolvedP[k].SubGridX == Grid0[j].SubGridX && UnsolvedP[k].SubGridY == Grid0[j].SubGridY) SubgridV.push_back(Grid0[j].Value);
				}

				std::vector<int> pairedMissing = PairMissing(ListMissing(RowV), ListMissing(ColV), ListMissing(SubgridV));
				if (pairedMissing.empty()) return false;
				if (pairedMissing.size() > 1) continue;

				std::vector<GridP>::iterator itr = std::find_if(Grid0.begin(), Grid0.end(), [x = UnsolvedP[k].X, y = UnsolvedP[k].Y](const GridP & val) {if (val.X == x && val.Y == y) return true; return false;  });
				if (itr == Grid0.end()) return false;

				int Ind = std::distance(Grid0.begin(), itr);

				Grid0[Ind].Value = pairedMissing[0];

				UnsolvedP[k].Value = pairedMissing[0];
				Solution0.push_back(UnsolvedP[k]);

				SolvedP.push_back(k);
			}
			if (SolvedP.size() == 0) return false;
			for (int k = 0; k < SolvedP.size(); k++) 
			{
				UnsolvedP.erase(UnsolvedP.begin() + (SolvedP[k] - k));
			}
		}

		return false;
	}
	bool Puzzle::SolveRecursive(std::vector<GridP> Grid0, std::vector<GridP> &Solution0)
	{
		Iterations++;

		GridP UnsolvedP;

		for (int k = 0; k < Grid0.size(); k++)
		{
			if (Grid0[k].Value < 0)
			{
				UnsolvedP = Grid0[k];
				break;
			}
		}

		if (UnsolvedP.Order == -1)
		{
			if (CheckSolution(Solution0)) return true;
			return false;
		}

		std::vector<int> RowV;
		std::vector<int> ColV;
		std::vector<int> SubgridV;
		for (int k = 0; k < Grid0.size(); k++)
		{
			if (Grid0[k].X == UnsolvedP.X) ColV.push_back(Grid0[k].Value);
			if (Grid0[k].Y == UnsolvedP.Y) RowV.push_back(Grid0[k].Value);
			if (UnsolvedP.SubGridX == Grid0[k].SubGridX && UnsolvedP.SubGridY == Grid0[k].SubGridY) SubgridV.push_back(Grid0[k].Value);
		}

		std::vector<int> pairedMissing = PairMissing(ListMissing(RowV), ListMissing(ColV), ListMissing(SubgridV));
		if (pairedMissing.empty()) return false;

		std::vector<GridP>::iterator itr = std::find_if(Grid0.begin(), Grid0.end(), [x = UnsolvedP.X, y = UnsolvedP.Y](const GridP & val) {if (val.X == x && val.Y == y) return true; return false;  });
		if (itr == Grid0.end()) return false;

		int Ind = std::distance(Grid0.begin(), itr);

		for (int k = 0; k < pairedMissing.size(); k++)
		{
			Grid0[Ind].Value = pairedMissing[k];

			UnsolvedP.Value = pairedMissing[k];
			Solution0.push_back(UnsolvedP);

			if (SolveRecursive(Grid0, Solution0)) return true;

			Solution0.pop_back();
		}

		return false;
	}
	std::vector<int> Puzzle::ListMissing(const std::vector<int> &Data)
	{
		std::vector<int> Missing;
		for (int k = 1; k < Rows + 1; k++)
		{
			std::vector<int>::const_iterator itr = std::find(Data.begin(), Data.end(), k);
			if (itr == Data.end()) Missing.push_back(k);
		}
		return Missing;
	}
	std::vector<int> Puzzle::PairMissing(const std::vector<int> &missingInRow, const std::vector<int> &missingInCol, const std::vector<int> &missingInSubgrid)
	{
		std::vector<int> CommonP;
		for (int k = 0; k < missingInRow.size(); k++)
		{
			for (int j = 0; j < missingInCol.size(); j++)
			{
				if (missingInRow[k] == missingInCol[j])
				{
					for (int p = 0; p < missingInSubgrid.size(); p++)
					{
						if (missingInCol[j] == missingInSubgrid[p]) CommonP.push_back(missingInRow[k]);
					}
				}
			}
		}

		return CommonP;
	}
	bool Puzzle::CheckSolution(const std::vector<GridP> &Solution0)
	{
		std::vector<GridP> Grid0 = this->Grid;
		for (int k = 0; k < Grid0.size(); k++)
		{
			if (Grid0[k].Value < 0)
			{
				bool Paired = false;
				for (int j = 0; j < Solution0.size(); j++)
				{
					if (Solution0[j].Order == Grid0[k].Order)
					{
						Grid0[k].Value = Solution0[j].Value;
						Paired = true;
					}
				}
				if (!Paired) return false;
			}
		}
		for (int k = 0; k < Rows; k++)
		{
			int R = 0;
			for (int j = 0; j < Grid0.size(); j++)
			{
				if (k == Grid0[j].Y) R += Grid0[j].Value;
			}
			if (abs(R - 45) != 0) return false;
		}
		for (int k = 0; k < Cols; k++)
		{
			int C = 0;
			for (int j = 0; j < Grid0.size(); j++)
			{
				if (k == Grid0[j].X) C += Grid0[j].Value;
			}
			if (abs(C - 45) != 0) return false;
		}
		for (int k = 0; k < SubGridsX; k++)
		{
			for (int j = 0; j < SubGridsY; j++)
			{
				int S = 0;
				for (int p = 0; p < Grid0.size(); p++)
				{
					if (k == Grid0[p].SubGridX && j == Grid0[p].SubGridY) S += Grid0[p].Value;
				}
				if (abs(S - 45) != 0) return false;
			}
		}
		return true;
	}
	void Puzzle::Output()
	{
		if (!this->Filled) return;

		int t = 0;
		std::cout << "|---+---+---|---+---+---|---+---+---|\n";
		for (int k = 0; k < Rows; k++)
		{
			std::cout << "| ";
			for (int j = 0; j < Cols; j++)
			{
				if (Grid[t].Value > 0) std::cout << Grid[t].Value << " ";
				else std::cout << "  ";

				std::cout << "| ";
				t++;
			}
			if (k == 2 || k == 5 || k == 8) std::cout << "\n|---+---+---|---+---+---|---+---+---|\n";
			else std::cout << "\n+-----------+-----------+-----------+\n";
		}
	}
	void Puzzle::OutputSolution(std::vector<GridP> &Solution)
	{
		if (!this->Filled) return;

		std::sort(Solution.begin(), Solution.end());

		int t = 0;
		std::cout << "|---+---+---|---+---+---|---+---+---|\n";
		for (int k = 0; k < Rows; k++)
		{
			std::cout << "| ";
			for (int j = 0; j < Cols; j++)
			{
				if (Grid[t].Value > 0) std::cout << Grid[t].Value << " ";
				else
				{
					bool SolvedP = false;
					for (int m = 0; m < Solution.size(); m++)
					{
						if (Grid[t].Order == Solution[m].Order)
						{
							std::cout << Solution[m].Value << " ";
							SolvedP = true;
							continue;
						}
					}
					if (!SolvedP) std::cout << "  ";
				}

				std::cout << "| ";
				t++;
			}
			if (k == 2 || k == 5 || k == 8) std::cout << "\n|---+---+---|---+---+---|---+---+---|\n";
			else std::cout << "\n+-----------+-----------+-----------+\n";
		}
	}
}

