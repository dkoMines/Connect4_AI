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

private:
    int board[6][7];
public:
    Connect4(){
        for (int i=0;i<6;i++){
            for (int j=0;j<7;j++){
                board[i][j] = 0;
            }
        }
    }

    void printBoard(){
        cout << " -----------------" << endl;
        for (int i=5;i>=0;i--){
            cout << " | ";
            for (int j=0;j<7;j++){
                cout << board[i][j] << " ";
            }
            cout << " | " << endl;
        }
        cout << " -----------------" << endl;
    }

    void replaceRow(string row, int countRow){
        int z;
        int countCol=0;
        for (char x : row){
            z = x-48;
            board[5-countRow][countCol] = z;
            countCol ++;
        }
    }

    bool canGo(){
        for (int i=0;i<7;i++){
            if (!filled(i)){
                return false;
            }
        }
        return true;
    }

    // Returns if colNum is filled
    bool filled(int colNum){
        if (board[5][colNum]!=0){
            return true;
        }
        return false;
    }

    bool placePiece(int colNum, int piece){
        if (filled(colNum)) return false;
        for (int i=0;i<6;i++){
            if (board[i][colNum]==0){
                board[i][colNum] = piece;
                return true;
            }
        }
        return false;
    }


};


int main(int argc, char ** argv){
    // Important Vars
    Connect4* myGame = new Connect4();
    ofstream outFile;
    int playerTurn;
    int gameMode;


    // Command Line
    if (argc != 5){
        cout << "Not all required arguments used";
        return 0;
    } 
    // Game Mode
    string gamemode_str = argv[1];
    if (gamemode_str=="one-move"){
        gameMode = 1;
    } else if (gamemode_str=="interactive"){
        gameMode = 2;
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

    







    // Interactive mode
    if (gameMode == 2){
        // Step 1) If Computer goes first, go to 2, else go to 5
        if (true) goto step5;

step2:
        // Step 2) print game board. Exit if board is full
        myGame->printBoard();
        if (myGame->canGo()) {
            cout << "Board is full. Game Over" << endl;
            goto programEnd;
        }

        // Step 3) Computer Chooses next move


        // Step 4) Save board state in computer.txt

step5:
        // Step 5) print game board. Exit if board is full
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
            if (colToGo >= 1 && colToGo <= 7 && myGame->placePiece(colToGo-1,2)){
                break;
            } else {
                cout << "That is not a valid row. Please try again" << endl;
            }
        }

        // Step 7) Save state as human.txt
        

        // Step 8) Goto 2
        goto step2;
    }


programEnd:

    return 0;


}

