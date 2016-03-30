#define THREE_SUP_WON 1000
#define TWO_SUP_WON 100
#define ONE_SUP_WON 10
#define TWO_ROW_WON 1

char board [81];
char metaBoard[9];

int main ()
{
	metaBoard = {
		"x", 0, "o", 0, 0, "x", 0, "o", "o"
	}
	int i;
	for(i=0;i<sizeof(board);i++)
	{
		board
	}
	printf("%d",getScore("x"));
	printf("\n");
}

int getScore (char player)
{
	//diagonals
	int i = 0;
	int j, k;
	int score = 0;
	for (i = 0; i < sizeof(metaBoard); i++)
	{
		if (metaBoard[i] != 0)
		{
			//diagonal left to right corner
			j = i*9;
			
			if (board[j] == board[j+4])
			{
				score+= (board[j] == player) ? TWO_ROW_WON : -TWO_ROW_WON;
			}
			j = i*9+3;
			if (board[j] == board[j+2])
			{	
				score+=(board[j] == player)?TWO_ROW_WON : -TWO_ROW_WON;	
			}
			//rows
			j=9*i;
			k = j;
			for(;k<=j+6;k+3) //fuck?
			{
				if (board[j] == board[j+1])
				{
					score+=(board[j] == player)?TWO_ROW_WON : -TWO_ROW_WON;
				}
			}
			
			//columns
			j=9*i;
			k = j;
			for(;k<j+3;k+1)
			{
				if (board[j] == board[j+3])
				{
					score+=(board[j] == player)?TWO_ROW_WON : -TWO_ROW_WON;
				}
			}		
		}
	}

	//META BOARD TODO: ONE WAY, THREE WAY
	j = 0;
	//diagonals		
	if (metaBoard[j] == metaBoard[j+4])
	{
		score+= (metaBoard[j] == player) ? TWO_SUP_WON : -TWO_SUP_WON;
	}
	j = 3;
	if (metaBoard[j] == metaBoard[j+2])
	{	
		score+=(metaBoard[j] == player)?TWO_SUP_WON : -TWO_SUP_WON;	
	}
	//rows
	j=0;
	k = j;
	for(;k<=6;j+3) //fuck?
	{
		if (metaBoard[j] == metaBoard[j+1])
		{
			score+=(metaBoard[j] == player)?TWO_SUP_WON : -TWO_SUP_WON;
		}
	}
	
	//columns
	j = 0;
	k = j;
	for(;k<3;j+1)
	{
		if (metaBoard[j] == metaBoard[j+3])
		{
			score+=(metaBoard[j] == player)?TWO_ROW_WON : -TWO_ROW_WON;
		}
	}	
	return score;
}