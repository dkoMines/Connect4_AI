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
#include <utility>

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
    bool gameOver(){
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
        countScore();
        float weighting[6] = {1,.45,.1}; // This is the weighting of our utilities
        // First  is straight up 4 in a rows
        // Second  is 3/4 in a row threats
        // Third  is 2/4 threats
        pair<float,float> finalScore;
        finalScore.first = 0;
        finalScore.second = 0;
        finalScore.first += player1Score;
        finalScore.second += player2Score;

        // Additional ways to build scores
        // Check threat cells ~ if can get 4 in a row. 
        // Horizontal Utility
        for (int pieceToCheck = 1; pieceToCheck < 3; pieceToCheck++){
            float utilityAdd = 0;
            for (int i=0;i<MAX_ROWS-3;i++){
                float countPoss = 0;
                float countPieces = 0;
                for (int j=0;j<MAX_COLS-3;j++){
                    int pieces = 0;
                    for (int count = 0;count<4;count++){
                        if (board[i][j+count]==pieceToCheck){
                            pieces++;
                        } else if (board[i][j+count]!=0){
                            pieces = -10;
                        } else {
                            // Need to check pieces below for 0's
                            if (i==0 || board[i-1][j]!=0){
                                // There is a supporting piece
                            } else {
                                pieces = -10;
                            }
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
        // Vertical Utility
        for (int pieceToCheck = 1; pieceToCheck < 3; pieceToCheck++){
            float utilityAdd = 0;
            for (int j=0;j<MAX_COLS;j++){
                int pieceCount = 0;
                int piecesLeft = 0;
                for (int i=0;i<MAX_ROWS;i++){
                    if (board[i][j]==pieceToCheck){
                        pieceCount ++;
                    } else if (board[i][j] != 0){
                        pieceCount = 0;
                    } else {
                        piecesLeft ++;
                    }
                }
                if (pieceCount==2 && piecesLeft >= 2){
                    utilityAdd += weighting[2]; // 2 in a row threat
                } else if (pieceCount==3 && piecesLeft >= 1){
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
                        } else if (board[i+count][j+count]!=0){
                            pieces = -10;
                        } else {
                            if (i+count==0 || board[i+count-1][j+count]!=0){
                                // There is a supporting piece
                            } else {
                                pieces = -10;
                            }
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
                        } else if (board[i+count][j-count]!=0){
                            pieces = -10;
                        } else {
                            if (i+count==0 || board[i+count-1][j-count]!=0){
                                // There is a supporting piece
                            } else {
                                pieces = -10;
                            }
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
pair<int,float> minimax(int depthLeft, int playerTurn, Connect4* myGame, bool max){
    vector<pair<int,float>> utilityTracker; // For the pair, col num, then it is the utility.
    depthLeft --;
    float utility;
    // Check the cols
    for (int j=0;j<MAX_COLS;j++){
        if (!myGame->filled(j)){ // It is possible to go here, lets try it out
            Connect4* newGame = new Connect4();
            newGame->copyBoard(myGame);
            newGame->placePiece(j,playerTurn);
            if (depthLeft>0&&!newGame->gameOver()){
                utility = minimax(depthLeft, flipPlayer(playerTurn) ,newGame, max).second;
            } else if(depthLeft>0 && newGame->gameOver()){
                newGame->countScore();
                utility = newGame->getPlayer1Score()-newGame->getPlayer2Score();
            }else if (depthLeft==0){
                // Utility is calculated at the leaf
                // Player1 up is positive, Player2 up is negative
                utility = newGame->evaluationFunction();
                // cout << "Col: "<< j+1 <<" Utility: " << utility << endl;

            }
            pair<int,float> colUtil;
            colUtil.first = j;
            colUtil.second = utility;
            utilityTracker.push_back(colUtil);
        }
    }
    // Find our best row utility
    bool highNum;
    float maximizedUtil;
    int bestIndex;
    if ((max && playerTurn==1) || (!max && playerTurn==2)){
        // We want utility to be as high as possible
        highNum = true;
        maximizedUtil = INT8_MAX*-1;
    } else {
        // We want utility to be as low as possible
        highNum = false;
        maximizedUtil = INT8_MAX;
    }
    for (int i=0;i<utilityTracker.size();i++){
        if (highNum){ // Want 1 to be maximized
            if (utilityTracker[i].second > maximizedUtil){
                maximizedUtil = utilityTracker[i].second;
                bestIndex = i;
            }
        } else {    // Want 2 to be maximized
            if (utilityTracker[i].second < maximizedUtil){
                maximizedUtil = utilityTracker[i].second;
                bestIndex = i;
            }
        }
    }

    return utilityTracker[bestIndex];

}

int cpuPlays(int depth, int playerTurn, Connect4* myGame){
    pair<int,float> getMinimax = minimax(depth,playerTurn,myGame, true);
    // cout << "Best utility: " << getMinimax.second << endl;
    return getMinimax.first;

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

    

    // ======================= one-move ====================================
    if (gameMode == 1){
        // 1) Read input file and initialize board state and score ( Done above )

         // 2) Print current board state and score and end if full
        myGame->printBoard();
        if (myGame->gameOver()) {
            cout << "Board is full. Game Over" << endl;
            goto programEnd;
        }

        // 3) Cpu makes its next move
        cpuPlays(depth,playerTurn,myGame);

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
        if (myGame->gameOver()) {
            cout << "Board is full. Game Over" << endl;
            goto programEnd;
        }

        // Step 3) Computer Chooses next move
        myGame->placePiece(cpuPlays(depth,playerTurn,myGame),playerTurn);

        // Step 4) Save board state in computer.txt
        playerTurn = flipPlayer(playerTurn);
        myGame->writeBoardtoFile("computer.txt",playerTurn);
        humanTurn = true;

step5:
        // Step 5) print game board and score. Exit if board is full
        
        myGame->printBoard();
        if (myGame->gameOver()) {
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

