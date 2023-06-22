# Simulation of Conway's Game of Life created during the course "Introduction to programming".

Conway's Game of Life  https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

## Compilation 
- to compile run : gcc  -DWIERSZE=22 -DKOLUMNY=80 main.c -o main 
- to change the size of a board f.ex to 25 rows and 100 columns run: gcc  -DWIERSZE=25 -DKOLUMNY=100 main.c -o main
  
## Rules: 

- 0 and . represent the cell alive and dead, respectively. 

- To create the first generation type f.ex: 
```
/0 11 12 13 
/1 12 13 
/2 14 15
/
```

"/0 11 12 13" means that the row 0 will have living cells in columns: 11 12 13.

"/1 12 13" means that there will be cells alive in row 1 in 11-th and 12-th column. 
etc. 

when finished creating the first generation type: /

### After creating initial generation
- To see the current generation type: 0
- To move the window x rows down and y columns to right type: x y
- To see the n-th generation type: n
- To exit the game type: . 

### Sample in out files  
Sample input and output files can be found in in.out folder
