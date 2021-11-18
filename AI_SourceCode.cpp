#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstdlib>
#include<string>
#include<ctime>
using namespace std;

#define BWIDTH 9
#define BHEIGHT 9
#define Max_Win BWIDTH*BHEIGHT
#define Min_Win 0
#define Target_Depth 4
#define Max_Scope_Size 15
#define Min_Scope_Size 8

///     UI Design

void ClearScreen(){
   cout << string( 100, '\n' );
}

void welcomeMessage(){
    cout << "Welcome to The Game of No Go!" << endl << endl
    << "Rules: " << endl
    << "To Play, type Moves in (row,column) Format." << endl
    << "For Menu, type M. " << endl;
    return;
}

void drawBoard(char (*board)[BWIDTH]){
    for (int i = 0; i < BHEIGHT; i++) {
        for (int j = 0; j < BWIDTH; j++) {
            if(j == 0)
                cout << board[i][j];
            else
                cout << setfill('-') << setw(3) << board[i][j];
        }cout << endl;
        if(i < BHEIGHT-1){
            for (int j = 0; j < BWIDTH; j++) {
                if(j == 0)
                    cout << '|';
                else
                    cout << setfill(' ') << setw(3) << '|';
            }cout << endl;
        }
    }
}

void saveGame(char (*board)[BWIDTH]){
    ofstream myfile ("gamedata.txt");
    if(myfile.is_open()){
        for(int i = 0; i < BHEIGHT; i++){
            for(int j = 0; j < BWIDTH; j++){
                myfile << board[i][j];
            }
        }
        myfile.close();
        ClearScreen();
        drawBoard(board);
    }
    else
        cout << "Unable to save game";
}

void openLastGame(char (*board)[BWIDTH]){
    ifstream myfile ("gamedata.txt");
    if (myfile.is_open()){
        for(int i = 0; i < BHEIGHT; i++){
            for(int j = 0; j < BWIDTH; j++){
                myfile >> noskipws >> board[i][j];
            }
        }
    }
    else
        cout << "Unable to save game";
    ClearScreen();
    drawBoard(board);
}

void newGame(char (*board)[BWIDTH]){
    ClearScreen();
    memset(board, ' ', sizeof(board[0][0]) * BHEIGHT*BWIDTH);
    drawBoard(board);
}

int Menu(char (*board)[BWIDTH]){
    cout << "To Start a New Game, type 'N'." << endl
    << "To Save Current Game, type 'S'."<< endl
    << "To Play Last Saved Game, type 'L'." << endl
    << "To Return to Game, type 'R'."<< endl
    << "To Exit, type 'E'."<< endl;
    char command[100];
    while(cin >> command){
        if(command[0] == 'N'){
            newGame(board);
            return 0;
        }
        else if(command[0] == 'S'){
            saveGame(board);
            return 0;
        }
        else if(command[0] == 'L'){
            openLastGame(board);
            return 0;
        }
        else if(command[0] == 'R'){
            ClearScreen();
            drawBoard(board);
            return 0;
        }
        else if(command[0] == 'E'){
            return 1;
        }
        else{
            cout << "Invalid Command, please retry" << endl;
            continue;
        }
    }
    return 0;
}


///     DFS for board (move validation, case availability)

bool dfs_air_visit[BHEIGHT][BWIDTH];
const int cx[]={-1,0,1,0};
const int cy[]={0,-1,0,1};

bool inBorder(int x, int y){
    return x>=0 && y>=0 && x<BWIDTH && y<BHEIGHT;
}


bool dfs_air(int fx, int fy, char (*board)[BWIDTH]){
    dfs_air_visit[fx][fy] = true;
    bool flag = false;
    for (int dir = 0; dir < 4; dir++){
        int dx = fx+cx[dir], dy = fy+cy[dir];
        if (inBorder(dx, dy)){
            if (board[dx][dy] == ' ')
                flag = true;
            if (board[dx][dy] == board[fx][fy] && !dfs_air_visit[dx][dy])
                if (dfs_air(dx, dy, board))
                    flag = true;
        }
    }
    return flag;
}

bool judgeAvailable(int fx, int fy, char piece, char (*board)[BWIDTH]){
    if (board[fx][fy] != ' ')
        return false;
    board[fx][fy] = piece;
    memset(dfs_air_visit, 0, sizeof(dfs_air_visit));
    if (!dfs_air(fx, fy, board)){
        board[fx][fy]  = ' ';
        return false;
    }
    for (int dir = 0; dir < 4; dir++){
        int dx=fx+cx[dir], dy=fy+cy[dir];
        if (inBorder(dx, dy)){
            if (board[dx][dy] != ' ' && !dfs_air_visit[dx][dy])
                if (!dfs_air(dx, dy, board)){
                    board[fx][fy]  = ' ';
                    return false;
                }
        }
    }
    board[fx][fy]  = ' ';
    return true;
}


///     Minimax+Alpha-Beta Pruning AI

int evaluationFunc(char (*board)[BWIDTH]){
    int count = 0;
    for (int i = 0; i < BHEIGHT; i++)
        for (int j = 0; j < BWIDTH; j++)
            if(judgeAvailable(i, j, 'C', board))
                count++;
    return count;
}

bool maxScope(char (*board)[BWIDTH]){
    int count = 0;
    for (int i = 0; i < BHEIGHT; i++)
        for (int j = 0; j < BWIDTH; j++){
            if(judgeAvailable(i, j, 'C', board))
                count++;
            if(count > Max_Scope_Size+2)
                return false;
        }
    return true;
}

int playableCellsInScope(char (*board)[BWIDTH],int i1, int i2, int j1, int j2){
    int count = 0;
    for (int i = i1; i <= i2; i++){
        for (int j = j1; j <= j2; j++){
            if(judgeAvailable(i, j, 'C', board))
                count++;
        }
    }
    return count;
}

struct Scope{
    int i1, j1, i2, j2;
};

Scope findScope(char (*board)[BWIDTH], char *input){
    Scope bestScope;
    srand(time(NULL));
    int i0 = input[1]-'0';
    int j0 = input[3]-'0';
    bestScope.i1 = i0;
    bestScope.i2 = i0;
    bestScope.j1 = j0;
    bestScope.j2 = j0;
    
    while(true){
        if(maxScope(board)){
            bestScope.i1 = 0;
            bestScope.i2 = BHEIGHT-1;
            bestScope.j1 = 0;
            bestScope.j2 = BWIDTH-1;
        }
        else{
            while(true){
                switch(rand() % 16) {
                    case 0 :
                        if(bestScope.i1 != BHEIGHT-1 && bestScope.i1 != i0 && bestScope.i1 != bestScope.i2)
                            bestScope.i1++;
                        break;
                    case 1 :
                        if(bestScope.i1 != 0)
                            bestScope.i1--;
                        break;
                    case 2 :
                        if(bestScope.i2 != BHEIGHT-1)
                            bestScope.i2++;
                        break;

                    case 3 :
                        if(bestScope.i2 != 0 && bestScope.i2 != i0 && bestScope.i2 != bestScope.i1)
                            bestScope.i2--;
                        break;
                    case 4 :
                        if(bestScope.j1 != BWIDTH-1 && bestScope.j1 != j0 && bestScope.j1 != bestScope.j2)
                            bestScope.j1++;
                        break;
                    case 5 :
                        if(bestScope.j1 != 0)
                            bestScope.j1--;
                        break;
                    case 6 :
                        if(bestScope.j2 != BWIDTH-1)
                            bestScope.j2++;
                        break;
                    case 7 :
                        if(bestScope.j2 != 0 && bestScope.j2 != j0 && bestScope.j2 != bestScope.j1)
                            bestScope.j2--;
                        break;
                    case 8 :
                        if(bestScope.i1 <= BHEIGHT-3 && bestScope.i1 <= i0-2)
                            bestScope.i1 += 2;
                        break;
                    case 9 :
                        if(bestScope.i1 >= 2)
                            bestScope.i1 -= 2;
                        break;
                    case 10 :
                        if(bestScope.i2 <= BHEIGHT-3)
                            bestScope.i2 += 2;
                        break;
                    case 11 :
                        if(bestScope.i2 >= 2 && bestScope.i2 >= i0+2)
                            bestScope.i2 -= 2;
                        break;
                    case 12 :
                        if(bestScope.j1 <= BWIDTH-3 && bestScope.j1 <= j0-2)
                            bestScope.j1 += 2;
                        break;
                    case 13 :
                        if(bestScope.j1 >= 2)
                            bestScope.j1 -= 2;
                        break;
                    case 14 :
                        if(bestScope.j2 <= BWIDTH-3)
                            bestScope.j2 += 2;
                        break;
                    case 15 :
                        if(bestScope.j2 >= 2 && bestScope.j2 >= i0+2)
                            bestScope.j2 -= 2;
                        break;
                }
                int temp = playableCellsInScope(board, bestScope.i1, bestScope.i2, bestScope.j1, bestScope.j2);
                if(i0 >= bestScope.i1 && i0 <= bestScope.i2 && j0 >= bestScope.j1 && j0 <= bestScope.j2 && temp <= Max_Scope_Size && temp >= Min_Scope_Size)
                    break;
            }
        }
        return bestScope;
    }
    
}

bool endGame(char (*board)[BWIDTH], bool isMax){
    char piece;
    if(isMax)
        piece = 'C';
    else
        piece = 'P';
    for (int i = 0; i < BHEIGHT; i++)
        for (int j = 0; j < BWIDTH; j++)
            if(judgeAvailable(i, j, piece, board))
                return false;
    return true;
}

struct Move{
    int row, col;
};

int minimax(char (*board)[BWIDTH], int depth, bool isMax, int i1, int i2, int j1, int j2, int alpha, int beta){
    if(depth == Target_Depth)
        return evaluationFunc(board);

    if (endGame(board, isMax)){
        if(isMax)
            return Min_Win;
        else
            return Max_Win;
    }

    if (isMax){
        int best = -1000;
        for (int i = i1; i <= i2; i++){
            for (int j = j1; j <= j2; j++){
                if (judgeAvailable(i,j,'C',board)){
                    board[i][j] = 'C';
                    best = max(best, minimax(board, depth+1, !isMax, i1, i2, j1, j2, alpha, beta) );
                    alpha = max(alpha, best);
                    board[i][j] = ' ';
                    if(beta <= alpha)
                        break;
                }
            }
        }
        return best;
    }

    else{
        int best = 1000;
        for (int i = i1; i <= i2; i++){
            for (int j = j1; j <= j2; j++){
                if (judgeAvailable(i,j,'P',board)){
                    board[i][j] = 'P';
                    best = min(best, minimax(board, depth+1, !isMax, i1, i2, j1, j2, alpha, beta) );
                    beta = min(beta,best);
                    board[i][j] = ' ';
                    if(beta <= alpha)
                        break;
                }
            }
        }
        return best;
    }
}

Move findBestMove(char (*board)[BWIDTH], char *input){
    Scope fixedScope = findScope(board, input);
    int bestVal = -1000;
    Move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
    for (int i = fixedScope.i1; i < fixedScope.i2; i++){
        for (int j = fixedScope.j1; j < fixedScope.j2; j++){
            if (judgeAvailable(i,j,'C',board)){
                board[i][j] = 'C';
                int moveVal = minimax(board, 0, false,fixedScope.i1,fixedScope.j1,fixedScope.i2,fixedScope.j2, -1000, 1000);
                board[i][j] = ' ';

                if (moveVal > bestVal){
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}


///     Play Turns

int playerPlay(char (*board)[BWIDTH], char *input){
    while(true){
        if(input[0] == 'M'){
            ClearScreen();
            if(Menu(board) == 1)
                return 0;
            else
                return 1;
        }
        else if(input[0] == '('){
            if(input[1]-'0' >= BHEIGHT || input[3]-'0' >= BWIDTH){
                cout << "Invalid move, please retry" << endl;
                return 1;
            }
            if(judgeAvailable(input[1]-'0', input[3]-'0', 'P', board)){
                board[input[1]-'0'][input[3]-'0'] = 'P';
                return 2;
            }
            else{
                cout << "Invalid move, please retry" << endl;
                return 1;
            }
        }
        else{
            cout << "Invalid Command. Type M for Menu. Type Coordinates to Play." << endl;
            return 1;
        }
    }
}

int computerPlay(char (*board)[BWIDTH], char *input){
    if(endGame(board, true)){
        cout << "You Win";
        return 0;
    }
    Move bestMove = findBestMove(board, input);
    board[bestMove.row][bestMove.col] = 'C';
    return 0;
}


///     Driver Function

int main(){
    
    welcomeMessage();
    char board[BHEIGHT][BWIDTH];
    memset(board, ' ', sizeof(board[0][0]) * BHEIGHT*BWIDTH);
    drawBoard(board);
    
    
    char input[100];
    int turn = 1;
    while(true){
        
        // player's turn
        if(endGame(board, false)){
            cout << "You Lose";
            return 0;
        }
        cin >> input;
        if(turn == 1 && input[1] == '4' && input[3] == '4'){
            cout << "Invalid move, please retry" << endl;
            continue;
        }
        int temp = playerPlay(board, input);
        if(temp == 0)
            return 0;
        if(temp == 1)
            continue;
        
        // computer's turn
        computerPlay(board, input);
        
        
        ClearScreen();
        drawBoard(board);
        memset(input, '\0', sizeof(input[0]) * 100);
        turn++;
    }
    return 0;
}




