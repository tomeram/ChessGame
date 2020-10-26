#ifndef CHESSPROG_
#define CHESSPROG_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>

/** Defines **/
#define CONSOLE  0
#define GUI 1

#define EXIT 0
#define HOME 1
#define SETTINGS 2
#define GAME 3

#define BOARD_SIZE 8
#define PVP 1
#define PVC 2
#define BEST 5
#define WHITE 0
#define BLACK 1

#define PAWN 'm'
#define BISHOP 'b'
#define ROOK 'r'
#define KNIGHT 'n'
#define QUEEN 'q'
#define KING 'k'

#define ILLEGAL_COMMAND "Illegal command, please try again\n"

/** Structs **/

typedef char board[BOARD_SIZE][BOARD_SIZE];
typedef int pos[2];

typedef struct {
	board board;
	int turn;
} game_board;

typedef struct move {
	pos from, to;
	char promote;
	struct move *next;
} move;

typedef struct {
	char *name;
	game_board g_board;
	int depth, user_color, mode;
	move *moves;
} game;

/** Global Vars **/

extern game g_game;
extern int error;

/** Global Funcs **/

int flush_input();
void init_board(board);
void new_board(board);
void set_game_mode(int);
void set_game_depth(int);
void set_user_color(int);
int load(char[51]);
int save(char path[51]);
void restart();
void quit();
void make_move(game_board*, move);
move* leagal_move(int, int, int, int);
move* get_moves(game_board);
void comp_turn();
int minmax(game_board board, int d, int max, int min);
int get_x(char*);
int get_y(char*);
int check_board(board, int);
int board_score(board);
int check4check(game_board, int);
void add_move(move**, move*);
move* create_move(int, int, int, int, char);
void clone_board(game_board*, game_board*);
void free_moves(move**);
int get_color(board, int, int);
move* move_pawn(board, pos);
move* move_straight(board, pos, int);
move* move_diag(board, pos, int);
move* move_knight(board, pos);
int calc_depth();

/* GUI funcs */
int main_gui();

/** Console Funcs **/
void console_main();
void print_move(move*);

#endif
