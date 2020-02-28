Dylan Ko - 10788700
Chris Cathcart - 10808241
Heather Keniry - 10782597

Created in c++

Used windows 10

Instuctions for running in COMMAND LINE: 

Make: g++ -o connect_4 connect_4.cpp
Compile Examples: 
        ./connect_4 interactive input1.txt computer-next 4
        ./connect_4 one-move input1.txt output1.txt 4

CODE STRUCTURE: 

In our code, we made separate functions for all the base functionality we needed: printBoard, calculateScore, placePiece, getPiece, getColumn, etc. All of these functions are the baseline for the game to work, this is NOT part of the AI or search algorithms. We also implemented both modes for use which are the one move and the interactive.  

The next section is our AI, where we implemented our alpha beta pruning min-max tree as our algorithm for the computer to compute the best move. We created 3 utility scores, (4 in a row, 3 in a row, and 2 in a row), where we calculated the final utility by getting our utility and subtracting it by the next player's utility. In this way, we could figure out for our move the best utility and for the next player's move the smallest utility and continue coming back up the tree. We set higher precedence for the middle columns of the board for the computer, and gave higher utility for 4 in a row, then 3 in a row, and then lastly 2 in a row. The 4 in a row utility is also much higher than the other two to give higher precedence in scoring / preventing points. 

That's all she wrote. :)
