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

//Call with superBoardSpot = 0 if testing superBoard
char boardWon(char subBoard[], char superBoardSpot) {
  char seed = superBoardSpot * 9;

  //Diagonal left
  if (subBoard[seed] > 0 && subBoard[seed] == subBoard[seed + 4] && subBoard[seed] == subBoard[seed + 8]) {
    return subBoard[seed];
  }

  //Diagonal right
  if (subBoard[seed + 2] > 0 && subBoard[seed + 2] == subBoard[seed + 4] && subBoard[seed + 2] == subBoard[seed + 6]) {
    return subBoard[seed + 2];
  }

  //Rows
  for (char i = seed; i < 9; i += 3) {
    if (subBoard[i] > 0 && subBoard[i] == subBoard[i + 1] && subBoard[i] == subBoard[i + 2]) {
      return subBoard[i];
    }
  }

  //Columns
  for (char i = seed; i < 3; i += 1) {
    if (subBoard[i] > 0 && subBoard[i] == subBoard[i + 3] && subBoard[i] == subBoard[i + 6]) {
      return subBoard[i];
    }
  }

  //Test for any empty spots, to detemine of its a tie
  for (char i = seed; i < seed + 9; i += 1) {
    if (subBoard[i] == 0) {
      //Not a tie
      return -1;
    }
  }

  //Its a tie
  return 0;
}

//Perform a move
char doMove(char subBoard[], char superBoard[], char player, char move) {
  //copy the board
  subBoard[move] = player;

  //Update the super board spot if needed
  char superBoardSpot = getSuperBoardSpot(move);
  char state = boardWon(subBoard, superBoardSpot);
  if (state > -1) {
    superBoard[superBoardSpot] = state;
  }

  char nextSuperBoardSpot = move % 9;
  if (boardWon(subBoard, nextSuperBoardSpot) > -1) {
    //Super board is won, so the next player gets to go anywhere
    return -1;
  }

  //Return the super board spot that the next player must move into
  return nextSuperBoardSpot;
}

char isOpenSpot(char subBoard[], char move) {
  if (subBoard[move] > 0) {
    return 0;
  } else {
    return 1;
  }
}

// char minimax(char subBoard[], char superBoard[], char superBoardSpot, char goal, char opPlayer, char level) {
//   char gameOver = boardWon(superBoard, 0);
//   char start, end;
//   char player;
//   long retVal;
//
//   if (gameOver == 0) {
//     return 0;
//   } else if (gameOver )
//
//   if (--level == 0) {
//     //evaluate the heurstic of the subboard and return that as the value
//     getScore(subBoard[], player);
//   } else {
//
//   }
//
//   //We need to go deeper
//   if (superBoardSpot == -1) {
//     //search all spots on the board
//     start = 0;
//     end = SUB_BOARD_SIZE;
//   } else {
//     start = superBoardSpot * 9;
//     end = start + 9;
//   }
//
//   //setup the minimizer/maximizer
//   if (goal == MAXIMIZE) {
//     retVal = -9999999999;
//   } else {
//     retVal = 9999999999;
//   }
//
//   //Choose the player based on whether we are maximizing or minimizing
//   if (goal == MAXIMIZE) {
//     if (opPlayer == X) {
//       player = X;
//     } else {
//       player = O;
//     }
//   } else {
//     if (opPlayer == X) {
//       player = O;
//     } else {
//       player = X;
//     }
//   }
//
//   for (int i=start; i < end; i++) {
//     if (isOpenSpot(subBoard, i)) {
//       char newSuperBoardSpot;
//       long result;
//
//       newSuperBoardSpot = doMove(subBoard, superBoard, player, i);
//       result = minimax(subBoard, superBoard, newSuperBoardSpot, goal == MAXIMIZE ? MINIMIZE : MAXIMIZE, opPlayer, level);
//       undoMove(subBoard, superBoard, i);
//       if (goal == MAXIMIZE) {
//          if (result > retVal) {
//            retVal = result;
//          }
//       } else {
//         if (result < retVal) {
//           retVal = result;
//         }
//       }
//     }
//   }
//   return retVal;
// }
//
// char getBestMove(char subBoard[], char superBoard[], char superBoardSpot, char opPlayer, char levels) {
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
//   for (char i = start; i < end; i++) {
//     if (isOpenSpot(subBoard, i)) {
//       char newSuperBoardSpot = doMove(subBoard, superBoard, player, i);
//       long result = minimax(subBoard, superBoard, newSuperBoardSpot, MINIMIZE, opPlayer, levels);
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
  //rows
  for (char seed = 0; seed < 81; seed += 27) {
     for (char pp = seed; pp < seed + 9;  pp+=3) {
           //columns
          for (int i = pp; i < seed + 27; i+= 9) {
            for (int j = i; j < i + 3; j++) {

              if (j < 10) {
                if (subBoard[j] == X) {
                  printf("%c  ", 'X');
                } else if (subBoard[j] == O) {
                  printf("%c  ", 'O');
                } else {
                  printf("%d  ", j);
                }
              } else {
                if (subBoard[j] == X) {
                  printf("%c  ", 'X');
                } else if (subBoard[j] == O) {
                  printf("%c  ", 'O');
                } else {
                  printf("%d ", j);
                }
              }
            }
            printf("| ");
          }
          printf("\n");
     }
     if (seed != 54) {
       printf("--------------------------------\n");
     }
   }




   return;


   for (char i=0; i < SUB_BOARD_SIZE; i++) {
     if (i % 9 == 0) {
       printf("\n");
     }

     if (i % 3 == 0 && i != 0 && i % 9 != 0) {
       printf("%c ", '|');
     }

     if (i % 27 == 0 && i != 0) {
       printf("%s", "------------------------------\n");
     }


     if (i < 10) {
       if (subBoard[i] == X) {
         printf("%c  ", 'X');
       } else if (subBoard[i] == O) {
         printf("%c  ", 'O');
       } else {
         printf("%d  ", i);
       }
     } else {
       if (subBoard[i] == X) {
         printf("%c  ", 'X');
       } else if (subBoard[i] == O) {
         printf("%c  ", 'O');
       } else {
         printf("%d ", i);
       }
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
