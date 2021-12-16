#include<stdio.h>
#include<windows.h>

int map[25][25];
int map_h = 20;
int map_w = 20;

int cr_turn = 0;
int p1_score = 0;
int p2_score = 0;


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


int check_avail_moves(int c_player, int c_x, int c_y) {
	if (map[c_y][c_x] != 0) {
		return 0;
	}
	
	int v[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};
	
	int s = (c_player == 1) ? 2 : 1;
	
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
		
//		printf("P: %d %d\n", x, y);
		do {
			x += v[a][0];
			y += v[a][1];
//			printf("%d %d %d = %d\n", x, y, map[y][x], c_player);
			
			if (x < 0 || x > map_w) {
				break;
			}
			
			if (y < 0 || y > map_h) {
				break;
			}
	
			if (map[y][x] == c_player) {
//				printf("yes");
				return 1;
			}
	
			if (map[y][x] != s) {
				break;
			}
		} 
		while(map[y][x] != c_player);		
	}
	
	return 0;
}


void print_map(int avail_pos[][2]) {
	system("cls");
	
	printf("CURRENT PLAYER: %c\n", (cr_turn == 1) ? '*' : 248);
	
	for (int a = 0; a < map_w + 1; a++) {
		printf("%c%c", (a == 0) ? 2505 : 2509, 2509);
	}
	
	printf("%c\n", 2491);
	
	for (int a = 0; a < map_h; a++) {
		
		printf("%c ", 2490);
		
		// p1 (black) : *
		// p2 (white) : o
				
		for (int b = 0; b < map_w; b++) {
			int avail = check_avail_moves(cr_turn, b, a);
			printf("%c ", (avail == 1) ? 176 : (map[a][b] == 0) ? ' ' : (map[a][b] == 1) ? '*' : 248);
		}
		
		printf("%c\n", 2490);
	}
	
	for (int a = 0; a < map_w + 1; a++) {
		printf("%c%c", (a == 0) ? 2504 : 2509, 2509);
	}
	
	printf("%c\n", 2492);
}


void init_game() {
	cr_turn = 1;
	p1_score = 2;
	p2_score = 2;

	init_map();
}


int game() {
	init_game();
	
	int s_game = 0;
	
	while (!s_game) {
		int avail_positions[map_h * map_w][2];
		print_map(avail_positions);
		
		
		getchar();
		cr_turn = (cr_turn % 2) + 1;
	}
}

int main() {
	int winner = game();
}
