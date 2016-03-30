#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUB_BOARD_SIZE 81
#define SUPER_BOARD_SIZE 9
#define blank 0
#define X 1
#define O 2
#define MAXIMIZE 0
#define MINIMIZE 1

char subboard[81];
char superboard[9];
short currentPlayer = X;

//Get the region of the super board the a move is in
char getSuperBoardSpot(char move) {
  char numberOfSpotsIntoSubBoard = move % 9;
  char firstSpotOfSubBoard =  move - numberOfSpotsIntoSubBoard;
  return firstSpotOfSubBoard / 9;
}

//Remove a move from the board
void undoMove(char subboard[], char superboard[], char move) {
  subboard[move] = 0;
  char superBoardSpot = getSuperBoardSpot(move);
  superboard[superBoardSpot] = 0;
}

//Perform a move
void doMove(char subboard[], char superboard[], char player, char move) {
  //copy the board
  subboard[move] = player;

  // if (isSubWon()) {
  //   superboard[move % 9] = player;
  // }
}

char *copyBoard(char board[], char size) {
  char newBoard[size];
  memcpy(newBoard, board, size * sizeof(char));
  return newBoard;
}

void printBoard(char subboard[]) {
   for (char i=0; i < SUB_BOARD_SIZE; i++) {
     if (i % 9 == 0) {
       printf("\n");
     }
     if (subboard[i] == X) {
       printf("%c ", 'X');
     } else if (subboard[i] == O) {
       printf("%c ", 'O');
     } else {
       printf("%c ", '0');
     }
   }
}

int main(void) {
  char inputMove;
  char gameOver;

  while (1) {
    printBoard(subboard);

    if (currentPlayer == O) {
      printf("\nEnter move: ");
      scanf("%d", &inputMove);
    } else {
      printf("\nEnter move: ");
      scanf("%d", &inputMove);
    }

    // if (isValidMove(board, move)) {
    //   board = makeMove(board, currentPlayer, move);
    //   if (currentPlayer == X) {
    //     currentPlayer = O;
    //   } else {
    //     currentPlayer = X;
    //   }
    // } else {
    //   printf("\nInvalid Move.\n");
    // }

    // gameOver = isGameOver(board);
    // if (gameOver > -1) {
    //   printBoard(board);
    //   printf("\nGame Over: %d\n", gameOver);
    //   exit(0);
    // }

  }
}
