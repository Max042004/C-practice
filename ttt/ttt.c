#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// board 3*3 O/X 0x800001FF
/*
   012
   345
   678
*/

bool check_win(int *board, char *winner, char *cur_player)
{
	int offset = *cur_player == 79?9:0;
	if(((*board & (0b111 << offset)) == (0b111 << offset)) |
	   ((*board & (0b111000 << offset)) == (0b111000 << offset)) |
	   ((*board & (0b111000000 << offset)) == (0b111000000 << offset)) |
	   ((*board & (0b1001001 << offset)) == (0b1001001 << offset)) |
	   ((*board & (0b10010010 << offset)) == (0b10010010 << offset)) |
	   ((*board & (0b100100100 << offset)) == (0b100100100 << offset)) |
	   ((*board & (0b100010001 << offset)) == (0b100010001 << offset)) |
	   ((*board & (0b1010100 << offset)) == (0b1010100 << offset))){
		*winner = *cur_player;
		return true;		
	}
	return false;
}

// O first
void play_game(int *board, char *cur_player)
{
	int offset = *cur_player == 79?9:0;
	int shift, move = 0x1, tmp, new_board;
	for(;;){
		shift = rand() % 9;
		tmp = (move << shift) | (move << (shift + 9));
		if((*board & tmp) != 0) continue;
		new_board = *board | (move << (shift+offset));
		if(new_board != *board) break;
	}
	*board = new_board | *board;
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
		if(check_win(&board, &winner, &cur_player)) break;
		if(cur_player == 79) cur_player = 88;
		else cur_player = 79;
		count++;
		if(count > 7){
			printf("No one win\n");
			return 0;
		}
	}
	printf("winner: %c\n", winner);

	return 0;
		
}
