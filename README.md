# Sudoku-Generator
  
A Sudoku puzzle generator written in C++.

## Steps to Use:-  

### Requirements: 
-> git  
-> Latest version C++ compiler , (this program has been tested on g++ only)   

### Linux and MacOS

Type the follwing commands on your terminal (without the '$')  
```
$ git clone https://github.com/vaithak/Sudoku-Generator
$ cd Sudoku-Generator
$ bash setup.sh
```  

**Now restart the terminal and you are good to go**  

To run => enter `$ sudokuGen` from anywhere in the terminal  
You can view the svg image generated in any Browser.  

### Example Puzzle generated from the program
![image](https://github.com/vaithak/Sudoku-Generator/blob/master/images/example_puzzle.svg)  
   
### FlowChart

![image](https://github.com/vaithak/Sudoku-Generator/blob/master/images/flowchart1.png)  
![image](https://github.com/vaithak/Sudoku-Generator/blob/master/images/flowchart2.png)  

### Working of Step 1 and Step 6:  

**Step 1**  
```
 => Empty grid is solved using the normal backtracking method only, 
    but to make the seed different every time, we will not check numbers 
    1-9 sequentially at empty position.
    
 => Rather we will shuffle the list containing numbers 1-9 and fill cell according
    to it.
    
 => This ensures every time the program is run, the seed is different.
```  

**Step 6: Difficulty estimation** 
```
  
  Estimating the difficulty of a Sudoku puzzle is a tricky problem because of the
  variety of techniques human solvers use. 
  
  A quick and easy method that correlates roughly with difficulty is to keep track of the
  branch factor at each step on the path from the root of the search tree to the solution.

  We compute a branch-difficulty score by summing (B(i) - 1)^2 at each node, 
  where Bi are the branch factors. 
  
  A solution which requires no backtracking at all would thus have a branch-difficulty score of 0.

  The final score is given by:  S = B * 100 + E
  
  Where B is the branch-difficulty score and E is the number of empty cells. 
  E is included to bias the puzzle generator in the direction of fewer clues, 
  given multiple puzzles with the same branch-difficulty.

  A puzzle which requires no backtracking ends up with a final score of less than 100. 
  
  However, this naive approach does not correlate well with actual difficulty unless 
  a modification is made to the solver algorithm.
```   


### Modified Solver
```
  Rather than considering the possible values for a given empty cell, we could consider
  possible positions for a given missing value in one of the rows, columns or boxes.

  We make the following modification to the solver:

    1) Choose the cell with the fewest possible candidates. If no such cell can be found, the puzzle is solved.

    2) Search sets (rows, columns and boxes) for missing values, and count the positions they could occupy.
       Identify the set and value with the fewest number of possible positions.

    3) If the set-search result gives a number of positions which is smaller than the number of candidate values
       found in step 1, then continue with step 4. Otherwise, continue with step 5.

    4) Try filling the identified value in each candidate position in the set and recursively solve. 
       If all candidate positions are exhausted, signal failure to the caller.

    5) For each candidate value in the cell identified in step 1, place the value in the cell and 
       try to recursively solve the puzzle. If all candidate values are exhausted, signal failure.

  Essentially, the algorithm is the same, except that we try the set-oriented approach 
  if it results in a smaller branch factor. 
  This means that hidden singles or tuples yield similar branch factors to their naked equivalents.

  Making this modification often changes the results of difficulty estimations drastically. 
```  

[Reference](https://utiny.herokuapp.com/gt)  
