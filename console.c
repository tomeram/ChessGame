#include "console.h"

void console_main() {
	int action = SETTINGS;

	new_board(g_game.g_board.board);
	print_board(g_game.g_board.board);

	while (action != EXIT) {
		if (action == SETTINGS) {
			action = console_settings();
		}
		else {
			g_game.moves = get_moves(g_game.g_board);
			action = console_game();
			free_moves(&g_game.moves);
		}
	}
}

int console_settings() {
	char cmd[51];
	int in;

	printf("Enter game setting:\n");

	while (1) {
		scanf("%50s", cmd);

		/*****************************************/
		if (strcmp(cmd, "print") == 0) {
			if (flush_input() != 0) {
				printf(ILLEGAL_COMMAND);
			}
			else {
				print_board(g_game.g_board.board);
			}
		}

		/*****************************************/
		else if (strcmp(cmd, "quit") == 0) {
			if (flush_input() != 0) {
				printf(ILLEGAL_COMMAND);
				return SETTINGS;
			}
			else {
				return EXIT;
			}
		}

		/*****************************************/
		else if (strcmp(cmd, "start") == 0) {
			if (flush_input() != 0) {
				printf(ILLEGAL_COMMAND);
				return SETTINGS;
			}
			else {
				return GAME;
			}
		}

		/*****************************************/
		else if (strcmp(cmd, "game_mode") == 0) {
			scanf("%d", &in);

			if (flush_input() != 0) {
				printf(ILLEGAL_COMMAND);
			}
			else {
				if (in != 1 && in != 2) {
					printf("Wrong game mode\n");
				}
				else {
					set_game_mode(in);
					printf("Running game in ");
					if (in == 1) {
						printf("2 players ");
					}
					else {
						printf("player vs. AI ");
					}
					printf("mode\n");
				}
			}
		}

		/*****************************************/
		else if (strcmp(cmd, "difficulty") == 0) {
			if (g_game.mode == PVP) {
				printf(ILLEGAL_COMMAND);
				flush_input();
				return SETTINGS;
			}

			scanf("%s", cmd);
			if (strcmp(cmd, "depth") == 0) {
				scanf("%d", &in);

				if (flush_input() != 0 || in > 4 || in < 1) {
					printf("Wrong value for minimax depth. Value should be between 1 to 4\n");
				}
				else {
					set_game_depth(in);
				}
			}
			else if (strcmp(cmd, "best") == 0) {
				if (flush_input() != 0) {
					printf(ILLEGAL_COMMAND);
				}
				else {
					set_game_depth(BEST);
				}
			}
			else {
				flush_input();
				printf(ILLEGAL_COMMAND);
			}
		}

		/*****************************************/
		else if (strcmp(cmd, "user_color") == 0) {
			if (g_game.mode == PVP) {
				printf(ILLEGAL_COMMAND);
				flush_input();
				return SETTINGS;
			}

			scanf("%s", cmd);
			if (flush_input() != 0) {
				printf(ILLEGAL_COMMAND);
			}
			else if (strcmp(cmd, "white") == 0) {
				set_user_color(WHITE);
			}
			else if (strcmp(cmd, "black") == 0) {
				set_user_color(BLACK);
			}
			else {
				printf(ILLEGAL_COMMAND);
			}
		}

		/*****************************************/
		else if (strcmp(cmd, "load") == 0) {
			scanf("%s", cmd);
			if (flush_input() != 0) {
				printf(ILLEGAL_COMMAND);
			}
			else {
				if (load(cmd))
					print_board(g_game.g_board.board);
			}
		}

		/*****************************************/
		else {
			flush_input();
			printf(ILLEGAL_COMMAND);
		}
	}
}

int console_game() {
	char cmd[51], dst[6], src[6], promote;
	move *curr = NULL;
	int s_x, s_y, d_x, d_y, found = 0, i = 0;


	if (g_game.moves == NULL) {
		if (g_game.g_board.turn == WHITE) {
			printf("Mate! Black player wins the game\n");
		}
		else {
			printf("Mate! White player wins the game\n");
		}

		return EXIT;
	}

	if (g_game.mode == PVP || g_game.user_color == g_game.g_board.turn) {

		if (check4check(g_game.g_board, ((g_game.g_board.turn + 1) % 2))) {
			printf("Check!\n");
		}

		while (1) {
			if (g_game.g_board.turn == WHITE) {
				printf("White player - enter your move:\n");
			}
			else {
				printf("Black player - enter your move:\n");
			}

			scanf("%50s", cmd);

			/*****************************************/
			if (strcmp(cmd, "restart") == 0) {
				if (flush_input() != 0) {
					printf(ILLEGAL_COMMAND);
				}
				else { /* Load Defaults. better doublecheck defaults, and that everythings free */
					g_game.mode = PVP;
					g_game.depth = 1;
					new_board(g_game.g_board.board);
					print_board(g_game.g_board.board);

					return SETTINGS;
				}
			}

			/*****************************************/
			else if (strcmp(cmd, "quit") == 0) {
				if (flush_input() != 0) {
					printf(ILLEGAL_COMMAND);
				}
				else {
					return EXIT;
				}
			}

			/*****************************************/
			else if (strcmp(cmd, "move") == 0) {

				scanf("%5s", src);
				scanf("%50s", cmd);
				scanf("%5s", dst);
				if (strcmp(cmd, "to") == 0) {
					s_x = src[3] - '1';
					s_y = src[1] - 'a';
					d_x = dst[3] - '1';
					d_y = dst[1] - 'a';
					if (s_x < 0 || s_x > 7 || s_y < 0 || s_y > 7 || d_x < 0 || d_x > 7 || d_y < 0 || d_y > 7) {
						printf("Invalid position on the board\n");
					}
					else if (get_color(g_game.g_board.board, s_x, s_y) != g_game.g_board.turn) {
						printf("The specified position does not contain your piece\n");
					}
					else {
						promote = 0;
						if (tolower(g_game.g_board.board[s_y][s_x]) == PAWN && ((d_x == 7) || (d_x == 0))) {
							promote = QUEEN;
							for (i = 0; i < 8; i++) {
								if ((cmd[i] = getchar()) == '\n') {
									found = 1; /* fix for flush input... */
									break;
								}

							}
							cmd[i] = '\0';

							if (strcmp(cmd, " queen") == 0) {
								promote = QUEEN;
							}
							else if (strcmp(cmd, " rook") == 0)
								promote = ROOK;
							else if (strcmp(cmd, " bishop") == 0)
								promote = BISHOP;
							else if (strcmp(cmd, " knight") == 0)
								promote = KNIGHT;
							else if (i != 0)
								promote = 1; /* will give illegal move */
						}
						if (!found && flush_input() != 0) {
							printf("Illegal move\n");
						}
						else {
							/* TODO: make smarter */
							if (leagal_move(s_x, s_y, d_x, d_y) == NULL) {
								printf("Illegal move\n");
							}
							else {
								curr = create_move(s_x, s_y, d_x, d_y, promote);
								make_move(&g_game.g_board, *curr);
								free(curr);
								print_board(g_game.g_board.board);
								return GAME; /* To update the moves for the current player */
							}
						}
					}
				}
			}
			/*****************************************/
			else if (strcmp(cmd, "get_moves") == 0) {
				scanf("%5s", src);
				if (flush_input() != 0) {
					printf(ILLEGAL_COMMAND);
				}
				else {
					s_x = src[3] - '1';
					s_y = src[1] - 'a';
					curr = g_game.moves;
					while (curr != NULL) {
						if (curr->from[0] == s_x && curr->from[1] == s_y) {
							print_move(curr);
						}
						curr = curr->next;
					}
				}
			}

			/*****************************************/
			else if (strcmp(cmd, "save") == 0) {
				scanf("%s", cmd);
				if (flush_input() != 0) {
					printf(ILLEGAL_COMMAND);
				}
				else {
					save(cmd);
				}
			}
			/*****************************************/
			else {
				flush_input();
				printf(ILLEGAL_COMMAND);
			}
		}
	}
	else { /* PvC */
		comp_turn();
		print_board(g_game.g_board.board);
	}

	return GAME;
}

void print_board(char b[BOARD_SIZE][BOARD_SIZE]) {
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf("%d", j + 1);
		for (i = 0; i < BOARD_SIZE; i++) {
			printf("| %c ", b[i][j]);
		}
		printf("|\n");
		print_line();
	}
	printf("  ");
	for (j = 0; j < BOARD_SIZE; j++) {
		printf(" %c  ", (char)('a' + j));
	}
	printf("\n");
}

void print_line() {
	int i;
	printf(" |");
	for (i = 1; i < BOARD_SIZE * 4; i++) {
		printf("-");
	}
	printf("|\n");
}