#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define blank 0
#define X 1
#define Y 2

char **board;
short currentPlayer = X;

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
  char **newBoard = allocateArray(3, 3);
  for (int i = 0; i < 3; i++){
    memcpy(newBoard[i], board[i], 3 * sizeof(char));
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


// int evaluateTurn(char **board, short player, short row, short column) {
//   char **tempBoard = copyBoard(board);
//   for (int i=0; i<3; i++) {
//     for (int j=0; j<3; j++) {
//       if (isValidMove(tempBoard[i][j], row, column)) {
//         evaluateTurn(tempBoard)
//       }
//     }
//   }
//   return
// }

void printBoard(char **board) {
   for (char i=0; i<3; i++) {
     for (char j=0; j<3; j++) {
        if (j < 2) {
	         printf("%d | ", board[i][j]);
        } else {
	         printf("%d", board[i][j]);
        }
     }
     if (i < 2) {
       printf("\n---------");
     }
     printf("\n");
   }
}

int main(void) {
  int inputRow;
  int inputColumn;
  char gameOver;

  board = allocateArray(3, 3);
  while (1) {
    printBoard(board);
    printf("\nEnter row: ");
    scanf("%d", &inputRow);
    printf("\nEnter column: ");
    scanf("%d", &inputColumn);

    if (isValidMove(board, inputRow, inputColumn)) {
      board = makeMove(board, currentPlayer, inputRow, inputColumn);
      if (currentPlayer == X) {
        currentPlayer = Y;
      } else {
        currentPlayer = X;
      }
    } else {
      printf("\nInvalid Move.\n");
    }

    gameOver = isGameOver(board);
    if (gameOver > -1) {
      printf("\nGame Over: %d\n", gameOver);
      exit(0);
    }

  }
}
