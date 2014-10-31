#include "gui.h"

#define GUI_MAIN 1

SDL_Surface *window;
int mode = GUI_MAIN;
int img_error = 0; /* To prevent multiple shows of error screen */

int main_gui() {
	SDL_Surface *icon = SDL_LoadBMP("img/icon.bmp");
	SDL_Surface *splash = SDL_LoadBMP("img/splash.bmp");
	window = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	else if (window == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	if (check_img_load(icon))
		clear_green(icon);
	check_img_load(splash);

	SDL_WM_SetCaption("booyah", NULL);
	SDL_WM_SetIcon(icon, NULL);

	blit_all(splash, NULL, NULL);
	print_window();

	SDL_Delay(5000);

	while (mode != EXIT) {
		switch (mode)
		{
		case GUI_MAIN:
			main_menu();
			break;
		case GAME:
			gui_game();
			break;
		}
	}

	/* Frees & Quits */
	SDL_FreeSurface(window);
	SDL_FreeSurface(icon);

	SDL_Quit();
	return 1;
}

int main_menu() {
	SDL_Surface *main_menu = SDL_LoadBMP("img/main-menu.bmp");
	SDL_Surface *img = SDL_LoadBMP("img/menu-button-border.bmp");

	SDL_Rect img_rect = { 226, 131, 188, 51 };

	SDL_Event e;

	int selected_button = -1;

	check_img_load(main_menu);
	if (check_img_load(img))
		clear_green(img);

	while (mode == GUI_MAIN) {
		blit_all(main_menu, NULL, 0);

		if (selected_button > -1) {
			img_rect.y = 131 + (83 * selected_button);

			blit_all(img, NULL, &img_rect);
		}

		print_window();

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					mode = EXIT;
					break;
				case SDLK_DOWN:
					selected_button = (selected_button + 1) % 3;
					break;
				case SDLK_UP:
					if (selected_button == -1) selected_button++;
					selected_button = (selected_button + 2) % 3;
					break;
				case SDLK_RETURN:
					switch (selected_button) {
					case 0:
						restart();
						gui_pvp_settings();
						break;
					case 1:
						gui_saveload_menu(LOAD);
						break;
					case 2:
						mode = EXIT;
						break;
					}
					break;
				default:
					break;
				}
			}
		}
	}
	SDL_FreeSurface(main_menu);
	SDL_FreeSurface(img);

	return 0;
}

void gui_pvp_settings() {
	SDL_Surface *main_view = SDL_LoadBMP("img/pvp-settings.bmp");
	SDL_Surface *border = SDL_LoadBMP("img/menu-button-border.bmp");

	SDL_Rect img_rect = { 0, 0, 188, 51 };

	SDL_Event e;

	int curr_butt = -1;
	int loop = 1;

	check_img_load(main_view);
	if (check_img_load(border))
		clear_green(border);

	while (loop) {
		blit_all(main_view, NULL, 0);

		if (curr_butt >= 0) {
			switch (curr_butt) {
			case 0:
				img_rect.x = 124;
				img_rect.y = 173;
				break;
			case 1:
				img_rect.x = 328;
				img_rect.y = 173;
				break;
			case 2:
				img_rect.x = 224;
				img_rect.y = 256;
				break;
			}

			blit_all(border, NULL, &img_rect);
		}

		print_window();

		while (SDL_PollEvent(&e) && loop) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					loop = 0;
					break;
				case SDLK_RIGHT:
					if (curr_butt != 2)
						curr_butt = (curr_butt + 1) % 2;
					break;
				case SDLK_LEFT:
					if (curr_butt != 2) {
						if (curr_butt < 0) curr_butt = 0;
						curr_butt = (curr_butt + 1) % 2;
					}
					break;
				case SDLK_UP:
					if (curr_butt != 2) {
						curr_butt = 2;
					}
					else {
						curr_butt = 0;
					}
					break;
				case SDLK_DOWN:
					if (curr_butt != 2) {
						curr_butt = 2;
					}
					else {
						curr_butt = 0;
					}
					break;
				case SDLK_RETURN:
					switch (curr_butt) {
					case 0:
						mode = GAME;
						g_game.mode = 1;
						break;
					case 1:
						gui_pvc_settings();
						break;
					case 2:
						loop = 0;
						break;
					}

					if (mode == GAME)
						loop = 0;
					break;
				default:
					break;
				}
			}
		}
	}

	SDL_FreeSurface(main_view);
	SDL_FreeSurface(border);
}

void gui_pvc_settings() {
	SDL_Surface *main_view = SDL_LoadBMP("img/pvc-settings.bmp");
	SDL_Surface *border = SDL_LoadBMP("img/menu-button-border.bmp");
	SDL_Surface *diff_buttons = SDL_LoadBMP("img/diff-buttons.bmp");
	SDL_Surface *diff_border = SDL_LoadBMP("img/diff-border.bmp");
	SDL_Surface *color_btn = SDL_LoadBMP("img/color-btn.bmp");

	SDL_Rect button_border = { 76, 148, 188, 51 };
	SDL_Rect diff_pos = { 320, 219, 60, 45 };
	SDL_Rect diff_but = { 0, 0, 65, 43 };
	SDL_Rect diff_but_rect = { 316, 215, 69, 47 };
	SDL_Rect color_but = { 0, 0, 190, 45 };
	SDL_Rect color_pos = { 320, 152, 69, 44 };

	SDL_Event e;

	int curr_butt = -1;
	int loop = 1;

	check_img_load(main_view);
	if (check_img_load(border))
		clear_green(border);
	check_img_load(diff_buttons);
	if (check_img_load(diff_border))
		clear_green(diff_border);
	check_img_load(color_btn);

	while (loop) {
		blit_all(main_view, NULL, 0);

		/* Update color display */
		color_but.y = 45 * g_game.user_color;

		blit_all(color_btn, &color_but, &color_pos);

		/* Update difficulty display */
		diff_but.y = 43 * (g_game.depth - 1);

		blit_all(diff_buttons, &diff_but, &diff_pos);

		/* Update selected button */
		if (curr_butt >= 0) {

			if (curr_butt < 4) {
				button_border.x = 76;
				button_border.y = 148 + (67 * curr_butt);

				blit_all(border, NULL, &button_border);
			}
			else if (curr_butt == 5) {
				blit_all(diff_border, NULL, &diff_but_rect);
			}
			else {
				button_border.x = 317;
				button_border.y = 149;

				blit_all(border, NULL, &button_border);
			}
		}

		print_window();

		while (SDL_PollEvent(&e) && loop) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					loop = 0;
					break;
				case SDLK_RIGHT:
					if (curr_butt == 0 || curr_butt == 1)
						curr_butt += 4;
					break;
				case SDLK_LEFT:
					if (curr_butt == 4 || curr_butt == 5)
						curr_butt -= 4;
					break;
				case SDLK_UP:
					if (curr_butt < 0) curr_butt = 0;
					if (curr_butt == 5) {
						g_game.depth--;
						g_game.depth = ((g_game.depth + 1) % 5) + 1;
					}
					else if (curr_butt == 4)
						g_game.user_color = (g_game.user_color + 1) % 2;
					else
						curr_butt = (curr_butt + 3) % 4;
					break;
				case SDLK_DOWN:
					if (curr_butt == 5) {
						g_game.depth--;
						g_game.depth = ((g_game.depth + 4) % 5) + 1;
					}
					else if (curr_butt == 4)
						g_game.user_color = (g_game.user_color + 1) % 2;
					else
						curr_butt = (curr_butt + 1) % 4;
					break;
				case SDLK_RETURN:
					switch (curr_butt) {
					case 0:
						curr_butt = 4;
						break;
					case 1:
						curr_butt = 5;
						break;
					case 2:
						mode = GAME;
						g_game.mode = 2;
						loop = 0;
						break;
					case 3:
						loop = 0;
						break;
					case 4:
						g_game.user_color = (g_game.user_color + 1) % 2;
						break;
					case 5:
						g_game.depth--;
						g_game.depth = ((g_game.depth + 1) % 5) + 1;
						break;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	SDL_FreeSurface(main_view);
	SDL_FreeSurface(border);
	SDL_FreeSurface(diff_buttons);
	SDL_FreeSurface(diff_border);
	SDL_FreeSurface(color_btn);
}

void gui_saveload_menu(int type) {
	SDL_Surface *menu;
	SDL_Surface *img = SDL_LoadBMP("img/menu-button-border.bmp");

	SDL_Rect img_rect = { 84, 139, 188, 51 };

	SDL_Event e;

	int loop = 1, selected_r = -1, selected_c = -1, selected;

	char avail_slots = 0, path[51] = "saved_games/game_x.xml\0";

	if (type == LOAD) {
		menu = SDL_LoadBMP("img/load_menu.bmp");
	}
	else {
		menu = SDL_LoadBMP("img/save_menu.bmp");
	}

	if (check_img_load(img))
		clear_green(img);
	check_img_load(menu);

	avail_slots = check_empty_slots();

	while (loop) {
		blit_all(menu, NULL, 0);
		draw_empty_slots(avail_slots);

		if (selected_r > -1 && selected_c > -1) {
			img_rect.y = 139 + (75 * selected_r);
			img_rect.x = 84 + (284 * selected_c);

			blit_all(img, NULL, &img_rect);
		}

		print_window();

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					loop = 0;
					break;

				case SDLK_DOWN:
					if (selected_c == -1 && selected_r == -1) {
						selected_c = 0;
						selected_r = 0;
					}
					else {
						selected_r = (selected_r + 1) % 3;
					}
					break;

				case SDLK_UP:
					if (selected_c == -1 && selected_r == -1) {
						selected_c = 0;
						selected_r = 2;
					}
					else {
						selected_r = (selected_r + 2) % 3;
					}
					break;

				case SDLK_LEFT:
					if (selected_c == -1 && selected_r == -1) {
						selected_c = 0;
						selected_r = 0;
					}
					else {
						selected_c = (selected_c + 1) % 2;
					}
					break;

				case SDLK_RIGHT:
					if (selected_c == -1 && selected_r == -1) {
						selected_c = 0;
						selected_r = 0;
					}
					else {
						selected_c = (selected_c + 1) % 2;
					}
					break;

				case SDLK_RETURN:
					selected = (selected_r * 2) + selected_c;
					path[17] = (selected + 1) + '0';
					if (selected == 5) {
						loop = 0;
						break;
					}
					else if (type == SAVE) {
						save(path);
						loop = 0;
					}
					else {
						if ((avail_slots >> selected) & 1) { /*Not Empty Slot*/
							if (load(path)) {
								loop = 0;
								gui_pvp_settings();
							}
						}
						else {
							/*TODO no file found error that is not printf? */
						}
					}

					break;
				default:
					break;
				}
			}
		}
	}

	SDL_FreeSurface(menu);
	SDL_FreeSurface(img);
}


void print_window() {
	if (SDL_Flip(window) != 0) {
		printf("ERROR: failed to flip buffer.\n");
		error = 1;
	}
}


char check_empty_slots() {
	FILE *slot = NULL;
	char path[23] = "saved_games/game_x.xml\0", res = 0;
	int i;

	for (i = 5; i > 0; i--) {
		path[17] = i + '0';
		slot = fopen(path, "r");
		if (slot != NULL) {
			res += 1;
			fclose(slot);
			slot = NULL;
		}
		res <<= 1;
	}
	res >>= 1; /* deal with one extra << */
	return res;
}

void draw_empty_slots(char avail_slots) {
	SDL_Surface *empty = SDL_LoadBMP("img/button_empty.bmp");
	SDL_Rect img_rect = { 88, 143, 188, 51 };
	int i;

	check_img_load(empty);

	for (i = 0; i < 5; i++) {
		if (!(avail_slots & 1)) {
			blit_all(empty, NULL, &img_rect);
		}

		if (i % 2 == 0)
			img_rect.x += 284;
		else {
			img_rect.y += 75;
			img_rect.x -= 284;
		}
		avail_slots >>= 1;
	}
	SDL_FreeSurface(empty);
}

void gui_game() {
	SDL_Surface *bg_img = SDL_LoadBMP("img/chess-game.bmp");
	SDL_Surface *board_img = SDL_LoadBMP("img/board_img.bmp");
	SDL_Surface *bord_border = SDL_LoadBMP("img/bord-border.bmp");
	SDL_Surface *selected_border = SDL_LoadBMP("img/bord-selected.bmp");
	SDL_Surface *button_border = SDL_LoadBMP("img/menu-button-border.bmp");

	SDL_Rect board_rect = { 0, 50, 430, 430 };
	SDL_Rect bord_border_rect = { 0, 0, 58, 58 };
	SDL_Rect button_border_rect = { 440, 0, 188, 51 };

	SDL_Event e;

	move *curr = NULL;

	int loop = 1, x = 0, y = 0, z = 0, selected = 0, selected_x, selected_y, promote = 0;

	check_img_load(bg_img);
	check_img_load(board_img);
	if (check_img_load(bord_border))
		clear_green(bord_border);
	if (check_img_load(selected_border))
		clear_green(selected_border);
	if (check_img_load(button_border))
		clear_green(button_border);

	g_game.moves = get_moves(g_game.g_board);

	if (error) {
		loop = 0;
		mode = EXIT;
	}

	while (loop) {

		blit_all(bg_img, NULL, 0);
		blit_all(board_img, NULL, &board_rect);

		print_pieces();
		print_title();

		if (selected) {
			print_moves(selected_x, selected_y);
		}

		/* Show selected border */
		if (selected) {
			bord_border_rect.x = 24 + (50 * selected_x);
			bord_border_rect.y = 398 - (50 * selected_y);

			blit_all(selected_border, NULL, &bord_border_rect);
		}

		/* Show selection border */
		if (x < 8 && (g_game.mode == 1 || g_game.user_color == g_game.g_board.turn)) {
			bord_border_rect.x = 24 + 50 * x;
			bord_border_rect.y = 398 - (50 * y);

			blit_all(bord_border, NULL, &bord_border_rect);
		}
		else if (x == 8) {
			button_border_rect.y = 53 + (112 * z);

			blit_all(button_border, NULL, &button_border_rect);
		}

		print_window();

		if (g_game.mode == 2 && g_game.g_board.turn != g_game.user_color && g_game.moves != NULL) {
			comp_turn();

			free_moves(&g_game.moves);
			g_game.moves = get_moves(g_game.g_board);

			if (error) {
				loop = 0;
				mode = EXIT;
			}
		}

		print_window();

		while (SDL_PollEvent(&e) && loop && (g_game.mode == 1 || g_game.user_color == g_game.g_board.turn || g_game.moves == NULL)) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					if (selected)
						selected = 0;
					else {
						mode = GUI_MAIN;
						loop = 0;
					}
					break;
				case SDLK_RIGHT:
					x = (x + 1) % 9;
					break;
				case SDLK_LEFT:
					x = (x + 8) % 9;
					break;
				case SDLK_UP:
					if (x < 8)
						y = (y + 1) % 8;
					else
						z = (z + 3) % 4;
					break;
				case SDLK_DOWN:
					if (x < 8)
						y = (y + 7) % 8;
					else
						z = (z + 1) % 4;
					break;
				case SDLK_RETURN:
					if (x < 8) {
						if (selected && selected_x == x && selected_y == y) {
							selected = 0;
						}
						else if (!selected || (selected && get_color(g_game.g_board.board, y, x) == g_game.g_board.turn)) {
							curr = g_game.moves;

							while (curr != NULL) {
								if (curr->from[0] == y && curr->from[1] == x) {
									selected = 1;
									selected_x = x;
									selected_y = y;
									curr = NULL;
								}
								else
									curr = curr->next;
							}
						}
						else {
							curr = leagal_move(selected_y, selected_x, y, x);

							if (curr != NULL) {
								if (tolower(g_game.g_board.board[selected_x][selected_y]) == PAWN) {
									if ((g_game.g_board.turn == WHITE && y == BOARD_SIZE - 1) || (g_game.g_board.turn == BLACK && y == 0)) {
										promote = gui_promote();
									}
								}
								if (promote != -1) {
									curr->promote = promote;
									promote = 0;

									make_move(&g_game.g_board, *curr);
									selected = 0;

									free_moves(&g_game.moves);
									g_game.moves = get_moves(g_game.g_board);

									if (error) {
										loop = 0;
										mode = EXIT;
									}
								}
							}
						}

					}
					else {
						switch (z) {
						case 0:
							gui_saveload_menu(SAVE);
							break;
						case 1:
							new_board(g_game.g_board.board);
							g_game.g_board.turn = WHITE;
							free_moves(&g_game.moves);
							g_game.moves = get_moves(g_game.g_board);

							if (error) {
								loop = 0;
								mode = EXIT;
							}
							break;
						case 2:
							mode = GUI_MAIN;
							loop = 0;
							break;
						case 3:
							mode = EXIT;
							loop = 0;
							break;
						}
					}
					break;
				default:
					break;
				}
			}
		}
	}

	free_moves(&g_game.moves);

	SDL_FreeSurface(bg_img);
	SDL_FreeSurface(board_img);
	SDL_FreeSurface(bord_border);
	SDL_FreeSurface(selected_border);
	SDL_FreeSurface(button_border);
}

void print_pieces() {
	SDL_Surface *chess_pieces = SDL_LoadBMP("img/chess-pieses.bmp");

	SDL_Rect piece_pos = { 0, 0, 50, 50 };
	SDL_Rect piece_selected = { 2, 2, 48, 48 };

	int x, y;
	char curr_color, curr_piece;

	if (check_img_load(chess_pieces))
		clear_green(chess_pieces);

	for (x = 0; x < BOARD_SIZE; x++) {
		for (y = 0; y < BOARD_SIZE; y++) {
			curr_color = get_color(g_game.g_board.board, x, y);
			piece_selected.y = 48 * curr_color;

			switch (tolower(g_game.g_board.board[y][x])) {
			case PAWN:
				curr_piece = 0;
				break;
			case ROOK:
				curr_piece = 1;
				break;
			case KNIGHT:
				curr_piece = 2;
				break;
			case BISHOP:
				curr_piece = 3;
				break;
			case QUEEN:
				curr_piece = 4;
				break;
			case KING:
				curr_piece = 5;
				break;
			default:
				curr_piece = 6;
			}

			if (curr_piece < 6) {
				piece_selected.x = 48 * curr_piece;

				piece_pos.x = 26 + (50 * y);
				piece_pos.y = 450 - 47 - (50 * x);

				blit_all(chess_pieces, &piece_selected, &piece_pos);
			}
		}
	}

	SDL_FreeSurface(chess_pieces);
}

void print_moves(int p_x, int p_y) {
	SDL_Surface *x = SDL_LoadBMP("img/move.bmp");
	SDL_Surface *o = SDL_LoadBMP("img/capture.bmp");

	SDL_Rect piece_pos = { 0, 0, 50, 50 };

	move *curr = g_game.moves;

	if (check_img_load(x))
		clear_green(x);
	if (check_img_load(o))
		clear_green(o);

	while (curr != NULL) {
		if (curr->from[0] == p_y && curr->from[1] == p_x) {

			piece_pos.x = 28 + (50 * curr->to[1]);
			piece_pos.y = 450 - 48 - (50 * curr->to[0]);

			if (get_color(g_game.g_board.board, curr->to[0], curr->to[1]) == -1) {
				blit_all(x, NULL, &piece_pos);
			}
			else {
				blit_all(o, NULL, &piece_pos);
			}
		}
		curr = curr->next;
	}

	SDL_FreeSurface(x);
	SDL_FreeSurface(o);
}

int gui_promote() {
	SDL_Surface *bg = SDL_LoadBMP("img/pawn-promotion.bmp");
	SDL_Surface *selection = SDL_LoadBMP("img/promotion-select.bmp");

	SDL_Rect bg_pos = { 50, 140, 403, 140 };
	SDL_Rect bg_rect = { 0, 0, 403, 140 };
	SDL_Rect select_rect = { 51, 141, 200, 277 };

	SDL_Event e;

	int curr = 0, loop = 1, res = -1;

	bg_rect.y = 140 * g_game.g_board.turn;

	check_img_load(bg);
	if (check_img_load(selection))
		clear_green(selection);

	while (loop) {
		select_rect.x = 51 + (100 * curr);

		blit_all(bg, &bg_rect, &bg_pos);
		blit_all(selection, 0, &select_rect);

		print_window();

		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_ESCAPE:
					loop = 0;
					break;
				case SDLK_RIGHT:
					curr = (curr + 1) % 4;
					break;
				case SDLK_LEFT:
					curr = (curr + 3) % 4;
					break;
				case SDLK_RETURN:
					switch (curr) {
					case 0:
						res = ROOK;
						break;
					case 1:
						res = KNIGHT;
						break;
					case 2:
						res = BISHOP;
						break;
					case 3:
						res = QUEEN;
						break;
					}
					break;
				default:
					break;
				}
			}
		}
		if (res > -1) {
			loop = 0;
		}
	}

	SDL_FreeSurface(bg);
	SDL_FreeSurface(selection);

	return res;
}

void print_title() {
	SDL_Surface *color = SDL_LoadBMP("img/color-btn.bmp");
	SDL_Surface *check_msg = SDL_LoadBMP("img/check_msg.bmp");


	SDL_Rect color_pos = { 70, 9, 64, 33 };
	SDL_Rect color_selector = { 60, 3, 64, 35 };
	SDL_Rect check_pos = { 150, 0, 354, 50 };
	SDL_Rect check_selector = { 0, 100, 354, 50 };

	check_img_load(color);
	check_img_load(check_msg);

	if (g_game.g_board.turn == WHITE) {
		color_selector.x = 60;
		color_selector.y = 5;
	}
	else {
		color_selector.x = 60;
		color_selector.y = 50;
	}

	blit_all(color, &color_selector, &color_pos);

	if (g_game.moves == NULL) { /*TODO is this the place where its checkmate? the g_game.moves == null > ^^*/
		if (g_game.g_board.turn == WHITE)
			check_selector.y = 0;
		else
			check_selector.y = 50;

		blit_all(check_msg, &check_selector, &check_pos);
	}

	else if (check4check(g_game.g_board, ((g_game.g_board.turn + 1) % 2))) {
		check_selector.y = 100;
		blit_all(check_msg, &check_selector, &check_pos);
	}

	SDL_FreeSurface(color);
	SDL_FreeSurface(check_msg);
}




void clear_green(SDL_Surface *dirty) {
	if (SDL_SetColorKey(dirty, SDL_SRCCOLORKEY, SDL_MapRGB(dirty->format, 0, 255, 0)) != 0) {
		printf("ERROR: failed to set color key: %s\n", SDL_GetError());
		SDL_FreeSurface(dirty);
	}
}

void blit_all(SDL_Surface *s, SDL_Rect *r1, SDL_Rect *r2) {
	if (SDL_BlitSurface(s, r1, window, r2) != 0) {
		if (!img_error) {
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			img_error = 1;
		}
	}
}

int check_img_load(SDL_Surface *img) {
	SDL_Surface *err = SDL_LoadBMP("img/alfred_err.bmp");

	SDL_Event e;

	int res = 1, loop = 1;

	if (img == NULL) {
		res = 0;

		if (!img_error) {
			blit_all(err, NULL, NULL);
			print_window();
			img_error = 1;
			printf("ERROR: failed to load image.\n");

			/* Wait until key press */
			while (SDL_PollEvent(&e) != 0 || loop) {
				if (e.type == SDL_KEYDOWN)
					loop = 0;
			}
		}
	}

	SDL_FreeSurface(err);
	return res;
}
