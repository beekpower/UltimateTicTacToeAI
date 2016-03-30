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

char subBoard[81];
char superBoard[9];
char AI;
short currentPlayer = X;

//Copy a sub or super board
char *copyBoard(char board[], char size) {
  char newBoard[size];
  memcpy(newBoard, board, size * sizeof(char));
  return newBoard;
}

//Get the region of the super board the a move is in
char getSuperBoardSpot(char move) {
  char numberOfSpotsIntoSubBoard = move % 9;
  char firstSpotOfSubBoard =  move - numberOfSpotsIntoSubBoard;
  return firstSpotOfSubBoard / 9;
}

//Remove a move from the board
void undoMove(char subBoard[], char superBoard[], char move) {
  subBoard[move] = 0;
  char superBoardSpot = getSuperBoardSpot(move);
  superBoard[superBoardSpot] = 0;
}

//Perform a move
char doMove(char subBoard[], char superBoard[], char player, char move) {
  //copy the board
  subBoard[move] = player;

  //Update the super board spot if needed
  char numberOfSpotsIntoSubBoard = move % 9;
  char seed =  move - numberOfSpotsIntoSubBoard;

  //Test Row 1
  if (subBoard[seed] > 0 && subBoard[seed] == subBoard[seed + 3] && subBoard[seed] == subBoard[seed + 3]) {
    superBoard[seed / 9] = player;
  }

  //Return the super board spot that the next player must move into
  return numberOfSpotsIntoSubBoard; //or -1 if superboard is won
}

char isOpenSpot(char subBoard[], char move) {
  if (subBoard[move] > 0) {
    return 0;
  } else {
    return 1;
  }
}

// char minimax(char subBoard[], char superBoard[], char superBoardSpot, char goal, char player, char level) {
//   if (gameMover)
//
//   if (--level == 0) {
//     //evaluate the heurstic of the subboard and return that as the value
//     getScore(subBoard[], player);
//   } else {
//
//   }
//
//
// }
//
// char getBestMove(char subBoard[], char superBoard[], char superBoardSpot, char player, char levels) {
//   long best = -9999999999;
//   char move;
//   char start, end;
//
//   if (superBoardSpot == -1) {
//     //search all spots on the board
//     start = 0;
//     end = SUB_BOARD_SIZE;
//   } else {
//     start = superBoardSpot * 9;
//     end = start + 9;
//   }
//
//   //search within the superboard
//   char seed =
//   for (char i = start; i < end; i++) {
//     if (isOpenSpot(subBoard, i)) {
//       char newSuperBoardSpot = doMove(subBoard, superBoard, i);
//       long result = minimax(subBoard, superBoard, newSuperBoardSpot, MINIMIZE, player, levels);
//       undoMove(subBoard, superBoard, i);
//
//       if (result > best) {
//         best = result;
//         move = i;
//       }
//     }
//   }
//
//   return move;
// }

//Print the UTTT board
void printBoard(char subBoard[]) {
   for (char i=0; i < SUB_BOARD_SIZE; i++) {
     if (i % 9 == 0) {
       printf("\n");
     }
     if (subBoard[i] == X) {
       printf("%c ", 'X');
     } else if (subBoard[i] == O) {
       printf("%c ", 'O');
     } else {
       printf("%c ", '0');
     }
   }
}

int main(void) {
  char inputMove, inputAI;

  while (1) {
    printf("\nWho will the AI play as? (X, O): ");
    scanf("%c", &inputAI);
    if (inputAI == 'X' || inputAI == 'x') {
      AI = X;
      break;
    } else if (inputAI == 'O' || inputAI == 'o') {
      AI = O;
      break;
    }

    //If we made it this far, invalid input
    printf("\nInvalid Selection\n");
  }

  while (1) {
    printBoard(subBoard);

    if (AI == currentPlayer) {
      printf("\nAI calculating best move...\n");
      //inputMove = getBestMove();
    } else {
      printf("\nEnter move: ");
      scanf("%d", &inputMove);
    }

  //  doMove();

    if (currentPlayer == X) {
      currentPlayer = O;
    } else {
      currentPlayer = X;
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
