/*
 A bit-encoding tic-tac-toe
 Using integer value to encoding every state while playing ttt.
 The first 9 bit records X position,
 and 10~18 bit records O position.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

/* board 3*3 O/X
   012
   345
   678
*/

bool check_win(int *board, char *winner, char *cur_player)
{
	int offset = *cur_player == 79?9:0;
	int check_board = *board >> offset;

	//check is current player achieve any line
	if((check_board & 0b111) == 0b111 |
	   (check_board & 0b111000) == 0b111000 |
	   (check_board & 0b111000000) == 0b111000000 |
	   (check_board & 0b1001001) == 0b1001001 |
	   (check_board & 0b10010010) == 0b10010010 |
	   (check_board & 0b100100100) == 0b100100100 |
	   (check_board & 0b100010001) == 0b100010001 |
	   (check_board & 0b1010100) == 0b1010100){
		*winner = *cur_player;
		return true;
	}
	return false;
}

// O first
void play_game(int *board, char *cur_player)
{
	int offset = *cur_player == 79?9:0;
	int shift, move = 0x1;
	int X_board = *board & 0x000001FF, O_board = *board & 0x0003FE00;
	int curr_board = X_board | O_board >> 9;
	// if random choose position is repeating, choose again.
	for(;;){
		shift = rand() % 9;
		if((move << shift | curr_board) != curr_board) break;
	}
	*board |= move << (shift+offset);
}

void draw_board(int *board)
{
	int move = 0x1;
	for(int i = 0; i < 9; i++){
		if((i+1)%3 == 1) printf("|");
		char piece = ' ';
		if((*board & move << i) == (move << i)) piece = 88;
		if((*board & move << (i+9)) == (move << (i+9))) piece = 79;
		printf("%c|", piece);
		if((i+1)%3 == 0) printf("\n");	
	}
	printf("\n");
}

int main()
{
	time_t t;
	srand((unsigned) time(&t));
	int board = 0, count = 0;
	char cur_player = 'O';
	char winner;
	for(;;){
		play_game(&board, &cur_player);
		draw_board(&board);
		if(check_win(&board, &winner, &cur_player)) break;
		if(cur_player == 79) cur_player = 88;
		else cur_player = 79;
		count++;
		if(count > 8){
			printf("No one win\n");
			return 0;
		}
		sleep(1);
	}
	printf("winner: %c\n", winner);

	return 0;		
}
