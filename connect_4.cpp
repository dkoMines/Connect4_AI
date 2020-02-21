// connect_4.cpp

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Connect4{
    /**
     * Game is set up on a double array board
     *     board[ ROW NUM (up and down) 0 is bottom ] [ COL NUM (left and right) 0 is left]
     * 
     **/

private:

    const int MAX_ROWS = 6;
    const int MAX_COLS = 7;

    int board[6][7];
    int player1Score = 0;
    int player2Score = 0;

public:
    Connect4(){
        for (int i=0;i<MAX_ROWS;i++){
            for (int j=0;j<MAX_COLS;j++){
                board[i][j] = 0;
            }
        }
    }

    // Prints the board
    void printBoard(){
        cout << " -----------------" << endl;
        for (int i=MAX_ROWS-1;i>=0;i--){
            cout << " | ";
            for (int j=0;j<MAX_COLS;j++){
                cout << board[i][j] << " ";
            }
            cout << " | " << endl;
        }
        cout << " -----------------" << endl;
        cout << endl;
        countScore();
        cout << "Player 1 Score: " << player1Score << endl;
        cout << "Player 2 Score: " << player2Score << endl;
    }

    // Used for generating board from input file
    void replaceRow(string row, int countRow){
        int z;
        int countCol=0;
        for (char x : row){
            z = x-48;
            board[MAX_ROWS-1-countRow][countCol] = z;
            countCol ++;
        }
    }

    // Will return false if all 42 pieces are filled
    bool canGo(){
        for (int i=0;i<MAX_COLS;i++){
            if (!filled(i)){
                return false;
            }
        }
        return true;
    }

    // Returns if colNum is filled
    bool filled(int colNum){
        if (board[MAX_ROWS-1][colNum]!=0){
            return true;
        }
        return false;
    }

    // Puts a piece down. 
    bool placePiece(int colNum, int playerNum){
        if (filled(colNum)) return false;
        for (int i=0;i<MAX_ROWS;i++){
            if (board[i][colNum]==0){
                board[i][colNum] = playerNum;
                return true;
            }
        }
        return false;
    }

    // Calculates both players scores
    void countScore(){
        player1Score = 0;
        player2Score = 0;
        // Horizontal Count
        for (int i=0;i<MAX_ROWS;i++){
            for (int j=0;j<MAX_COLS-3;j++){
                if (board[i][j]==1 && board[i][j+1]==1 && board[i][j+2]==1 && board[i][j+3]==1){
                    player1Score ++;
                }
                if (board[i][j]==2 && board[i][j+1]==2 && board[i][j+2]==2 && board[i][j+3]==2){
                    player2Score ++;
                }
            }
        }
        // Vertical Count
        for (int j=0;j<MAX_COLS;j++){
            for (int i=0;i<MAX_ROWS-3;i++){
                if (board[i][j]==1 && board[i+1][j]==1 && board[i+2][j]==1 && board[i+3][j]==1){
                    player1Score ++;
                }
                if (board[i][j]==2 && board[i+1][j]==2 && board[i+2][j]==2 && board[i+3][j]==2){
                    player2Score ++;
                }
            }
        }
        // Diagonal Count
        // ~ Bottom Left to Top Right
        for (int i=0;i<MAX_ROWS-3;i++){
            for (int j=0;j<MAX_COLS-3;j++){
                if (board[i][j]==1 && board[i+1][j+1]==1 && board[i+2][j+2]==1 && board[i+3][j+3]==1){
                    player1Score ++;
                }
                if (board[i][j]==2 && board[i+1][j+1]==2 && board[i+2][j+2]==2 && board[i+3][j+3]==2){
                    player2Score ++;
                }
            }
        }
        // ~ Bottom Right to Top Left
        for (int i=0;i<MAX_ROWS-3;i++){
            for (int j=MAX_COLS-1;j>=3;j--){
                if (board[i][j]==1 && board[i+1][j-1]==1 && board[i+2][j-2]==1 && board[i+3][j-3]==1){
                    player1Score ++;
                }
                if (board[i][j]==2 && board[i+1][j-1]==2 && board[i+2][j-2]==2 && board[i+3][j-3]==2){
                    player2Score ++;
                }
            }
        }

    }


};


int main(int argc, char ** argv){
    // Important Vars
    Connect4* myGame = new Connect4();
    ofstream outFile;
    int playerTurn;
    int gameMode;
    bool humanTurn = false;


    // Command Line
    if (argc != 5){
wrongArgs:
        cout << "Four command-line arguments are needed: " << endl;
        cout << "Usage " << argv[0] << " interavtive [input file] [computer-next/human-next] [depth]" << endl;
        cout << "or "<< argv[0] << " one-move [input_file] [output_file] [depth]" << endl;
        return 0;
    } 
    // Game Mode
    string gamemode_str = argv[1];
    if (gamemode_str=="one-move"){
        gameMode = 1;
        
    } else if (gamemode_str=="interactive"){
        gameMode = 2;
        string next_str = argv[3];
        if (next_str=="human-next"){
            humanTurn = true;
        } else if (next_str=="computer-next"){
            humanTurn = false;
        } else {
            goto wrongArgs;
        }
    } else {
        cout << "Game mode is either 'one-move' or 'interactive' " << endl;
        return 0;
    }
    // Input File
    ifstream inFile;
    inFile.open(argv[2]);
    string line;
    int countRow = 0;
    while (getline(inFile, line)){
        if (countRow<6){
            myGame->replaceRow(line,countRow);
        } else {
            playerTurn = stoi(line);
        }
        countRow++;
    }
    inFile.close();
    // Open Output File
    // outFile.open(argv[3]);
    // Get Depth
    int depth = stoi(argv[4]);

    

    // one-move
    if (gameMode == 1){
        // 1) Read input file and initialize board state and score

        // 2) Print current board state and score and end if full
            myGame->printBoard();
            if (myGame->canGo()) {
                cout << "Board is full. Game Over" << endl;
                goto programEnd;
            }

        // 3) Cpu makes its next move

        // 4) Print current board state and score

        // 5) Save current board state to the output file 

        // 6) Exit
        goto programEnd;
    }





    // Interactive mode
    if (gameMode == 2){
        // Step 1) If Computer goes first, go to 2, else go to 5
        if (humanTurn) goto step5;

step2:
        // Step 2) print game board and score. Exit if board is full
        
        myGame->printBoard();
        if (myGame->canGo()) {
            cout << "Board is full. Game Over" << endl;
            goto programEnd;
        }

        // Step 3) Computer Chooses next move


        // Step 4) Save board state in computer.txt

        humanTurn = true;

step5:
        // Step 5) print game board and score. Exit if board is full
        
        myGame->printBoard();
        if (myGame->canGo()) {
            cout << "Board is full. Game Over" << endl;
            goto programEnd;
        }
        // Step 6) Allow user to go
        while (true){
            string colToGo_str;
            cout << "Choose a column (1-7): ";
            getline(cin,colToGo_str);
            int colToGo = stoi(colToGo_str);
            if (colToGo >= 1 && colToGo <= 7 && myGame->placePiece(colToGo-1,playerTurn)){
                break;
            } else {
                cout << "That is not a valid row. Please try again" << endl;
            }
        }

        // Step 7) Save state as human.txt

        humanTurn = false;
        

        // Step 8) Goto 2
        goto step2;
    }


programEnd:

    return 0;


}

