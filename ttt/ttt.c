/*
 A bit-encoding tic-tac-toe
 Using integer value to encoding every state while playing ttt.
 The first 9 bit records O position,
 and 10~18 bit records X position.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

/* board 3*3 X/O
   012
   345
   678

   corresponding unsigned int in bits:
   0bxxxxxxxxxxxxxxxxxxxxxxxxxx
     |__||__||_______||_______|
      4    4     9        9
      |    |     |        |
      v    v     v        v
   offset count  X        O
*/

bool check_win(unsigned int *state)
{
	// right shift offset for checking current player's board
	unsigned int check_board = *state >> (*state >> 22);

	//check is current player achieve any line
	if((check_board & 0b111) == 0b111 |
	   (check_board & 0b111000) == 0b111000 |
	   (check_board & 0b111000000) == 0b111000000 |
	   (check_board & 0b1001001) == 0b1001001 |
	   (check_board & 0b10010010) == 0b10010010 |
	   (check_board & 0b100100100) == 0b100100100 |
	   (check_board & 0b100010001) == 0b100010001 |
	   (check_board & 0b1010100) == 0b1010100 ) {
		return true;
	}
	return false;
}

// O first
void play_game(unsigned int *state)
{
	unsigned int shift, move = 0x1;
	unsigned int O_board = *state & 0x000001FF, X_board = *state & 0x0003FE00;
	unsigned int curr_board = O_board | X_board >> 9;
	// if random choose position is repeating, choose again.
	for(;;){
		shift = rand() % 9;
		if((move << shift | curr_board) != curr_board) break;
	}
	*state |= move << (shift+(*state >> 22));
}

void draw_board(unsigned int *state)
{
	unsigned int move = 0x1, board = *state & 0x3FFFF;
	for(int i = 0; i < 9; i++){
		if((i+1)%3 == 1) printf("|");
		char piece = ' ';
		if((board & move << i) == (move << i)) piece = 79;
		if((board & move << (i+9)) == (move << (i+9))) piece = 88;
		printf("%c|", piece);
		if((i+1)%3 == 0) printf("\n");	
	}
	printf("\n");
}

int main()
{
	time_t t;
	srand((unsigned) time(&t));

	unsigned int state = 0;
	
	for(;;){
		play_game(&state);
		draw_board(&state);
		if(check_win(&state)) break;

		/* two states :0 = 0000, 88 = 1001
		   to transform between two states, using xor */
		state ^= 0x2400000;

		state += 262144; // count++, meaning state 19's bit++
		if(((state & 0x3C000) >> 18) > 8){
			printf("No one win\n");
			return 0;
		}
		sleep(1);
	}
	printf("winner: %c\n", (state >> 22)+79);
	return 0;		
}
