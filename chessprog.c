#include "chessprog.h"

game g_game;
int error = 0;
int calls = 0;
int general_mode = CONSOLE;

int main(int argc, char *argv[]) {

	g_game.mode = PVP;
	g_game.depth = 1;

	if (argc > 1) {
		if (!strcmp(argv[1], "gui")) {
			general_mode = GUI;
		}
	}

	if (general_mode == CONSOLE) {
		console_main();
	}
	else if (general_mode == GUI) {
		main_gui();
	}

	return 0;
}

int flush_input() {
	int res = 0;
	while (getchar() != '\n') {
		res++;
	}
	return res;
}

void init_board(board b) {
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			b[i][j] = ' ';
		}
	}
}

void new_board(board c_board) {
	int i;

	init_board(c_board);

	c_board[0][0] = c_board[7][0] = ROOK;
	c_board[1][0] = c_board[6][0] = KNIGHT;
	c_board[2][0] = c_board[5][0] = BISHOP;
	c_board[3][0] = QUEEN;
	c_board[4][0] = KING;

	for (i = 0; i < BOARD_SIZE; i++) {
		c_board[i][1] = PAWN;
		c_board[i][6] = toupper(PAWN);
		c_board[i][7] = toupper(c_board[i][0]);
	}
}

void set_game_mode(int n_mode) {
	g_game.mode = n_mode;
}

void set_game_depth(int n_depth) {
	g_game.depth = n_depth;
}

void set_user_color(int n_color) {
	g_game.user_color = n_color;
}

int load(char path[51]) {
	FILE *in = NULL;
	int i, j;

	in = fopen(path, "r"); /* in = fopen("D:\\test.xml", "r"); */
	if (in == NULL) {
		printf("Error: Failed to load game from given file path.\n");
	}

	while (fgetc(in) != '\n'); /* Skip the first line (the xml declarations) */

	while (fscanf(in, "%s", path) != 0 && strcmp(path, "</game>")) {
		if (strstr(path, "<type>") != NULL) {
			g_game.mode = path[6] - '0';
		}
		else if (strstr(path, "<difficulty>") != NULL) {
			if (strstr(path, "Best") != NULL)
				g_game.depth = BEST;
			else
				g_game.depth = path[12] - '0';
		}
		else if (strstr(path, "<user_color>") != NULL) {
			if (strstr(path, "White") != NULL)
				g_game.user_color = WHITE;
			else
				g_game.user_color = BLACK;
		}
		else if (strstr(path, "<next_turn>") != NULL) {
			if (strstr(path, "White") != NULL)
				g_game.g_board.turn = WHITE;
			else
				g_game.g_board.turn = BLACK;
		}
		else if (strstr(path, "<board>") != NULL) {
			for (i = BOARD_SIZE - 1; i >= 0; i--) {
				fscanf(in, "%s", path);

				for (j = 0; j < BOARD_SIZE; j++) {
					if (path[j + 7] == '_')
						g_game.g_board.board[j][i] = ' ';
					else
						g_game.g_board.board[j][i] = path[j + 7];
				}
			}
		}
	}

	fclose(in);

	return 1;
}

int save(char path[51]) {
	FILE *out = NULL;
	int i, j;

	out = fopen(path, "w");
	if (out == NULL) {
		printf("Error! Failed to save file to given file path.\n");
		return 0;
	}

	fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");

	fprintf(out, "<game>\n");

	fprintf(out, "\t<type>");
	fprintf(out, "%d", g_game.mode);
	fprintf(out, "</type>\n");

	if (g_game.mode == PVC) {

		fprintf(out, "\t<difficulty>");
		if (g_game.depth == BEST)
			fprintf(out, "Best");
		else
			fprintf(out, "%d", g_game.depth);
		fprintf(out, "</difficulty>\n");

		fprintf(out, "\t<user_color>");
		if (g_game.user_color == WHITE)
			fprintf(out, "White");
		else
			fprintf(out, "Black");
		fprintf(out, "</user_color>\n");

	}

	fprintf(out, "\t<next_turn>");
	if (g_game.g_board.turn == WHITE)
		fprintf(out, "White");
	else
		fprintf(out, "Black");
	fprintf(out, "</next_turn>\n");

	fprintf(out, "\t<board>\n");
	for (j = BOARD_SIZE - 1; j >= 0; j--) {
		fprintf(out, "\t\t<row_%d>", j + 1);
		for (i = 0; i < BOARD_SIZE; i++) {
			if (g_game.g_board.board[i][j] == ' ')
				fprintf(out, "_");
			else
				fprintf(out, "%c", g_game.g_board.board[i][j]);
		}
		fprintf(out, "</row_%d>\n", j + 1);
	}
	fprintf(out, "</board>\n");

	fprintf(out, "</game>");

	fclose(out);
	return 1;
}

void restart() {
	init_board(g_game.g_board.board);
	new_board(g_game.g_board.board);
	g_game.g_board.turn = WHITE;
	g_game.mode = PVP;
	g_game.depth = 1;
}

void quit() {

}

void make_move(game_board *b, move m) {
	/* parsing and moving. (check checkers!!!!) */
	switch (m.promote) {
	case ROOK:
		(*b).board[m.to[1]][m.to[0]] = ROOK;
		break;
	case KNIGHT:
		(*b).board[m.to[1]][m.to[0]] = KNIGHT;
		break;
	case BISHOP:
		(*b).board[m.to[1]][m.to[0]] = BISHOP;
		break;
	case QUEEN:
		(*b).board[m.to[1]][m.to[0]] = QUEEN;
		break;
	case 0:
		(*b).board[m.to[1]][m.to[0]] = (*b).board[m.from[1]][m.from[0]];
		break;
	}
	if ((*b).turn == BLACK)
		(*b).board[m.to[1]][m.to[0]] = toupper((*b).board[m.to[1]][m.to[0]]);
	(*b).board[m.from[1]][m.from[0]] = ' ';
	(*b).turn = ((*b).turn + 1) % 2;
}

move* leagal_move(int s_x, int s_y, int d_x, int d_y) {
	move *curr = NULL, *found = NULL;

	curr = g_game.moves;
	while (curr != NULL && found == NULL)
	{
		if (curr->from[0] == s_x && curr->from[1] == s_y && curr->to[0] == d_x && curr->to[1] == d_y) {
			found = curr;
		}
		curr = curr->next;
	}
	return found;
}

int check4check(game_board b, int color) {
	int i, j, check = 0;
	move *moves = NULL, *prev;
	pos p;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			p[0] = i;
			p[1] = j;

			if (get_color(b.board, i, j) == color) {
				switch (tolower(b.board[j][i]))
				{
				case PAWN:
					add_move(&moves, move_pawn(b.board, p));
					break;
				case ROOK:
					add_move(&moves, move_straight(b.board, p, 0));
					break;
				case KNIGHT:
					add_move(&moves, move_knight(b.board, p));
					break;
				case BISHOP:
					add_move(&moves, move_diag(b.board, p, 0));
					break;
				case QUEEN:
					add_move(&moves, move_straight(b.board, p, 0));
					add_move(&moves, move_diag(b.board, p, 0));
					break;
				case KING:
					add_move(&moves, move_straight(b.board, p, 1));
					add_move(&moves, move_diag(b.board, p, 1));
					break;
				}
			}
		}
	}

	while (moves != NULL) {
		if (tolower(b.board[moves->to[1]][moves->to[0]]) == KING)
			check = 1;

		prev = moves;
		moves = moves->next;
		free(prev);
	}

	return check;
}


move* get_moves(game_board b) { /* Look at the check4check function for refrances */
	game_board c_board;
	int i, j;
	move *res = NULL, *poss_moves = NULL, *prev;
	pos p;

	for (i = 0; i < BOARD_SIZE && !error; i++) {
		for (j = 0; j < BOARD_SIZE && !error; j++) {
			if (get_color(b.board, i, j) == b.turn) { /* The piece should belong to the current player */
				p[0] = i;
				p[1] = j;
				switch (tolower(b.board[j][i]))
				{
				case PAWN:
					add_move(&poss_moves, move_pawn(b.board, p));
					break;
				case ROOK:
					add_move(&poss_moves, move_straight(b.board, p, 0));
					break;
				case KNIGHT:
					add_move(&poss_moves, move_knight(b.board, p));
					break;
				case BISHOP:
					add_move(&poss_moves, move_diag(b.board, p, 0));
					break;
				case QUEEN:
					add_move(&poss_moves, move_straight(b.board, p, 0));
					add_move(&poss_moves, move_diag(b.board, p, 0));
					break;
				case KING:
					add_move(&poss_moves, move_straight(b.board, p, 1));
					add_move(&poss_moves, move_diag(b.board, p, 1));
					break;
				}
				while (poss_moves != NULL && !error) {
					clone_board(&b, &c_board);
					make_move(&c_board, *poss_moves);

					if (!check4check(c_board, c_board.turn)) {
						add_move(&res, create_move(poss_moves->from[0], poss_moves->from[1], poss_moves->to[0], poss_moves->to[1], poss_moves->promote));
					}

					prev = poss_moves;
					poss_moves = poss_moves->next;
					free(prev);
				}
			}
		}
	}

	if (error) {
		free_moves(&res);
		res = NULL;
	}

	return res;
}

void comp_turn() {
	int max = -600, temp_max;
	game_board clone;
	move *curr = g_game.moves, *best_move = curr;

	calls = 0;

	if (g_game.depth > 4) {
		g_game.depth = calc_depth();
	}

	while (curr != NULL) {
		clone_board(&g_game.g_board, &clone);
		make_move(&clone, *curr);

		temp_max = minmax(clone, 1, max, 600);

		if (temp_max > max) {
			max = temp_max;

			best_move = curr;
		}

		curr = curr->next;
	}

	make_move(&g_game.g_board, *best_move);

	if (general_mode == CONSOLE) {
		printf("Computer: move ");
		print_move(best_move);
	}
}

int minmax(game_board board, int d, int max, int min) {
	int temp;
	move *moves, *prev;
	game_board clone;

	moves = get_moves(board);

	if (moves == NULL) {
		if (board.turn == g_game.user_color)
			return 600;
		else
			return -600;
	}
	if (d == g_game.depth) {
		free_moves(&moves);
		return board_score(board.board);
	}

	while (moves != NULL) {
		if (calls == pow(10, 6)) {
			free_moves(&moves);
			break;
		}
		else {
			calls++;
		}

		clone_board(&board, &clone);

		make_move(&clone, *moves);

		temp = minmax(clone, d + 1, max, min);

		if (board.turn == g_game.user_color && temp < min) {
			min = temp;
		}
		else if (board.turn != g_game.user_color && temp > max) {
			max = temp;
		}

		if (min <= max) {
			free_moves(&moves);
			break;
		}

		prev = moves;
		moves = moves->next;
		free(prev);
	}

	if (board.turn == g_game.user_color)
		return min;
	else
		return max;
}

int get_x(char *move) {
	return 0;
}

int get_y(char *move) {
	return 0;
}

int check_board(board b, int turn) {
	return 0;
}

int board_score(board b) {
	int i, j, sum = 0;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			switch (tolower(b[j][i]))
			{
			case PAWN:
				if (get_color(b, i, j) == g_game.user_color)
					sum -= 1;
				else
					sum += 1;
				break;
			case ROOK:
				if (get_color(b, i, j) == g_game.user_color)
					sum -= 5;
				else
					sum += 5;
				break;
			case KNIGHT:
				if (get_color(b, i, j) == g_game.user_color)
					sum -= 3;
				else
					sum += 3;
				break;
			case BISHOP:
				if (get_color(b, i, j) == g_game.user_color)
					sum -= 3;
				else
					sum += 3;
				break;
			case QUEEN:
				if (get_color(b, i, j) == g_game.user_color)
					sum -= 9;
				else
					sum += 9;
				break;
			case KING:
				if (get_color(b, i, j) == g_game.user_color)
					sum -= 400;
				else
					sum += 400;
				break;
			}
		}
	}
	return sum;
}

void add_move(move **head, move *new_move) {
	move *curr = *head;

	if (new_move == NULL) {
		return;
	}
	else if (curr == NULL) {
		*head = new_move;
	}
	else {
		while (curr->next != NULL)
			curr = curr->next;
		curr->next = new_move;
	}
}

move* create_move(int from_x, int from_y, int to_x, int to_y, char promotion) {
	move *res = (move*)malloc(sizeof(move));

	if (res == NULL) {
		printf("Error: Failed to allocate memory.\n");
		error = 1;
		return NULL;
	}

	res->from[0] = from_x;
	res->from[1] = from_y;
	res->to[0] = to_x;
	res->to[1] = to_y;
	res->promote = promotion;

	res->next = NULL;

	return res;
}

move* move_straight(board b, pos p, int len) {
	move *res = NULL, *n_move;
	int i, color = get_color(b, p[0], p[1]);

	for (i = p[0] + 1; i < BOARD_SIZE; i++) { /* Up */
		if (get_color(b, i, p[1]) != color) {
			n_move = create_move(p[0], p[1], i, p[1], 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, i, p[1]) != -1)
			break;
	}

	for (i = p[0] - 1; i >= 0; i--) { /* Down */
		if (get_color(b, i, p[1]) != color) {
			n_move = create_move(p[0], p[1], i, p[1], 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, i, p[1]) != -1)
			break;
	}

	for (i = p[1] + 1; i < BOARD_SIZE; i++) { /* Right */
		if (get_color(b, p[0], i) != color) {
			n_move = create_move(p[0], p[1], p[0], i, 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, p[0], i) != -1)
			break;
	}

	for (i = p[1] - 1; i >= 0; i--) { /* Left */
		if (get_color(b, p[0], i) != color) {
			n_move = create_move(p[0], p[1], p[0], i, 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, p[0], i) != -1)
			break;
	}

	return res;
}

move* move_diag(board b, pos p, int len) {
	move *res = NULL, *n_move;
	int i, color = get_color(b, p[0], p[1]);

	for (i = 1; p[0] + i < BOARD_SIZE && p[1] + i < BOARD_SIZE; i++) { /* Up + Right */
		if (get_color(b, p[0] + i, p[1] + i) != color) {
			n_move = create_move(p[0], p[1], p[0] + i, p[1] + i, 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, p[0] + i, p[1] + i) != -1)
			break;
	}

	for (i = 1; p[0] + i < BOARD_SIZE && p[1] - i >= 0; i++) { /* Up + Left */
		if (get_color(b, p[0] + i, p[1] - i) != color) {
			n_move = create_move(p[0], p[1], p[0] + i, p[1] - i, 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, p[0] + i, p[1] - i) != -1)
			break;
	}

	for (i = 1; p[0] - i >= 0 && p[1] + i < BOARD_SIZE; i++) { /* Down + Right */
		if (get_color(b, p[0] - i, p[1] + i) != color) {
			n_move = create_move(p[0], p[1], p[0] - i, p[1] + i, 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, p[0] - i, p[1] + i) != -1)
			break;
	}

	for (i = 1; p[0] - i >= 0 && p[1] - i >= 0; i++) { /* Down + Left */
		if (get_color(b, p[0] - i, p[1] - i) != color) {
			n_move = create_move(p[0], p[1], p[0] - i, p[1] - i, 0);
			add_move(&res, n_move);
		}

		if (len == 1 || get_color(b, p[0] - i, p[1] - i) != -1)
			break;
	}

	return res;
}

move* move_pawn(board b, pos p) {
	move *res = NULL, *n_move;
	int color = get_color(b, p[0], p[1]), dir;

	if (color == WHITE)
		dir = 1;
	else
		dir = -1;

	/* Regular move */
	if (get_color(b, p[0] + dir, p[1]) == -1) {
		if (p[0] + dir == BOARD_SIZE - 1 || p[0] + dir == 0) {
			n_move = create_move(p[0], p[1], p[0] + dir, p[1], QUEEN);
			add_move(&res, n_move);

			n_move = create_move(p[0], p[1], p[0] + dir, p[1], BISHOP);
			add_move(&res, n_move);

			n_move = create_move(p[0], p[1], p[0] + dir, p[1], ROOK);
			add_move(&res, n_move);

			n_move = create_move(p[0], p[1], p[0] + dir, p[1], KNIGHT);
			add_move(&res, n_move);
		}
		else {
			n_move = create_move(p[0], p[1], p[0] + dir, p[1], 0);
			add_move(&res, n_move);
		}
	}

	/* Capture move */
	if (p[1] + 1 < BOARD_SIZE) {
		if (get_color(b, p[0] + dir, p[1] + 1) != color && get_color(b, p[0] + dir, p[1] + 1) != -1) {
			if (p[0] + dir == BOARD_SIZE - 1 || p[0] + dir == 0) {
				n_move = create_move(p[0], p[1], p[0] + dir, p[1] + 1, QUEEN);
				add_move(&res, n_move);

				n_move = create_move(p[0], p[1], p[0] + dir, p[1] + 1, BISHOP);
				add_move(&res, n_move);

				n_move = create_move(p[0], p[1], p[0] + dir, p[1] + 1, ROOK);
				add_move(&res, n_move);

				n_move = create_move(p[0], p[1], p[0] + dir, p[1] + 1, KNIGHT);
				add_move(&res, n_move);
			}
			else {
				n_move = create_move(p[0], p[1], p[0] + dir, p[1] + 1, 0);
				add_move(&res, n_move);
			}
		}
	}

	if (p[1] - 1 >= 0) {
		if (get_color(b, p[0] + dir, p[1] - 1) != color && get_color(b, p[0] + dir, p[1] - 1) != -1) {
			if (p[0] + dir == BOARD_SIZE - 1 || p[0] + dir == 0) {
				n_move = create_move(p[0], p[1], p[0] + dir, p[1] - 1, QUEEN);
				add_move(&res, n_move);

				n_move = create_move(p[0], p[1], p[0] + dir, p[1] - 1, BISHOP);
				add_move(&res, n_move);

				n_move = create_move(p[0], p[1], p[0] + dir, p[1] - 1, ROOK);
				add_move(&res, n_move);

				n_move = create_move(p[0], p[1], p[0] + dir, p[1] - 1, KNIGHT);
				add_move(&res, n_move);
			}
			else {
				n_move = create_move(p[0], p[1], p[0] + dir, p[1] - 1, 0);
				add_move(&res, n_move);
			}
		}
	}
	return res;
}

move* move_knight(board b, pos p) {
	move *res = NULL, *n_move;
	int color = get_color(b, p[0], p[1]), dest_color, x = p[0], y = p[1];

	if (x + 2 < BOARD_SIZE) {
		if (y + 1 < BOARD_SIZE) {
			dest_color = get_color(b, x + 2, y + 1);

			if (color != dest_color) {
				n_move = create_move(x, y, x + 2, y + 1, 0);
				add_move(&res, n_move);
			}
		}

		if (y - 1 >= 0) {
			dest_color = get_color(b, x + 2, y - 1);

			if (color != dest_color) {
				n_move = create_move(x, y, x + 2, y - 1, 0);
				add_move(&res, n_move);
			}
		}
	}

	if (x - 2 >= 0) {
		if (y + 1 < BOARD_SIZE) {
			dest_color = get_color(b, x - 2, y + 1);

			if (color != dest_color) {
				n_move = create_move(x, y, x - 2, y + 1, 0);
				add_move(&res, n_move);
			}
		}

		if (y - 1 >= 0) {
			dest_color = get_color(b, x - 2, y - 1);

			if (color != dest_color) {
				n_move = create_move(x, y, x - 2, y - 1, 0);
				add_move(&res, n_move);
			}
		}
	}

	if (y + 2 < BOARD_SIZE) {
		if (x + 1 < BOARD_SIZE) {
			dest_color = get_color(b, x + 1, y + 2);

			if (color != dest_color) {
				n_move = create_move(x, y, x + 1, y + 2, 0);
				add_move(&res, n_move);
			}
		}

		if (x - 1 >= 0) {
			dest_color = get_color(b, x - 1, y + 2);

			if (color != dest_color) {
				n_move = create_move(x, y, x - 1, y + 2, 0);
				add_move(&res, n_move);
			}
		}
	}

	if (y - 2 >= 0) {
		if (x + 1 < BOARD_SIZE) {
			dest_color = get_color(b, x + 1, y - 2);

			if (color != dest_color) {
				n_move = create_move(x, y, x + 1, y - 2, 0);
				add_move(&res, n_move);
			}
		}

		if (x - 1 >= 0) {
			dest_color = get_color(b, x - 1, y - 2);

			if (color != dest_color) {
				n_move = create_move(x, y, x - 1, y - 2, 0);
				add_move(&res, n_move);
			}
		}
	}

	return res;
}

int get_color(board b, int x, int y) {
	char curr = b[y][x];
	if (curr >= 'a' && curr <= 'z')
		return WHITE;
	else if (curr >= 'A' && curr <= 'Z')
		return BLACK;
	else
		return -1;
}

void clone_board(game_board *s_board, game_board *d_board) {
	int i, j;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++)
			(*d_board).board[i][j] = (*s_board).board[i][j];
	}

	(*d_board).turn = (*s_board).turn;
}

void free_moves(move **head) {
	if (*head != NULL)
	{
		free_moves((&(*head)->next));
		free(*head);
	}
}

void print_move(move *m) {
	printf("<%c,%c> to <%c,%c>", m->from[1] + 'a', m->from[0] + '1', m->to[1] + 'a', m->to[0] + '1');

	switch (m->promote) {
	case ROOK:
		printf(" rook\n");
		break;
	case KNIGHT:
		printf(" knight\n");
		break;
	case BISHOP:
		printf(" bishop\n");
		break;
	case QUEEN:
		printf(" queen\n");
		break;
	case 0:
		printf("\n");
		break;
	default:
		break;
	}
}

int calc_depth() {
	int num = 0, i, j;
	float tmp = 1.0;

	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (g_game.g_board.board[i][j] != ' ') {
				num++;
			}
		}
	}

	tmp = (tmp / num) * 64 + 4;

	if (tmp > 7)
		tmp = 7;

	return (int)tmp;
}
