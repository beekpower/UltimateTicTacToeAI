#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include "thpool.h"

#define MAX_TURN_TIME 154
#define THREADS 81
#define SUB_BOARD_SIZE 81
#define SUPER_BOARD_SIZE 9
#define blank 0
#define X 1
#define O 2
#define MAXIMIZE 0
#define MINIMIZE 1

#define SUPER_THREE 1000
#define SUPER_TWO 100
#define SUPER_ONE 10
#define SUB_TWO 1

struct timeval t1;
char subBoard[81] = {0};
char superBoard[9] = {0};
char AI;
char currentPlayer = X;
char allowedSuperSpot = -1;
int moves = 0;

typedef struct {
    char *subBoard;
    char *superBoard;
    char opPlayer;
    char levels;
    char move;
} ThreadData;

typedef struct {
  char move;
  long score;
} ThreadReturnData;

char numThreads;

ThreadData tData[81];
ThreadReturnData trData[81];

//Copy a sub or super board
char *copyBoard(char board[], char size) {
  char *newBoard = malloc(sizeof(char) * size);
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
  for (char i = seed; i < seed + 9; i += 3) {
    if (subBoard[i] > 0 && subBoard[i] == subBoard[i + 1] && subBoard[i] == subBoard[i + 2]) {
      return subBoard[i];
    }
  }

  //Columns
  for (char i = seed; i < seed + 3; i += 1) {
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
  //make the move
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

char isOpenSpot(char subBoard[], char superBoard[], char move) {
  char superBoardSpot = getSuperBoardSpot(move);
  if (superBoard[superBoardSpot] > 0) {
    return 0;
  }

  if (subBoard[move] > 0) {
    return 0;
  } else {
    return 1;
  }
}

//Evaluate a heuristic for a subboard
long subHeuristic(char subBoard[], char board, char player) {
  char seed = board * 9;
  long total = 0;
  //Diagonal left
  if (subBoard[seed] > 0 && subBoard[seed] == subBoard[seed+4]) {
    if (subBoard[seed] == player) {
      total += SUB_TWO;
    } else {
      total -= SUB_TWO;
    }
  }

  if (subBoard[seed] > 0 && subBoard[seed] == subBoard[seed+8]) {
    if (subBoard[seed] == player) {
      total += SUB_TWO;
    } else {
      total -= SUB_TWO;
    }
  }

  if (subBoard[seed+4] > 0 && subBoard[seed+4] == subBoard[seed+8]) {
    if (subBoard[seed+4] == player) {
      total += SUB_TWO;
    } else {
      total -= SUB_TWO;
    }
  }

  //Diagonal right
  if (subBoard[seed+2] > 0 && subBoard[seed+2] == subBoard[seed+4]) {
    if (subBoard[seed+2] == player) {
      total += SUB_TWO;
    } else {
      total -= SUB_TWO;
    }
  }

  if (subBoard[seed+2] > 0 && subBoard[seed+2] == subBoard[seed+6]) {
    if (subBoard[seed+2] == player) {
      total += SUB_TWO;
    } else {
      total -= SUB_TWO;
    }
  }

  if (subBoard[seed+4] > 0 && subBoard[seed+4] == subBoard[seed+6]) {
    if (subBoard[seed+4] == player) {
      total += SUB_TWO;
    } else {
      total -= SUB_TWO;
    }
  }

  //Rows
  for (char i = seed; i < seed + 9; i += 3) {
    if (subBoard[i] > 0 && subBoard[i] == subBoard[i+1]) {
      if (subBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (subBoard[i] > 0 && subBoard[i] == subBoard[i+2]) {
      if (subBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (subBoard[i+1] > 0 && subBoard[i+1] == subBoard[i+2]) {
      if (subBoard[i+1] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }
  }

  //Columns
  for (char i = seed; i < seed + 3; i += 1) {
    if (subBoard[i] > 0 && subBoard[i] == subBoard[i+3]) {
      if (subBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (subBoard[i] > 0 && subBoard[i] == subBoard[i+6]) {
      if (subBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (subBoard[i+3] > 0 && subBoard[i+3] == subBoard[i+6]) {
      if (subBoard[i+3] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }
  }

  return total;
}

//Evaluate the super board heuristic;
long heuristic(char subBoard[], char superBoard[], char player) {
  long total = 0;

  //Check the super board
  //individual spots
  for (char i = 0; i < SUPER_BOARD_SIZE; i++) {
    if (superBoard[i] == player) {
      //If it is player's spot
      if (i == 4) {
        //if its the center board that is won, add more points
        total += SUPER_ONE;
      }
      total += SUPER_ONE;
    } else if (superBoard[i] > 0) {
      //If it is opponent's spot
      if (i == 4) {
        //if its the center board that is won, subtract more points
        total -= SUPER_ONE;
      }
      total -= SUPER_ONE;
    }
  }

  //Diagonal left
  if (superBoard[0] > 0 && superBoard[0] == superBoard[4]) {
    if (superBoard[0] == player) {
      total += SUPER_TWO;
    } else {
      total -= SUPER_TWO;
    }
  }

  if (superBoard[0] > 0 && superBoard[0] == superBoard[8]) {
    if (superBoard[0] == player) {
      total += SUPER_TWO;
    } else {
      total -= SUPER_TWO;
    }
  }

  if (superBoard[4] > 0 && superBoard[4] == superBoard[8]) {
    if (superBoard[4] == player) {
      total += SUPER_TWO;
    } else {
      total -= SUPER_TWO;
    }
  }

  //Diagonal right
  if (superBoard[2] > 0 && superBoard[2] == superBoard[4]) {
    if (superBoard[2] == player) {
      total += SUPER_TWO;
    } else {
      total -= SUPER_TWO;
    }
  }

  if (superBoard[2] > 0 && superBoard[2] == superBoard[6]) {
    if (superBoard[2] == player) {
      total += SUPER_TWO;
    } else {
      total -= SUPER_TWO;
    }
  }

  if (superBoard[4] > 0 && superBoard[4] == superBoard[6]) {
    if (superBoard[4] == player) {
      total += SUPER_TWO;
    } else {
      total -= SUPER_TWO;
    }
  }

  //Rows
  for (char i = 0; i < 9; i += 3) {
    if (superBoard[i] > 0 && superBoard[i] == superBoard[i+1]) {
      if (superBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (superBoard[i] > 0 && superBoard[i] == superBoard[i+2]) {
      if (superBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (superBoard[i+1] > 0 && superBoard[i+1] == superBoard[i+2]) {
      if (superBoard[i+1] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }
  }

  //Columns
  for (char i = 0; i < 3; i += 1) {
    if (superBoard[i] > 0 && superBoard[i] == superBoard[i+3]) {
      if (superBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (superBoard[i] > 0 && superBoard[i] == superBoard[i+6]) {
      if (superBoard[i] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }

    if (superBoard[i+3] > 0 && superBoard[i+3] == superBoard[i+6]) {
      if (superBoard[i+3] == player) {
        total += SUB_TWO;
      } else {
        total -= SUB_TWO;
      }
    }
  }

  //for each subboard evaluate the heuristic
  for (char i = 0; i < 9; i++) {
    total += subHeuristic(subBoard, i, player);
  }

  return total;
}

//Minimax with alpha beta pruning
long minimax(char subBoard[], char superBoard[], char superBoardSpot, char goal, char opPlayer, char level, long alpha, long beta) {
  char gameOver = boardWon(superBoard, 0);
  char start, end;
  char player;
  long v;
  struct timeval testTime;
  double elapsedTime;

  //Time ran out
  gettimeofday(&testTime, NULL);
  elapsedTime = (testTime.tv_sec - t1.tv_sec);
  if (elapsedTime >= MAX_TURN_TIME) {
    return heuristic(subBoard, superBoard, opPlayer);
  }

  if (level == 0) {
    return heuristic(subBoard, superBoard, opPlayer);
  }

  if (gameOver == 0) {
    return 0;
  } else if (gameOver == opPlayer) {
    return SUPER_THREE;
  } else if (gameOver > -1) {
    return -SUPER_THREE;
  }

  //We need to go deeper
  if (superBoardSpot == -1) {
    //search all spots on the board
    start = 0;
    end = SUB_BOARD_SIZE;
  } else {
    start = superBoardSpot * 9;
    end = start + 9;
  }

  //setup the minimizer/maximizer
  if (goal == MAXIMIZE) {
    v = -9999999999;
    //Choose the player based on whether we are maximizing or minimizing
    if (opPlayer == X) {
      player = X;
    } else {
      player = O;
    }

    for (int i=start; i < end; i++) {
      if (isOpenSpot(subBoard, superBoard, i)) {
        char newSuperBoardSpot;
        long result;

        //Time ran out
        gettimeofday(&testTime, NULL);
        elapsedTime = (testTime.tv_sec - t1.tv_sec);
        if (elapsedTime >= MAX_TURN_TIME) {
          return v;
        }

        newSuperBoardSpot = doMove(subBoard, superBoard, player, i);
        result = minimax(subBoard, superBoard, newSuperBoardSpot, MINIMIZE, opPlayer, level - 1, alpha, beta);
        undoMove(subBoard, superBoard, i);

        if (result > v) {
          v = result;
        }

        if (v > alpha) {
          alpha = v;
        }

        if (beta <= alpha) {
          break;
        }
      }
    }
  } else {
    v = 9999999999;
    //Choose the player based on whether we are maximizing or minimizing
    if (opPlayer == X) {
      player = O;
    } else {
      player = X;
    }
    for (int i=start; i < end; i++) {
      if (isOpenSpot(subBoard, superBoard, i)) {
        char newSuperBoardSpot;
        long result;

        Time ran out
        gettimeofday(&testTime, NULL);
        elapsedTime = (testTime.tv_sec - t1.tv_sec);
        if (elapsedTime >= MAX_TURN_TIME) {
          return v;
        }

        newSuperBoardSpot = doMove(subBoard, superBoard, player, i);
        result = minimax(subBoard, superBoard, newSuperBoardSpot, MAXIMIZE, opPlayer, level - 1, alpha, beta);
        undoMove(subBoard, superBoard, i);

        if (result < v) {
          v = result;
        }

        if (v < beta) {
          alpha = v;
        }

        if (beta <= alpha) {
          break;
        }
      }
    }
  }


  return v;
}

void threadStarter(char thread) {
  char newSuperBoardSpot = doMove(tData[thread].subBoard, tData[thread].superBoard, tData[thread].opPlayer, tData[thread].move);
  long result = minimax(tData[thread].subBoard, tData[thread].superBoard, newSuperBoardSpot, MINIMIZE, tData[thread].opPlayer, tData[thread].levels, -9999999999, 9999999999);
  undoMove(tData[thread].subBoard, tData[thread].superBoard, tData[thread].move);
  trData[thread].score = result;
  trData[thread].move = tData[thread].move;
}

char getBestMove(char subBoard[], char superBoard[], char superBoardSpot, char opPlayer, char levels) {
  long best = -9999999999;
  char move;
  char start, end;
  char j = 0;

  if (superBoardSpot == -1) {
    //search all spots on the board
    start = 0;
    end = SUB_BOARD_SIZE;
  } else {
    start = superBoardSpot * 9;
    end = start + 9;
  }

  threadpool thpool = thpool_init(THREADS);
  //search within the superboard
  for (char i = start; i < end; i++) {
    if (isOpenSpot(subBoard, superBoard, i)) {
      ThreadData data;
      data.subBoard = copyBoard(subBoard, SUB_BOARD_SIZE);
      data.superBoard = copyBoard(superBoard, SUPER_BOARD_SIZE);
      data.opPlayer = opPlayer;
      data.levels = levels;
      data.move = i;
      tData[j] = data;
      thpool_add_work(thpool, (void*)threadStarter,  j++);
    }
  }
  numThreads = j;

  thpool_wait(thpool);

  for (char i = 0; i < numThreads; i++) {
    if (trData[i].score > best) {
      best = trData[i].score;
      move = trData[i].move;
    }
  }

  return move;
}

void printSuperBoard(char superBoard[]) {
  for (char i = 0; i < SUPER_BOARD_SIZE; i++) {
    printf("%d ", superBoard[i]);
  }
  printf("\n");
}

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
}



int main(void) {
  char inputMove, inputMove2, inputAI, gameOver;
  struct timeval t2, tLast;
  double elapsedTime;

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
    printSuperBoard(superBoard);

    while (1) {
      if (AI == currentPlayer) {
        //Start Timer
        gettimeofday(&t1, NULL);
        gettimeofday(&tLast, NULL);
        printf("\nAI calculating best move in (region %d)...\n", allowedSuperSpot);
        //AI always chooses the first spot as its move, so just set it.
        if (moves == 0) {
          inputMove = 36;
        } else {
          for (char i = 4; i < 81; i++) {
            char levelMove;
            printf("Calculating to level %d...\n", i);
            levelMove = getBestMove(subBoard, superBoard, allowedSuperSpot, currentPlayer, i);
            gettimeofday(&t2, NULL);
            elapsedTime = t2.tv_sec - tLast.tv_sec;
            printf("%f seconds elapsed for level %d\n", elapsedTime, i);
            elapsedTime = t2.tv_sec - t1.tv_sec;
            if (elapsedTime < MAX_TURN_TIME) {
              inputMove = levelMove;
            } else {
              printf("Failed to complete level %d, using level %d move", i, i-1);
              break;
            }
            tLast = t2;
          }
        }
        // stop timer
        gettimeofday(&t2, NULL);
        elapsedTime = (t2.tv_sec - t1.tv_sec);
        printf("\nAI moved to spot: %d (region %d)\n", inputMove, getSuperBoardSpot(inputMove));
        printf("Move took %f seconds\n", elapsedTime);
      } else {
        printf("\nEnter move (region %d): ", allowedSuperSpot);
        scanf("%d", &inputMove);
      }

      if (isOpenSpot(subBoard, superBoard, inputMove) && (getSuperBoardSpot(inputMove) == allowedSuperSpot || allowedSuperSpot == -1)) {
        break;
      } else {
        printBoard(subBoard);
        printf("\nInvalid move.\n");
      }

    }

    allowedSuperSpot = doMove(subBoard, superBoard, currentPlayer, inputMove);
    moves++;

    if (currentPlayer == X) {
      currentPlayer = O;
    } else {
      currentPlayer = X;
    }

    //Check if the game is over
    gameOver = boardWon(superBoard, 0);
    if (gameOver == X) {
      printBoard(subBoard);
      printf("\nGame over, X wins.\n");
      exit(0);
    } else if (gameOver == O) {
      printBoard(subBoard);
      printf("\nGame over, O wins.\n");
      exit(0);
    } else if (gameOver == 0) {
      printBoard(subBoard);
      printf("\nGame over, tie.\n");
      exit(0);
    }
  }
}
