#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <random>
//#include <gtkmm-4.0/gtkmm.h>

#define UNASSIGNED 0

class sudoku_grid{
	public:
		inline sudoku_grid();
		inline explicit sudoku_grid(std::string, bool row_major=true);
		inline void fillEmptyDiagonalBox(int);
		inline void createSeed();
		inline void printGrid();
		bool solveGrid();
		inline std::string getGrid();
		void countSolution(int& number);
		inline void genPuzzle();
		inline bool verifyGridStatus() const;
		inline void calculateDifficulty();
		inline int branchDifficultyScore();
	private:
		int grid [9][9];
		int solution_grid [9][9];
		int guessNum [9];
		int gridPos [81];
		int difficultyLevel;
		bool gridStatus;
};

//get grid as string in row major order
inline std::string sudoku_grid::getGrid(){
	std::string s;
	for (auto & row_num : grid)
		for (int col_num: row_num)
			s += std::to_string(col_num);


	return s;
}

//gen random number
std::random_device rd;
std::mt19937 mt(rd());

//helper functions for solving grid
inline bool findUnassignedLocation(int grid[9][9], int &row, int &col){
	for (row = 0; row < 9; row++)
		for (col = 0; col < 9; col++)
			if (grid[row][col] == UNASSIGNED)
				return true;

	return false;
}

inline bool usedInRow(int grid[9][9], int row, int num){
	for (int col = 0; col < 9; col++)
		if (grid[row][col] == num)
			return true;


	return false;
}

inline bool usedInCol(int grid[9][9], int col, int num){
	for (int row = 0; row < 9; row++)
		if (grid[row][col] == num)
			return true;


	return false;
}

inline bool usedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num){
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++)
			if (grid[row + boxStartRow][col + boxStartCol] == num)
				return true;


	return false;
}

inline bool isSafe(int grid[9][9], int row, int col, int num){
	return !usedInRow(grid, row, num) && !usedInCol(grid,col,num) && !usedInBox(grid,row-row%3,col-col%3,num);
}

//create seed grid
inline void sudoku_grid::fillEmptyDiagonalBox(int idx){
	int start=idx*3;
	std::shuffle(this->guessNum, (this->guessNum) + 9, mt);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			this->grid[start + i][start + j] = guessNum[i * 3 + j];
}

inline void sudoku_grid::createSeed(){
	/*Fill diagonal boxes to form
	 * x | . | .
	 * . | x | .
	 * . | . | x
	 */
	this->fillEmptyDiagonalBox(0);
	this->fillEmptyDiagonalBox(1);
	this->fillEmptyDiagonalBox(2);
	/*fill the remaining blocks
	 * x | x | x
	 * x | x | x
	 * x | x | x
	 */
	this->solveGrid();

	//saving solution grid
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			this->solution_grid[i][j] = this->grid[i][j];
}

//initialising
inline sudoku_grid::sudoku_grid(){
	//initialise difficultyLevel
	this->difficultyLevel=0;
	for (int i = 0; i < 81; i++)
		this->gridPos[i] = i;


	std::shuffle(this-> gridPos, (this->gridPos) + 81, mt);

	//Randomly shuffling the guessing number array
	for(int i=0; i<9; i++){
		this->guessNum[i]=i+1;
	}

	std::shuffle(this->guessNum, (this->guessNum) + 9, mt);

	//initialising the grid
	for (auto &i: this->grid)
		for (int &j: i)
			j = 0;

	gridStatus = true;
}

//grid passed as argument initialisation
inline sudoku_grid::sudoku_grid(std::string grid_str, bool row_major){
	if(grid_str.length() != 81){
		gridStatus = false;
		return;
	}

	//check if all cells are valid
	for(int i=0; i<81; i++){
		int cur_num = grid_str[i] -'0';
		if(!((cur_num == UNASSIGNED) || (cur_num > 0 && cur_num < 10))){
			gridStatus = false;
			return;
		}

		if(row_major) grid[i/9][i%9] = cur_num;
		else          grid[i%9][i/9] = cur_num;
	}

	//check if all columns are valid
	for(int col_num =0; col_num < 9; col_num++){
		bool nums[10]={false};
		for(auto & row_num : grid){
			int curr_num = row_num[col_num];
			if(curr_num != UNASSIGNED && nums[curr_num]){
				gridStatus=false;
				return;
			}
			nums[curr_num] = true;
		}
	}

	//check if all rows are valid
	for(auto & row_num : grid){
		bool nums[10]={false};
		for(int curr_num : row_num){
			if(curr_num != UNASSIGNED && nums[curr_num]){
				gridStatus=false;
				return;
			}
			nums[curr_num] = true;
		}
	}

	//check if all boxes are valid
	for(int block_num = 0; block_num < 9; block_num++){
		bool nums[10]={false};
		for(int cell_num = 0; cell_num < 9; cell_num++){
			int curr_num = grid[((int)(block_num/3))*3 + (cell_num/3)][((int)(block_num%3))*3 + (cell_num%3)];
			if(curr_num != UNASSIGNED && nums[curr_num]){
				gridStatus = false;
				return;
			}
		}
	}
	//randomly shuffling guessing number array
	for(int i = 0; i < 9; i++){
		this->guessNum[i] = i + 1;
	}

	std::shuffle(this->guessNum, (this->guessNum)+9, mt);

	gridStatus=true;
}

//printing the grid
inline void sudoku_grid::printGrid(){
	for(auto & i : grid){
		for(int j : i){
			if(j == UNASSIGNED){
				std::cout<<'.';
			}
			else{
				std::cout<<j;
			}
			std::cout<<'|';
		}
		std::cout<<std::endl;
	}
	std::cout<<"\nDifficulty of current sudoku(0 being easiest): "<<this->difficultyLevel;
	std::cout<<std::endl;
}

//modified sudoku solver
bool sudoku_grid::solveGrid(){
	int row,col;

	//if there is no unassigned location we are done
	if(!findUnassignedLocation(this->grid, row, col)){
		return true;//success!
	}

	//consider digits 1 to 9
	for (int num : this->guessNum)
		//if it looks promising
		if (isSafe(grid, row, col, num)) {
			//make temporary assignment
			this->grid[row][col] = num;

			//return if success
			if (solveGrid()) {
				return true;
			}

			//failure, unmake changes
			this->grid[row][col] = UNASSIGNED;
		}


	return false;//this triggers backtracking;

}

//check if the grid is uniquely solvable
void sudoku_grid::countSolution(int& number){
	int row, col;
	if(!findUnassignedLocation(this->grid, row, col)){
		number++;
		return;
	}

	for(int i = 0; i < 9 && number < 2; i++){
		if(isSafe(this->grid, row, col, guessNum[i])){
			this->grid[row][col] = this->guessNum[i];
			countSolution(number);
		}

		this->grid[row][col] = UNASSIGNED;
	}
}

//generate puzzle
inline void sudoku_grid::genPuzzle(){
	for(int & gridPo : this->gridPos){
		int x = gridPo / 9;
		int y = gridPo % 9;
		int temp=this->grid[x][y];
		this->grid[x][y] = UNASSIGNED;

		//if more than 1 solution replace it back
		int check = 0;
		countSolution(check);
		if(check != 1)
			this->grid[x][y] = temp;
	}
}

//calculate difficulty branch difficulty score
inline int sudoku_grid::branchDifficultyScore(){
	int emptyPositions = -1;
	int tempGrid[9][9];
	int sum = 0;

	for(int i = 0; i < 9; i++)
		for(int j = 0; j < 9; j++)
			tempGrid[i][j] = this->grid[i][j];



	while(emptyPositions != 0){
		std::vector<std::vector<int>> empty;

		for (int i = 0; i < 81; i++)
			if (tempGrid[(int) i / 9][(int) i % 9] == 0) {
				std::vector<int> temp;
				temp.push_back(i);

				for (int num = 1; num <= 9; num++)
					if (isSafe(tempGrid, i / 9, i % 9, num))
						temp.push_back(num);


				empty.push_back(temp);
			}

		if(empty.empty()){
			std::cout<<"Hello: "<<sum<<std::endl;
			return sum;
		}

		int minIndex = 0;
		int check = (int)empty.size();
		for (int i = 0; i < check; i++)
			if (empty[i].size() < empty[minIndex].size())
				minIndex = i;


		int branchingFactor = (int)empty[minIndex].size();
		int rowIndex = empty[minIndex][0] / 9;
		int colIndex = empty[minIndex][0] % 9;

		tempGrid[rowIndex][colIndex] = this->solution_grid[rowIndex][colIndex];
		sum = sum + ((branchingFactor - 2) * (branchingFactor -2));
		emptyPositions = (int)empty.size() - 1;
	}
	return sum;
}

//calculate difficulty
inline void sudoku_grid::calculateDifficulty(){
	int B = branchDifficultyScore();
	int emptyCells = 0;
	for (auto &i: this->grid)
		for (int j: i)
			if (j == UNASSIGNED)
				emptyCells++;


	this->difficultyLevel = B * 100 + emptyCells;
}

inline bool sudoku_grid::verifyGridStatus() const {
	return gridStatus;
}

/*class sudokuWindow : public Gtk::Window {

  };*/

int main(){
	//creating an instance of sudoku
	auto* puzzle = new sudoku_grid();

	//creating a seed for puzzle
	puzzle->createSeed();

	//generating puzzle
	puzzle->genPuzzle();

	//calculating difficulty
	puzzle->calculateDifficulty();

	//printing the grid for development purposes
	puzzle->printGrid();
}
