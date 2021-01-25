# NoGoGameAI
An AI that can play the game of No-Go

AI design: Minimax + Alpha-Beta Pruning (Computer vs Player)

1. Computer (C) only judges the surrounding area ofPlayer (P)'s position in this round. There should be 8 to 15 "placeable positions" in the area, which is n. The program uses the dfs_air() and judgeAvailable() functions to determine whether a location is "available", and uses the findScope() function to determine the specific location and size of the area.

2. findScope() uses (rand()% 16) in a switch with 16 cases to change the player position as the starting point to gradually find an optimal area. findScope() is the most unique part of my program, because it can help me find the area of analysis more flexibly, so I can realize "Minimax + Alpha-Beta Pruning".

3. The Maximizer of "Minimax + Alpha-Beta" Game Tree is Computer, and Minimizer is Player. By saving Alpha and Beta values, Game Tree has realized the optimization of pruning. The depth of the "Minimax + Alpha-Beta" Game Tree is D+1. The depth is determined in advance in #define Target_Depth.

4. The program uses the evaluationFunc() function to determine the return value of each leaf node of the "Minimax + Alpha-Beta" Game Tree (it calculates how many positions on the chessboard Computer can place chess pieces). The total number of leaf nodes depends on the "placeable position" n in the return area of the findScope() function: [total number = — Alpha-Beta minus subtrees and leaf nodes]. My evaluationFunc() is very simple, and the program optimization mainly relies on the findScope() function and pruning.

5. Through the above analysis, the findBestMove() function finds the values of all positions in the scope. Its return value is the position of AI drop.


UI design:
Please see the program. The game has a complete menu (press "M" to enter the menu) to meet the basic requirements. Save and read the disk using ofstream myfile ("gamedata.txt") and ifstream myfile ("gamedata.txt"), so if you run on a Windows system, you may need to rewrite the directory.
