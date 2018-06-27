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

## TODO

=> Currently the sudoku created is of Random difficulty.  
   I have to implement a way such that the sudoku generated is of difficulty level entered by the User.  
   
   
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

**Step 6:**  
```
  TODO
```  
