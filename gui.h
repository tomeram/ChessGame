#ifndef GUI_
#define GUI_

#include "chessprog.h"


#define WIN_W 640
#define WIN_H 480
#define LOAD 0
#define SAVE 1
#define SLOTS 5

/* functions */

void gui_pvp_settings();
void gui_pvc_settings();
void gui_saveload_menu(int);
void print_window();
int main_menu();
char check_empty_slots();
void draw_empty_slots(char);
void print_title();
void gui_game();
void print_pieces();
void print_moves();
int gui_promote();
void clear_green(SDL_Surface*);
void blit_all(SDL_Surface*, SDL_Rect*, SDL_Rect*);
int check_img_load(SDL_Surface*);

#endif
