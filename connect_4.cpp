// connect_4.cpp

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;


const int MAX_ROWS = 6;
const int MAX_COLS = 7;

int flipPlayer(int playerTurn){
    if (playerTurn==1) return 2;
    return 1;
}

class Connect4{
    /**
     * Game is set up on a double array board
     *     board[ ROW NUM (up and down) 0 is bottom ] [ COL NUM (left and right) 0 is left]
     * 
     **/

private:

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

    int getPiece(int i, int j){
        return board[i][j];
    }
    int getPlayer1Score(){
        return player1Score;
    }
    int getPlayer2Score(){
        return player2Score;
    }

    // Copies board from another game
    void copyBoard(Connect4* otherGame){
        for (int i=0;i<MAX_ROWS;i++){
            for (int j=0;j<MAX_COLS;j++){
                board[i][j] = otherGame->getPiece(i,j);
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

    void writeBoardtoFile(string fileName, int playerTurn){
        ofstream outFile;
        outFile.open(fileName);
        for (int i=MAX_ROWS-1;i>=0;i--){
            string line = "";
            for (int j=0;j<MAX_COLS;j++){
                stringstream ss;
                ss << board[i][j];
                string s;
                ss >> s;
                line = line + s;
            }
            outFile << line << '\n';
        }
        outFile << playerTurn;
        outFile.close();
    }

    float evaluationFunction(){ // This will give a utility 
        float weighting[6] = {1,.5,.25}; // This is the weighting of our utilities
        // First  is straight up 4 in a rows
        // Second  is 3/4 in a row threats
        // Third  is 2/4 threats
        pair<float,float> finalScore;
        float p1Eval = player1Score;
        float p2Eval = player2Score;
        finalScore.first = player1Score*weighting[0];
        finalScore.second = player2Score*weighting[0];

        // Additional ways to build scores
        // Check threat cells ~ if can get 4 in a row. 
        // Horizontal Utility
        for (int pieceToCheck = 1; pieceToCheck < 3; pieceToCheck++){
            float utilityAdd = 0;
            for (int i=0;i<MAX_ROWS;i++){
                float countPoss = 0;
                float countPieces = 0;
                for (int j=0;j<MAX_COLS;j++){
                    if (board[i][j]==pieceToCheck){
                        countPieces ++;
                    } else if (board[i][j] == 0){
                        countPoss ++;
                    } else {
                        // Enemy Piece is blocking. Reset counters
                        if (countPoss >=4 && countPieces==2){ // There is a threat filled by 2/4 pieces
                            utilityAdd += weighting[2];
                        } else if (countPoss >=4 && countPieces==3){ // There is a threat filled by 3/4 pieces
                            utilityAdd += weighting[1];
                        } else if (countPoss >=4 && countPieces>=4 && countPoss>countPieces){  // This should be a similar threat as 3 threat as it could be 4 that goes to 5 ~ +1 points
                            utilityAdd += weighting[1];
                        }
                        countPoss = 0;
                        countPieces = 0;
                    }
                }
            }
            if (pieceToCheck==1){
                finalScore.first += utilityAdd;
            } else {
                finalScore.second += utilityAdd;
            }
        }
        // Vertical Utility
        for (int pieceToCheck = 1; pieceToCheck < 3; pieceToCheck++){
            float utilityAdd = 0;
            for (int j=0;j<MAX_COLS;j++){
                int pieceCount = 0;
                for (int i=0;i<MAX_ROWS;i++){
                    if (board[i][j]==pieceToCheck){
                        pieceCount ++;
                    } else if (board[i][j] != 0){
                        pieceCount = 0;
                    }
                }
                if (pieceCount==2){
                    utilityAdd += weighting[2]; // 2 in a row threat
                } else if (pieceCount==3){
                    utilityAdd += weighting[1]; // 3 in a row threat
                }
            }
            if (pieceToCheck==1){
                finalScore.first += utilityAdd;
            } else {
                finalScore.second += utilityAdd;
            }
        }
        // Diagonal Utility
        // ~ Bottom Left to Top Right
        for (int pieceToCheck = 1; pieceToCheck < 3; pieceToCheck++){
            float utilityAdd = 0;
            for (int i=0;i<MAX_ROWS-3;i++){
                for (int j=0;j<MAX_COLS-3;j++){
                    int pieces = 0;
                    for (int count = 0;count<4;count++){
                        if (board[i+count][j+count]==pieceToCheck){
                            pieces++;
                        } else if (board[i+count][j+count]!=pieceToCheck){
                            pieces = -10;
                        }
                    }
                    if (pieces==2){
                        utilityAdd += weighting[2]; // 2/4 threats
                    } else if (pieces==3){
                        utilityAdd += weighting[1]; // 3/4 threats
                    }
                }
            }
            // ~ Bottom Right to Top Left
            for (int i=0;i<MAX_ROWS-3;i++){
                for (int j=MAX_COLS-1;j>=3;j--){
                    int pieces = 0;
                    for (int count = 0;count<4;count++){
                        if (board[i+count][j-count]==pieceToCheck){
                            pieces++;
                        } else if (board[i+count][j-count]!=pieceToCheck){
                            pieces = -10;
                        }
                    }
                    if (pieces==2){
                        utilityAdd += weighting[2]; // 2/4 threats
                    } else if (pieces==3){
                        utilityAdd += weighting[1]; // 3/4 threats
                    }
                }
            }
            if (pieceToCheck==1){
                finalScore.first += utilityAdd;
            } else {
                finalScore.second += utilityAdd;
            }
        }


        return finalScore.first - finalScore.second;
    }
};

// This will help us determine where the player should go to next. Pair is returned as col, utility
pair<int,int> minimax(int depthLeft, int playerTurn, Connect4* myGame, bool max){
    vector<pair<int,int>> utilityTracker; // For the pair, col num, then it is the utility.
    depthLeft --;
    int utility;
    // Check the cols
    for (int j=0;j<MAX_COLS;j++){
        Connect4* newGame = new Connect4();
        newGame->copyBoard(myGame);
        if (!newGame->filled(j)){
            // It is possible to go here
            newGame->placePiece(j,playerTurn);
            if (depthLeft>0){
                utility = minimax(depthLeft, flipPlayer(playerTurn) ,newGame, !max).second;
            } else if (depthLeft==0){
                // Gotta get those Utility numbers
                // Player1 up is positive, Player2 up is negative
                utility = newGame->evaluationFunction();
            }
            pair<int,int> colUtil;
            colUtil.first = j;
            colUtil.second = utility;
            utilityTracker.push_back(colUtil);
        }
    }
    // MMk, we have utility for all 7 rows. 
    return utilityTracker[0];





}

int cpuPlays(int depth, int playerTurn, Connect4* myGame){
    minimax(depth,playerTurn,myGame, true);

    
    return 1;
}





int main(int argc, char ** argv){
    // Important Vars
    Connect4* myGame = new Connect4();
    int playerTurn;
    int gameMode;
    bool humanTurn = false;
    string outputFile;


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
        outputFile = argv[3];        
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
    if (!inFile.is_open()){
        // Board is already set to 0's.
        playerTurn = 1;
    } else {
        // If file exists, replace the rows.
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
    }
    // Get Depth
    int depth = stoi(argv[4]);

    

    // one-move
    if (gameMode == 1){
        // 1) Read input file and initialize board state and score ( Done above )

         // 2) Print current board state and score and end if full
        myGame->printBoard();
        if (myGame->canGo()) {
            cout << "Board is full. Game Over" << endl;
            goto programEnd;
        }

        // 3) Cpu makes its next move
        // TODO

        // 4) Print current board state and score
        myGame->printBoard();

        // 5) Save current board state to the output file 
        playerTurn = flipPlayer(playerTurn);
        myGame->writeBoardtoFile(outputFile,playerTurn);

        // 6) Exit
        goto programEnd;
    }





    // ========================== Interactive mode ===================================
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
        // TODO

        // Step 4) Save board state in computer.txt
        playerTurn = flipPlayer(playerTurn);
        myGame->writeBoardtoFile("computer.txt",playerTurn);
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
        playerTurn = flipPlayer(playerTurn);
        myGame->writeBoardtoFile("human.txt",playerTurn);
        humanTurn = false;
        
        // Step 8) Goto 2
        goto step2;
    }


programEnd:

    return 0;


}

