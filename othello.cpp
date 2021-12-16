#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <utility>

#define _WIN32_WINNT 0x0500


int map[25][25];
int map_h = 20;
int map_w = 20;

int cr_turn = 0;
int p_score[3];

int y_resting = map_h + 6;


void move_cursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void init_map() {
	for (int a = 0; a < map_h; a++) {
		for (int b = 0; b < map_w; b++) {
			map[a][b] = 0;
		}
	}
	
	map[map_h/2-1][map_w/2-1] = 1;
	map[map_h/2-1][map_w/2] = 2;
	map[map_h/2][map_w/2-1] = 2;
	map[map_h/2][map_w/2] = 1;
}


int check_avail_moves(int c_x, int c_y) {
	if (map[c_y][c_x] != 0) {
		return 0;
	}
	
	int v[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
	int s = (cr_turn == 1) ? 2 : 1;
	
	for (int a = 0; a < 8; a++) {
		int x = c_x + v[a][0];
		int y = c_y + v[a][1];
		
		if (x < 0 || x > map_w) {
			continue;
		}
		
		if (y < 0 || y > map_h) {
			continue;
		}

		if (map[y][x] != s) {
			continue;
		}
		
		do {
			x += v[a][0];
			y += v[a][1];
			
			if (x < 0 || x > map_w) {
				break;
			}
			
			if (y < 0 || y > map_h) {
				break;
			}
	
			if (map[y][x] == cr_turn) {
				return 1;
			}
	
			if (map[y][x] != s) {
				break;
			}
		} 
		while(map[y][x] != cr_turn);		
	}
	
	return 0;
}


int print_map(int avail_pos[][2]) {
	system("cls");
	
	int n_avail_pos = 0;
	
	for (int a = 0; a < map_w + 1; a++) {
		printf("%c%c", (a == 0) ? 2505 : 2509, 2509);
	}
	
	printf("%c\n", 2491);
	
	for (int a = 0; a < map_h; a++) {
		
		printf("%c ", 2490);
		
		// p1 (black) : *
		// p2 (white) : o
				
		for (int b = 0; b < map_w; b++) {
			int avail = check_avail_moves(b, a);
			printf("%c ", (avail == 1) ? 176 : (map[a][b] == 0) ? ' ' : (map[a][b] == 1) ? '*' : 248);
			
			if (avail == 1) {
				avail_pos[n_avail_pos][0] = b;
				avail_pos[n_avail_pos++][1] = a;
			}
		}
		
		printf("%c\n", 2490);
	}
	
	for (int a = 0; a < map_w + 1; a++) {
		printf("%c%c", (a == 0) ? 2504 : 2509, 2509);
	}
	
	printf("%c\n", 2492);
	
	return n_avail_pos;
}


void init_game() {
	cr_turn = 1;
	p_score[1] = 2;
	p_score[2] = 2;

	init_map();
}


void activate_selected(int x, int y) {
	move_cursor((x + 1) * 2, y + 1);
	printf("%c", 178);	
	
	move_cursor(0, y_resting);
}


void deactivate_selected(int x, int y) {
	move_cursor((x + 1) * 2, y + 1);
	printf("%c", 176);
	
	move_cursor(0, y_resting);
}


void draw_menu() {
	int x_position = (map_w + 4) * 2;
	int y_position = 0;
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	
	move_cursor(x_position, y_position++);
	printf("%-37s", "");
	
	move_cursor(x_position, y_position++);
	printf("%-37s", "              SADTHELLO");
	
	move_cursor(x_position, y_position++);
	printf("%-37s", "");
	
	y_position++;
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
	move_cursor(x_position, y_position++);
	printf("%-16s", "CURRENT PLAYER:");
	
	y_position++;
	
	char white_ind[256];
	sprintf(white_ind, "(WHITE) %c", 248);
	
	printf("%20s", (cr_turn == 1) ? "(BLACK) *" : white_ind);
	
	move_cursor(x_position, y_position++);
	printf("%-16s", "BLACK SCORE:");
	printf("%20d", p_score[1]);
	
	move_cursor(x_position, y_position++);
	printf("%-16s", "WHITE SCORE:");
	printf("%20d", p_score[2]);
	
	move_cursor(0, y_resting);
}


void draw_legends() {
	int y_position = map_h + 4;
	move_cursor(0, y_position);
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
	
	char menu[256];
	sprintf(menu, "   [ESC]EXIT   [%c %c]CHANGE POSITION   [ENTER]PLACE", 273, 272);
	
	printf("%-86s" , menu); 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	
	move_cursor(0, y_resting);
}


void place_bead(int c_x, int c_y) {
	map[c_y][c_x] = cr_turn;
	p_score[cr_turn]++;
	
	int v[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
	int s = (cr_turn == 1) ? 2 : 1;
	
	for (int a = 0; a < 8; a++) {
		int x = c_x + v[a][0];
		int y = c_y + v[a][1];
		
		if (x < 0 || x > map_w) {
			continue;
		}
		
		if (y < 0 || y > map_h) {
			continue;
		}

		if (map[y][x] != s) {
			continue;
		}
		
		do {
			x += v[a][0];
			y += v[a][1];
			
			if (x < 0 || x > map_w) {
				break;
			}
			
			if (y < 0 || y > map_h) {
				break;
			}
	
			if (map[y][x] == cr_turn) {
				x += v[a][0] * -1;
				y += v[a][1] * -1;
				
				do {	
					if (map[y][x] != cr_turn) {
						p_score[cr_turn]++;
						p_score[s]--;
					}
					
					map[y][x] = cr_turn;
					
					x += v[a][0] * -1;
					y += v[a][1] * -1;
				}
				while( map[y][x] != cr_turn );
				
				break;
			}
	
			if (map[y][x] != s) {
				break;
			}
		} 
		while(map[y][x] != cr_turn);		
	}
}


int game() {
	init_game();
	
	int s_game = 0;
	
	while (!s_game) {
		int avail_positions[map_h * map_w][2] = {0};
		int n = print_map(avail_positions);

		int k = 0;
		int active_position = 0;
		
		draw_legends();
		draw_menu();
		
		do{
			activate_selected(avail_positions[active_position][0], avail_positions[active_position][1]);
			
			k = getch();
			
			switch(k) {
				case 75:
					if (active_position != 0) {
						deactivate_selected(avail_positions[active_position][0], avail_positions[active_position][1]);
						active_position--;
					}
					break;
				case 77:
					if (active_position != n - 1) {
						deactivate_selected(avail_positions[active_position][0], avail_positions[active_position][1]);
						active_position++;
					}
					break;
				case 13:
					place_bead(avail_positions[active_position][0], avail_positions[active_position][1]);
				default:
					break;
			}
		}
		while(k != 13);
		
		cr_turn = (cr_turn % 2) + 1;
	}
}

int main() {
	int winner = game();
}
