#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOARD_SIZE 9
#define blank 0
#define X 1
#define O 2
#define MAXIMIZE 0
#define MINIMIZE 1

char **board;
short currentPlayer = X;

typedef struct {
  char row;
  char column;
} Move;

char **allocateArray(int rows, int columns) {
  char **array;
  array = malloc(rows * sizeof(char *));
  if(array == NULL) {
    fprintf(stderr, "out of memory\n");
  	exit(0);
  }
  for (char i = 0; i < rows; i++) {
    array[i] = malloc(columns * sizeof(char));
  	if(array[i] == NULL) {
  		fprintf(stderr, "out of memory\n");
  		exit(0);
  	}
  }
  return array;
}

char **copyBoard(char **board) {
  char **newBoard = allocateArray(BOARD_SIZE, BOARD_SIZE);
  for (char i = 0; i < BOARD_SIZE; i++){
    memcpy(newBoard[i], board[i], BOARD_SIZE * sizeof(char));
  }
  return newBoard;
}

char isGameOver(char **board) {
  //test row wins
  for (char i = 0; i < 3; i++) {
    if (board[i][0] > 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
      return board[i][0];
    }
  }

  //test column wins
  for (char i = 0; i < 3; i++) {
    if (board[0][i] > 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
      return board[0][i];
    }
  }

  //test diagonal wins
  if (board[0][0] > 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
    return board[0][0];
  }
  if (board[2][0] > 0 && board[2][0] == board[1][1] && board[1][1] == board[0][2]) {
    return board[2][0];
  }

  for (char i = 0; i < 3; i++) {
    for (char j = 0; j < 3; j++) {
      if (board[i][j] == 0) {
        return -1;
      }
    }
  }

  return 0;
}

char isValidMove(char **board, short row, short column) {
  if (board[row][column] > 0) {
    return 0;
  } else {
    return 1;
  }
}

char **makeMove(char **board, short player, short row, short column) {
  //copy the board
  board = copyBoard(board);
  board[row][column] = player;

  return board;
}

int minimax(char **board, char goal) {
  char retVal;
  char gameOver = isGameOver(board);
  if (gameOver == 0) {
    return 0;
  } else if (gameOver > 0) {
    if (goal == MAXIMIZE) {
      return -10;
    } else {
      return 10;
    }
  }

  if (goal == MAXIMIZE) {
    retVal = -100;
  } else {
    retVal = 100;
  }

  //loop through all spaces on the board
  for (int row=0; row<3; row++) {
    for (int column=0; column<3; column++) {
      if (isValidMove(board, row, column)) {
        //copy the board
        char **tempBoard = copyBoard(board);
        char result;
        char player = (goal == MAXIMIZE) ? O : X;

        tempBoard = makeMove(board, player, row, column);
        result = minimax(tempBoard, goal == MAXIMIZE ? MINIMIZE : MAXIMIZE);
        if (goal == MAXIMIZE) {
           if (result > retVal) {
             retVal = result;
           }
        } else {
          if (result < retVal) {
            retVal = result;
          }
        }
      }
    }
  }
  return retVal;
}

Move getBestMove(char **board, short player) {
  char best = -10;
  Move move;
  move.row = 0;
  move.column = 0;


  for (int row=0; row<3; row++) {
    for (int column=0; column<3; column++) {
      if (isValidMove(board, row, column)) {
        //copy the board
        char **tempBoard = copyBoard(board);

        tempBoard = makeMove(board, player, row, column);

        char result = minimax(tempBoard, MINIMIZE);
        if (result > best) {
          best = result;
          move.row = row;
          move.column = column;
        }
      }
    }
  }

  return move;
}

void printBoard(char **board) {
   for (char i=0; i<BOARD_SIZE; i++) {
     for (char j=0; j<BOARD_SIZE; j++) {
       if (board[i][j] == X) {
         printf("%c ", 'X');
       } else if (board[i][j] == O) {
         printf("%c ", 'O');
       } else {
         printf("%c ", '0');
       }
     }
     printf("\n");
   }
}

int main(void) {
  int inputRow;
  int inputColumn;
  char gameOver;

  board = allocateArray(BOARD_SIZE, BOARD_SIZE);

  printBoard(board);

  exit(0);
  while (1) {
    printBoard(board);

    if (currentPlayer == O) {
      Move move = getBestMove(board, currentPlayer);
      inputRow = move.row;
      inputColumn = move.column;
    } else {
      printf("\nEnter row: ");
      scanf("%d", &inputRow);
      printf("\nEnter column: ");
      scanf("%d", &inputColumn);
    }

    if (isValidMove(board, inputRow, inputColumn)) {
      board = makeMove(board, currentPlayer, inputRow, inputColumn);
      if (currentPlayer == X) {
        currentPlayer = O;
      } else {
        currentPlayer = X;
      }
    } else {
      printf("\nInvalid Move.\n");
    }

    gameOver = isGameOver(board);
    if (gameOver > -1) {
      printBoard(board);
      printf("\nGame Over: %d\n", gameOver);
      exit(0);
    }

  }
}
