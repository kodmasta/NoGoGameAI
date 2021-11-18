# NoGoGameAI
An AI that can play the game of No-Go

AI design: Minimax + Alpha-Beta Pruning (Computer vs Player)

1. Computer (C) only judges the surrounding area of Player (P)'s position in a round. There should be 8 to 15 "placeable positions" in the area, which is represented by 'n'. The program uses the dfs_air() and judgeAvailable() functions to determine whether a location is "available", and uses the findScope() function to determine the specific location and size of the area.

2. findScope() uses (rand()% 16) in a switch with 16 cases to find an optimal area around the Player's last move. findScope() is an important aspect of this AI program, because it helps define the area of analysis more flexibly and efficiently, so the Computer can compute more efficiently the "Minimax + Alpha-Beta Pruning" algorithm.

3. The Maximizer of the "Minimax + Alpha-Beta" Game Tree is the Computer, and the Minimizer is the Player. By saving Alpha and Beta values, the Game Tree realizes the optimization of pruning. The depth of the "Minimax + Alpha-Beta" Game Tree is D+1. The depth is determined in advance in #define Target_Depth.

4. The program uses the evaluationFunc() function to determine the return value of each leaf node of the "Minimax + Alpha-Beta" Game Tree (it calculates how many positions on the chessboard the Computer can still place chess pieces). The total number of leaf nodes depends on the "placeable positions" n in the return area of the findScope() function: [n!/(n-D)! - (the "pruned" subtrees and leaf nodes)]. The evaluationFunc() is very simple, optimization mainly takes place in findScope() and pruning.

5. Through the above analysis, the findBestMove() function finds the values of all positions in the scope. Its return value is the position of the Computer's move.


Playing instructions:

Run the file. The game has a complete menu (press "M" to enter the menu). "Save" and "Resume" options are done using ofstream myfile ("gamedata.txt") and ifstream myfile ("gamedata.txt"), therefore, if you run the program on a Windows system, you may need to rewrite the directory.
